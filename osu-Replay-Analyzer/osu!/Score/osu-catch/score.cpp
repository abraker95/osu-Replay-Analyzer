#include "score.h"

std::vector<OSUCATCH::TIMING> OSUCATCH::accTimings;
static Play* play;

void OSUCATCH::genAccTimings(Play* _play)
{
	play = _play;
}