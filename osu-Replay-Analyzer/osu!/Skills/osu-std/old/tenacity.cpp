#include "tenacity.h"
#include "analysis.h"

/*
double getTenacitySkill(Beatmap* _beatmap, int _time)
{
	double tenacity = 0;
	int index = _beatmap->FindHitobjectAt(_time) + 1;

	if (index > _beatmap->hitObjects.size() - 1) 	// if it's beyond the end
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

		if (_beatmap->hitObjects[index - 1]->IsHitObjectType(SLIDER))
			if (_time < _beatmap->hitObjects[index - 1]->slider->GetLastTickTime())
				index--;

		pattern = getPattern(_beatmap, _time, index, 3);
		
		/// TODO
	}

	return tenacity;
}*/