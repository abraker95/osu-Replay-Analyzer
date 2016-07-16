#include "osu_standard.h"

int OSUSTANDARD::FindHitobjectAt(std::vector<Hitobject>* _hitobjects, double _time)
{
	int start = 0;
	int end = _hitobjects->size() - 2;
	int mid;

	while (start <= end)
	{
		mid = (start + end) / 2;

		if (BTWN((*_hitobjects)[mid].getTime(), _time, (*_hitobjects)[mid].getEndTime()))
			return mid;

		if (BTWN((*_hitobjects)[mid].getTime(), _time, (*_hitobjects)[mid + 1].getTime()))
			return mid + 1;

		if (_time < (*_hitobjects)[mid].getTime())
			end = mid - 1;
		else 
			start = mid + 1;
	}
}
