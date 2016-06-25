#include "score.h"

std::vector<OSUSTANDARD::TIMING> OSUSTANDARD::accTimings;
static Play* play;

void OSUSTANDARD::genAccTimings(Play* _play)
{
	play = _play;
}