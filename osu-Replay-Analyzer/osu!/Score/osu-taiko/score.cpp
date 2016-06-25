#include "score.h"

std::vector<OSUTAIKO::TIMING> OSUTAIKO::accTimings;
static Play* play;

void OSUTAIKO::genAccTimings(Play* _play)
{
	play = _play;
}