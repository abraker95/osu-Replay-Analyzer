#include "osu_standard.h"

int OSUSTANDARD::FindHitobjectAt(std::vector<Hitobject*>* _hitobjects, double _time)
{
	int start = 0;
	int end = _hitobjects->size() - 2;
	int mid;

	while (start <= end)
	{
		mid = (start + end) / 2;

		// Between ends of a hold object
		if (BTWN((*_hitobjects)[mid]->getTime(), _time, (*_hitobjects)[mid]->getEndTime()))
			return mid;

		// Between some two objects
		if (BTWN((*_hitobjects)[mid]->getEndTime(), _time, (*_hitobjects)[mid + 1]->getTime()))
			return mid;

		if (_time < (*_hitobjects)[mid]->getTime())
			end = mid - 1;
		else 
			start = mid + 1;
	}
}

std::vector<std::pair<irr::core::vector2d<double>, double>> OSUSTANDARD::getPattern(std::vector<Hitobject*>* _hitobjects, int _num, double _interval, double _time, bool _skipSliders)
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

bool OSUSTANDARD::isHitobjectAt(std::vector<Hitobject*>& _hitobjects, double _prevTime, double _currTime)
{
	int i = FindHitobjectAt(&_hitobjects, _currTime);
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