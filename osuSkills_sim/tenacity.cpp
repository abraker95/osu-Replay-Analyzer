#include "tenacity.h"
#include "analysis.h"

double getTenacitySkill(std::vector<Hitcircle> &_hitcircles, int _time, double _CS, double _AR, bool _hidden)
{
	double tenacity = 0;
	int index = getHitcircleAt(_hitcircles, _time) + 1;

	if (index > _hitcircles.size() - 1) 	// if it's beyond the end
	{
		tenacity = 0;
	}
	else if (index < 3) // if it's at or before the beggining
	{
		tenacity = 0;
	}
	else
	{
		std::vector<std::tuple<int, int, int, int>> pattern;

		if (_hitcircles[index - 1].isSlider())
			if (_time < _hitcircles[index - 1].getEndTime())
				index--;

		pattern = getPattern(_hitcircles, _time, index, _CS, 3);
		
		// TODO
	}

	return tenacity;
}