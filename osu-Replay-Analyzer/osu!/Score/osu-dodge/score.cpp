#include "score.h"

std::vector<osu::TIMING> OSUDODGE::accTimings;
static Play* play;

void OSUDODGE::genAccTimings(Play* _play)
{
	accTimings.clear();
	std::vector<osu::TIMING>().swap(accTimings);

	play = _play;
}