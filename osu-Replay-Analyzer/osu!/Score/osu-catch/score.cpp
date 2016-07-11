#include "score.h"

std::vector<OSUCATCH::TIMING> OSUCATCH::accTimings;
static Play* play;

void OSUCATCH::genAccTimings(Play* _play)
{
	accTimings.clear();
	std::vector<OSUCATCH::TIMING>().swap(accTimings);

	play = _play;
}