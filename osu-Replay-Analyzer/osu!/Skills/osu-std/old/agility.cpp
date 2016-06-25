#include "agility.h"
#include "analysis.h"

double getAgilitySkill(Beatmap* _beatmap, int _time)
{
	double agility = 0;
	/*int index = getHitcircleAt(_hitcircles, _time) + 1;

	if (index > _hitcircles.size() - 1) 	// if it's beyond the end
	{
		agility = 0;
	}
	else if (index < 3) // if it's at or before the beggining
	{
		agility = 0;
	}
	else
	{
		std::vector<std::tuple<int, int, int, int>> pattern;

		if (_hitcircles[index - 1].isSlider())
			if (_time < _hitcircles[index - 1].getEndTime())
				index--;

		pattern = getPattern(_hitcircles, _time, index, _CS, 3);

		// TODO
	}*/

	return agility;
}