#include "osu_standard.h"
#include "osuCalc.h"

#include "Filestructure\Play.h"

// dir = true -> look forward
// dir = false -> look backward
int OSUSTANDARD::FindHitobjectAt(std::vector<Hitobject*>& _hitobjects, long _time, bool _dir)
{
	int start = 0;
	int end = _hitobjects.size() - 2;
	int mid;

	while (start <= end)
	{
		mid = (start + end) / 2;

		// Between ends of a hold object
		if (BTWN(_hitobjects[mid]->getTime(), _time, _hitobjects[mid]->getEndTime()))
		{
			// Return next object if next object's timings overlap with this one
			if (BTWN(_hitobjects[mid + 1]->getTime(), _time, _hitobjects[mid + 1]->getEndTime()))
				return mid + 1;
			else
				return mid;
		}
			

		// Between some two objects
		if (BTWN(_hitobjects[mid]->getEndTime(), _time, _hitobjects[mid + 1]->getTime()))
			return mid + (long)_dir;

		if (_time < _hitobjects[mid]->getTime())
			end = mid - 1;
		else 
			start = mid + 1;
	}
}

std::vector<osu::TIMING> OSUSTANDARD::getPattern(std::vector<Hitobject*>& _hitobjects, int _num, double _interval, long _time, bool _skipSliders, bool _newHitobject)
{
	// make sure we have something to get
	if(_num <= 0) return std::vector<osu::TIMING>();

	int i = FindHitobjectAt(_hitobjects, _time);
	if (!BTWN(1, i, _hitobjects.size() - 1)) return std::vector<osu::TIMING>(); // Out of bound check

	std::vector<osu::TIMING> points;
	if (_hitobjects[i]->isHitobjectLong())
	{
		long startHitobject = _hitobjects[i]->getTime();
		long endHitobject = _hitobjects[i]->getEndTime();
		irr::core::vector2d<double> pos = _hitobjects[i]->getSlider()->GetSliderPos(_time);

		// if we are behind the slider
		if (_time < startHitobject)
		{
			// go record from previous slider's end
			return getPattern(_hitobjects, _num - 1, _interval, _hitobjects[i]->getEndTime(), _skipSliders, false);
		}
		else if(_skipSliders)
		{
			// go iterate backwards, skipping middle of slider
			long time;
			long sliderMidTime = (endHitobject + startHitobject) / 2;
			bool prevHitobject;

			// record this slider's beginning or end point
			osu::TIMING point;

			if (sliderMidTime < _time)
			{
				point.pos = pos;
				point.time = _hitobjects[i]->getEndTime();
				point.press = _newHitobject;
				time = _hitobjects[i]->getTime();

				prevHitobject = false;
			}
			else
			{
				point.pos = pos;
				point.time = startHitobject;
				point.press = _newHitobject;
				time = _hitobjects[i - 1]->getEndTime();

				prevHitobject = true;
			}
	
			points.push_back(point);

			std::vector<osu::TIMING> newPoints;
			newPoints = getPattern(_hitobjects, _num - 1, _interval, time, _skipSliders, prevHitobject);

			return Merge(points, newPoints);
		}
		else
		{
			// record this point on the slider
			osu::TIMING point;
				point.pos = pos;
				point.time = _time;
				point.press = _newHitobject;
				points.push_back(point);

			// go to previous point on slider 
			std::vector<osu::TIMING> newPoints;
			newPoints = getPattern(_hitobjects, _num - 1, _interval, _time - _interval, _skipSliders);

			return Merge(points, newPoints);
		}
	}
	else
	{
		// record this hitobject
		osu::TIMING point;
			point.pos = _hitobjects[i]->getPos();
			point.time = _hitobjects[i]->getTime();
			point.press = _newHitobject;
		points.push_back(point);
		
		// go to previous hitobject
		std::vector<osu::TIMING> newPoints;
		newPoints = getPattern(_hitobjects, _num - 1, _interval, _hitobjects[i - 1]->getEndTime(), _skipSliders, true);

		return Merge(points, newPoints);
	}
}

// Returns (pos), (time), and (is mid slider?) of the next tick point
osu::TIMING OSUSTANDARD::getNextTickPoint(std::vector<Hitobject*>& _hitobjects, long _time, long* _timeItr)
{
	osu::TIMING tickPoint;
	int i = FindHitobjectAt(_hitobjects, _time, true);

	// if we reached the end, make timing.data = -1
	if (i >= _hitobjects.size() - 1) return osu::TIMING({ osu::TIMING::INVAID, osu::TIMING::INVAID });

	// If the time is before the hitobject's starting point, then return the hitobject's starting point
	if (_time < _hitobjects[i]->getTime())
	{
		tickPoint.pos = _hitobjects[i]->getPos();
		tickPoint.time = _hitobjects[i]->getTime();
		tickPoint.data = 0;
		tickPoint.press = false;

		if (_timeItr != nullptr) *_timeItr = tickPoint.time;
		return tickPoint;
	}

	// If the time if after the hitobject's starting point take action depending if the hitobject is a circle or a slider

	// It's a circle, then return the next hitobject's starting point
	if (!_hitobjects[i]->isHitobjectLong())
	{
		tickPoint.pos = _hitobjects[i + 1]->getPos();
		tickPoint.time = _hitobjects[i + 1]->getTime();
		tickPoint.data = 0;
		tickPoint.press = false;

		if (_timeItr != nullptr) *_timeItr = tickPoint.time;
		return tickPoint;
	}

	// It's a slider, then go through the slider's ticks
	if (_hitobjects[i]->isHitobjectLong())
	{
		SliderHitObject* slider = _hitobjects[i]->getSlider();
		std::vector<int> ticks = slider->getTickTimes();

		for (int tick : ticks)
		{
			// If the time occures before a tick, return that tick
			if (_time < tick)
			{
				tickPoint.pos = slider->GetSliderPos(tick);
				tickPoint.time = tick;
				tickPoint.data = 0;
				tickPoint.press = false;

				if (_timeItr != nullptr) *_timeItr = tickPoint.time;
				return tickPoint;

			}
		}

		// The time is after all the slider's ticks. Check if the viewTime is after the slider's end
		if (_time < _hitobjects[i]->getEndTime())
		{
			tickPoint.pos = slider->GetSliderPos(_hitobjects[i]->getEndTime());
			tickPoint.time = _hitobjects[i]->getEndTime();
			tickPoint.data = 0;
			tickPoint.press = false;

			if (_timeItr != nullptr) *_timeItr = tickPoint.time;
			return tickPoint;
		}
		
		// Otherwise, the time is after the current hitobject, but before the start of the next hitobject. Return the next hitobject's starting point
		tickPoint.pos = _hitobjects[i + 1]->getPos();
		tickPoint.time = _hitobjects[i + 1]->getTime();
		tickPoint.data = 0;
		tickPoint.press = false;

		if (_timeItr != nullptr) *_timeItr = tickPoint.time;
		return tickPoint;
	}
}

bool OSUSTANDARD::isHitobjectAt(std::vector<Hitobject*>& _hitobjects, long _prevTime, long _currTime)
{
	int i = FindHitobjectAt(_hitobjects, _currTime);
	if (BTWN(_prevTime, _hitobjects[i]->getTime(), _currTime)) return true;						// a normal note
	if (BTWN(_hitobjects[i]->getTime(), _currTime, _hitobjects[i]->getEndTime())) return true;  // a hold note

	return false;
}

int OSUSTANDARD::getButtonState(int _prevKey, int _currKey)
{
	int state = 0;
	for (int i = 0; i < 4; i++) // osu standard has 4 buttons
	{
		int prevKey = _prevKey & (1 << i);
		int currKey = _currKey & (1 << i);

		// trigger down
		if ((prevKey == 0) && (currKey != 0))
		{
			if (state == 2)	state = 3; // press & release
			else			state = 1; // pressed
		}

		// trigger up
		if ((prevKey != 0) && (currKey == 0))
		{
			if (state == 1)	state = 3; // press & release
			else			state = 2; // release
		}
	}

	return state;
}

int OSUSTANDARD::getButtonState(int _prevKey, int _currKey, int _key)
{
	int state = 0;

	int prevKey = _prevKey & (1 << _key);
	int currKey = _currKey & (1 << _key);

	// trigger down
	if ((prevKey == 0) && (currKey != 0))
		state = 1; // pressed

	// trigger up
	if ((prevKey != 0) && (currKey == 0))
		state = 2; // release

	return state;
}

int OSUSTANDARD::getNumHitobjectsVisibleAt(Play* _play, int _index, double _opacity)
{
	std::pair<int, int> indices = getIndicesVisibleAt(_play, _play->beatmap->getHitobjects()[_index]->getTime(), _opacity);
	return indices.second - indices.first;
}

std::pair<int, int> OSUSTANDARD::getIndicesVisibleAt(Play* _play, int _time, double _opacity)
{
	std::vector<Hitobject*> hitobjects = _play->beatmap->getHitobjects();
	int index = _play->beatmap->FindHitobjectAt(_time) - 1;
	std::pair<int, int> range;

	// Find first note visible
	for (index = MAX(index, 0); index < hitobjects.size(); index++)
	{
		if (isVisibleAt(*hitobjects[index], _time, _play->getMod()->getAR(), _play->getMod()->getModifier().HD))
			break;
	}
	range.first = index;

	// Find last note visible
	for (; index < hitobjects.size(); index++)
	{
		if (!(isVisibleAt(*hitobjects[index], _time, _play->getMod()->getAR(), _play->getMod()->getModifier().HD)))
			break;
	}
	range.second = index;

	return range;
}


bool OSUSTANDARD::isVisibleAt(Hitobject& _hitobject, int _time, double _AR, bool _hidden)
{
	double opacity = getOpacityAt(_hitobject, _time, _AR, _hidden);
	if (opacity > 0.0)
	{
		return true;
	}
	else
	{
		return false;
	}
}


std::pair<int, int> OSUSTANDARD::getVisiblityTimes(Hitobject& _hitobject, double _AR, bool _hidden, double _opacityStart, double _opacityEnd)
{
	double preappTime = _hitobject.getTime() - AR2ms(_AR);	// Pre-appear time. Time the object start appearing
	std::pair<int, int> times;

	if (_hidden)
	{
		double fadeinDuration = 0.4*AR2ms(_AR);				// how long the fadein period is
		double fadeinTimeEnd = preappTime + fadeinDuration; // When it is fully faded in

		times.first = getValue(preappTime, fadeinTimeEnd, _opacityStart);


		// If it's a slider, then the fade out period lasts from when it's fadedin to
		// 70% to the time it the slider ends
		if (_hitobject.IsHitObjectType(SLIDER))
		{
			double fadeoutDuration = (_hitobject.getEndTime() - fadeinTimeEnd); // how long the fadeout period is
			double fadeoutTimeEnd = fadeinTimeEnd + fadeoutDuration;		   // When it is fully faded out
			times.second = getValue(fadeinTimeEnd, fadeoutTimeEnd, 1.0 - _opacityEnd);

			return times;
		}
		else
		{
			double fadeoutDuration = 0.7*(_hitobject.getTime() - fadeinTimeEnd);		// how long the fadeout period is
			double fadeoutTimeEnd = fadeinTimeEnd + fadeoutDuration;	// When it is fully faded out
			times.second = getValue(fadeinTimeEnd, fadeoutTimeEnd, 1.0 - _opacityStart); // <-- no this is not a mistake :D

			return times;
		}
	}
	else
	{
		double fadeinDuration = MIN(AR2ms(_AR), 400);		// how long the fadein period is
		double fadeinTimeEnd = preappTime + fadeinDuration; // When it is fully faded in

															// Fadein period always lasts from preamp time to 400 ms after preamp time or
															// when the object needs to be hit, which ever is smaller
		times.first = getValue(preappTime, fadeinTimeEnd, _opacityStart);

		// If it is during the slider hold period, then it's fully visible.
		// Otherwise, it's not visible anymore.
		if (_hitobject.IsHitObjectType(SLIDER))
		{
			times.second = _hitobject.getEndTime();
			return times;
		}
		else
		{
			times.second = _hitobject.getTime();
			return times;
		}
	}
}

double OSUSTANDARD::getOpacityAt(Hitobject& _hitobject, int _time, double _AR, bool _hidden)
{
	double preampTime = _hitobject.getTime() - AR2ms(_AR);	// Time when the AR goes into effect

	if (_hidden)
	{
		double fadeinDuration = 0.4*AR2ms(_AR);				// how long the fadein period is
		double fadeinTimeEnd = preampTime + fadeinDuration; // When it is fully faded in

															// Fadein period always lasts from preamp time to 40% from preamp time to hit time
		double percentFadeIn = getPercent(preampTime, _time, fadeinTimeEnd);

		// If it's not fully faded in, then we haven't gotten up to the later stuff 
		if (percentFadeIn < 1.0)
		{
			return percentFadeIn;
		}
		else // fadeout
		{
			// If it's a slider, then the fade out period lasts from when it's fadedin to
			// 70% to the time it the slider ends
			if (_hitobject.IsHitObjectType(SLIDER))
			{
				double fadeoutDuration = (_hitobject.getSlider()->GetLastTickTime() - fadeinTimeEnd); // how long the fadeout period is
				double fadeoutTimeEnd = fadeinTimeEnd + fadeoutDuration;		   // When it is fully faded out
				return (1.0 - getPercent(fadeinTimeEnd, _time, fadeoutTimeEnd));
			}
			else
			{
				double fadeoutDuration = 0.7*(_hitobject.getTime() - fadeinTimeEnd);		// how long the fadeout period is
				double fadeoutTimeEnd = fadeinTimeEnd + fadeoutDuration;	// When it is fully faded out
				return (1.0 - getPercent(fadeinTimeEnd, _time, fadeoutTimeEnd));
			}
		}
	}
	else
	{
		double fadeinDuration = MIN(AR2ms(_AR), 400);		// how long the fadein period is
		double fadeinTimeEnd = _hitobject.getTime(); // When it is fully faded in

										   // Fadein period always lasts from preamp time to 400 ms after preamp time or
										   // when the object needs to be hit, which ever is smaller
		double percentFadeIn = getPercent(preampTime, _time, fadeinTimeEnd);

		// If it's not fully faded in, then we haven't gotten up to the later stuff 
		if (percentFadeIn < 1.0)
		{
			return percentFadeIn;
		}
		else // fadeout
		{
			// If it is during the slider hold period, then it's fully visible.
			// Otherwise, it's not visible anymore.
			if (_hitobject.IsHitObjectType(SLIDER))
			{
				if (_time > _hitobject.getEndTime())
				{
					// \TODO: Opacity when pressed and when missed
					return 0.0;
				}
				else
				{
					return 1.0;
				}
			}
			else
			{
				if (_time > _hitobject.getTime())
				{
					// \TODO: Opacity when pressed and when missed
					return 0.0;
				}
				else
				{
					return 1.0;
				}
			}
		}
	}
}
