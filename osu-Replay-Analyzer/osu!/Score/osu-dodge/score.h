#ifndef DODGE_SCORE_H
#define DODGE_SCORE_H

#include <vector>

#include "../../osu_dodge.h"
#include "../../Filestructure/Play.h"

namespace OSUDODGE
{
	extern std::vector<OSUDODGE::TIMING> accTimings;

	void genAccTimings(Play* _play);

	std::tuple<long, int, int> getNextEvent(int* _iFrame);
	Hitobject* getNextNoteOnColumn(int _column, int* _iNote);

	std::pair<double, double> getODms(Hitobject* _prevNote, Hitobject* _currNote, Hitobject* _nextNote, bool _press);
};

#endif