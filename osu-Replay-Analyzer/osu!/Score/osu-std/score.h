#ifndef STANDARD_SCORE_H
#define STANDARD_SCORE_H

#include <vector>

#include "../../osu_standard.h"
#include "../../Filestructure/Play.h"

namespace OSUSTANDARD
{
	extern std::vector<OSUSTANDARD::TIMING> accTimings;

	void genAccTimings(Play* _play);

	std::tuple<long, int, int> getNextEvent(int* _iFrame);
	Hitobject* getNextNote(int* _iNote);

	int getJudgment(int _frameTime, int _noteTime, bool _pressState);
};

#endif