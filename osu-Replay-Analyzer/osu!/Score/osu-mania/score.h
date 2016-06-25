#ifndef MANIA_SCORE_H
#define MANIA_SCORE_H

#include <vector>

#include "../../osu_mania.h"
#include "../../Filestructure/Play.h"

namespace OSUMANIA
{
	extern std::vector<OSUMANIA::TIMING> accTimings;

	void genAccTimings(Play* _play);

	std::tuple<long, int, int> getNextEvent(int* _iFrame);
	Hitobject* getNextNoteOnColumn(int _column, int* _iNote);

	std::pair<double, double> getODms(Hitobject* _prevNote, Hitobject* _currNote, Hitobject* _nextNote, bool _press);
};

#endif // !MANIA_SCORE_H