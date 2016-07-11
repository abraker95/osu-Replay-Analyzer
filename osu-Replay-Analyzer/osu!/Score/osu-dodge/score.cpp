#include "score.h"

std::vector<OSUDODGE::TIMING> OSUDODGE::accTimings;
static Play* play;

void OSUDODGE::genAccTimings(Play* _play)
{
	accTimings.clear();
	std::vector<OSUDODGE::TIMING>().swap(accTimings);

	play = _play;
}