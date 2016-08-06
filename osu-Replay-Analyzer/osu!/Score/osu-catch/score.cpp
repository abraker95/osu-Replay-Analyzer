#include "score.h"

std::vector<osu::TIMING> OSUCATCH::accTimings;
static Play* play;

void OSUCATCH::genAccTimings(Play* _play)
{
	accTimings.clear();
	std::vector<osu::TIMING>().swap(accTimings);

	play = _play;
}