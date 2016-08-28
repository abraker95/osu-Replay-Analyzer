#include "osu_standard.h"

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
	if(_num <= 0)
		return std::vector<std::pair<irr::core::vector2d<double>, double>>();

	int i = FindHitobjectAt(_hitobjects, _time);

	// Out of bound check
	if (!BTWN(1, i, _hitobjects->size() - 1))
		return std::vector<std::pair<irr::core::vector2d<double>, double>>();

	std::vector<std::pair<irr::core::vector2d<double>, double>> points;
	if ((*_hitobjects)[i]->isHitobjectLong())
	{
		// if we are behind the slider
		if (_time < (*_hitobjects)[i]->getTime())
		{
			/*// record the slider's beginning point
			std::pair<irr::core::vector2d<double>, double> point;
				point.first = (*_hitobjects)[i]->getPos();
				point.second = (*_hitobjects)[i]->getTime();
				points.push_back(point);*/

			// go record from previous slider's end
			//std::vector<std::pair<irr::core::vector2d<double>, double>> newPoints;
			//newPoints = getPattern(_hitobjects, _num - 1, _interval, (*_hitobjects)[i - 1]->getEndTime(), _skipSliders);

			//return Merge(points, newPoints);

			// go record from previous slider's end
			return getPattern(_hitobjects, _num - 1, _interval, (*_hitobjects)[i - 1]->getEndTime(), _skipSliders);
		}
		else if(_skipSliders)
		{
			// go iterate backwards, skipping middle of slider
			double time;
			double closeToEndTime = ABS(_time - (*_hitobjects)[i]->getEndTime());
			double closeToBegTime = ABS(_time - (*_hitobjects)[i]->getTime());

			// record this slider's beginning point
			std::pair<irr::core::vector2d<double>, double> point;

			if (closeToEndTime < closeToBegTime)
			{
				point.first = (*_hitobjects)[i]->getSlider()->GetSliderPos((*_hitobjects)[i]->getEndTime());
				point.second = (*_hitobjects)[i]->getEndTime();
				time = (*_hitobjects)[i]->getTime();
			}
			else
			{
				point.first = (*_hitobjects)[i]->getPos();
				point.second = (*_hitobjects)[i]->getTime();
				time = (*_hitobjects)[i - 1]->getEndTime();
			}
	
			points.push_back(point);

			std::vector<std::pair<irr::core::vector2d<double>, double>> newPoints;
			newPoints = getPattern(_hitobjects, _num - 1, _interval, time, _skipSliders);

			return Merge(points, newPoints);
		}
		else
		{
			// record this point on the slider
			std::pair<irr::core::vector2d<double>, double> point;
			point.first = (*_hitobjects)[i]->getSlider()->GetSliderPos(_time);
			point.second = _time;
			points.push_back(point);

			// go to previous point on slider 
			std::vector<std::pair<irr::core::vector2d<double>, double>> newPoints;
			newPoints = getPattern(_hitobjects, _num - 1, _interval, _time - _interval, _skipSliders);

			return Merge(points, newPoints);
		}
	}
	else
	{
		// record this hitobject
		std::pair<irr::core::vector2d<double>, double> point;
			point.first = (*_hitobjects)[i]->getPos();
			point.second = (*_hitobjects)[i]->getTime();
		points.push_back(point);
		
		// go to previous hitobject
		std::vector<std::pair<irr::core::vector2d<double>, double>> newPoints;
		newPoints = getPattern(_hitobjects, _num - 1, _interval, (*_hitobjects)[i - 1]->getEndTime(), _skipSliders);

		return Merge(points, newPoints);
	}
}

// Returns (pos), (time), and (is mid slider?) of the next tick point
osu::TIMING OSUSTANDARD::getNextTickPoint(std::vector<Hitobject*>& _hitobjects, long* _time)
{
	osu::TIMING tickPoint;
	int i = FindHitobjectAt(_hitobjects, *_time, true);
	
	// if we reached the end, make timing.data = -1
	if (i >= _hitobjects.size() - 1) return osu::TIMING({0, -1}); 

	// if the time is between 2 hitobjects, return the start of the next hitobject
	if (!isHitobjectAt(_hitobjects, *_time - 1, *_time))
	{
		*_time = _hitobjects[i + 1]->getTime();
		irr::core::vector2d<double> pos = _hitobjects[i + 1]->getPos();

		tickPoint.pos = irr::core::vector2df(pos.X, pos.Y);
		tickPoint.time = *_time;
		tickPoint.data = 0;
		tickPoint.press = false;
		return tickPoint;
	}
	else
	{
		// if it is a slider, return the next closest tick
		if (_hitobjects[i]->isHitobjectLong())
		{
			SliderHitObject* slider = _hitobjects[i]->getSlider();
			std::vector<int> ticks = slider->getTickTimes();

			for (int tick = 1; tick < ticks.size(); tick++)
			{
				if (BTWN(ticks[tick - 1], *_time, ticks[tick]))
				{
					*_time = ticks[tick];
					irr::core::vector2d<double> pos = slider->GetSliderPos(ticks[tick]);

					tickPoint.pos = irr::core::vector2df(pos.X, pos.Y);
					tickPoint.time = *_time;
					tickPoint.data = 0;
					tickPoint.press = true;
					return tickPoint;
				}
			}

			// else slider had no second tick
			*_time = _hitobjects[i + 1]->getTime();
			irr::core::vector2d<double> pos = _hitobjects[i + 1]->getPos();

			tickPoint.pos = irr::core::vector2df(pos.X, pos.Y);
			tickPoint.time = *_time;
			tickPoint.data = 0;
			tickPoint.press = false;
			return tickPoint;
		}
		else // if it is a regular hitobject, return the start of the next hitobject
		{
			*_time = _hitobjects[i + 1]->getTime();
			irr::core::vector2d<double> pos = _hitobjects[i + 1]->getPos();

			tickPoint.pos = irr::core::vector2df(pos.X, pos.Y);
			tickPoint.time = *_time;
			tickPoint.data = 0;
			tickPoint.press = false;
			return tickPoint;
		}
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