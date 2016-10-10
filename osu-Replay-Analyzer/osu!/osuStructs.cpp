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

	return INT_MIN;
}

bool sortATimings(osu::TIMING i, osu::TIMING j)
{
	return i.time < j.time;
}


void osu::SortByTime(std::vector<TIMING>& _timings)
{
	std::sort(_timings.begin(), _timings.end(), sortATimings);
}

// Exponential averaging method
void osu::ExpAverage(std::vector<TIMING>& _timings, double _factor)
{
	if (_timings.size() < 2) return;

	for (int i = 1; i < _timings.size(); i++)
		_timings[i].data = (_timings[i].data * _factor) + (_timings[i - 1].data * (1.0 - _factor));
}

// Moving average method
void osu::MovAverage(std::vector<TIMING>& _timings, int _blockSize)
{
	double blockAvg;

	for(int i = 0; i < _timings.size(); i++)
	{
		blockAvg = 0;
		for (int k = 0; k < _blockSize; k++)
		{
			if (i - k > 0)	
				blockAvg += _timings[i - k].data;
		}

		blockAvg /= (double)_blockSize;
		_timings[i].data = blockAvg;
	}
}

// Step average method (segments the data by blocks and averages each individual block (non continuously))
void osu::StpAverage(std::vector<TIMING>& _timings, int _blockSize)
{
	double blockAvg;

	for(int i = _blockSize; i < _timings.size(); i+=_blockSize)
	{
		blockAvg = 0;
		for (int k = 0; k < _blockSize; k++)
		{
			if (i - k > 0)	
				blockAvg += _timings[i - k].data;
		}

		blockAvg /= (double)_blockSize;
		_timings[i].data = blockAvg;

		for(int k = 0; k < _blockSize; k++)
			_timings[i - k].data = lerp(_timings[i].data, _timings[i - _blockSize].data, getPercent(0, k, _blockSize));
	}

	// handle the last block
	int lastBlockStart = _timings.size() - (_timings.size() % _blockSize);
	int lastBlockSize = _timings.size() - lastBlockStart;

	blockAvg = 0;
	for (int k = 0; k < _blockSize; k++)
	{
		if (_timings.size() - k - 1 > 0)
			blockAvg += _timings[_timings.size() - k - 1].data;
	}

	for (int k = 0; k < _blockSize; k++)
		_timings[_timings.size() - k - 1].data = lerp(_timings[_timings.size() - 1].data, _timings[lastBlockStart].data, getPercent(0, k, _blockSize));

}