#include "score.h"

std::vector<OSUDODGE::TIMING> OSUDODGE::accTimings;
static Play* play;

void OSUDODGE::genAccTimings(Play* _play)
{
	play = _play;
}