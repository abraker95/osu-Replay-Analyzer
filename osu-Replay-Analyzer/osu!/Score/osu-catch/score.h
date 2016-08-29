#ifndef CATCH_SCORE_H
#define CATCH_SCORE_H

#include <vector>

#include "../../osu_catch.h"
#include "../../Filestructure/Play.h"
#include "../../osuStructs.h"

namespace OSUCATCH
{
	extern std::vector<osu::TIMING> accTimings;

	void genAccTimings(Play* _play);

	std::tuple<long, int, int> getNextEvent(int* _iFrame);

	std::pair<double, double> getODms(Hitobject* _prevNote, Hitobject* _currNote, Hitobject* _nextNote, bool _press);
};

#endif