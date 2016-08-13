#include "osuStructs.h"
#include "../utils/mathUtils.h"

int osu::FindTimingAt(std::vector<TIMING>& _timings, double _time)
{
	int start = 0;
	int end = _timings.size() - 2;
	int mid;

	while (start <= end)
	{
		mid = (start + end) / 2;

		if (BTWN(_timings[mid].time, _time, _timings[mid + 1].time))
			return mid + 1;

		if (_time < _timings[mid].time)
			end = mid - 1;
		else
			start = mid + 1;
	}
}

bool sortATimings(osu::TIMING i, osu::TIMING j)
{
	return i.time < j.time;
}


void osu::SortByTime(std::vector<TIMING>& _timings)
{
	std::sort(_timings.begin(), _timings.end(), sortATimings);
}