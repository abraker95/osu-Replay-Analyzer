#include "score.h"

std::vector<osu::TIMING> OSUTAIKO::accTimings;
static Play* play;

void OSUTAIKO::genAccTimings(Play* _play)
{
	play = _play;
}