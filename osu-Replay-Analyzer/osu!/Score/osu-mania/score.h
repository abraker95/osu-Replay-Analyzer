#ifndef MANIA_SCORE_H
#define MANIA_SCORE_H

#include <vector>

#include "../../osu_mania.h"
#include "../../Filestructure/Play.h"
#include "../../osuStructs.h"

namespace OSUMANIA
{
	extern std::vector<osu::TIMING> accTimings;

	void genAccTimings(Play* _play);
	bool sortAccTimings(osu::TIMING i, osu::TIMING j);

	std::tuple<long, int, int> getNextEvent(int* _iFrame);
	Hitobject* getNextNoteOnColumn(int _column, int* _iNote);

	int getJudgment(int _frameTime, int _noteTime, bool _pressState);

	namespace SCORE
	{
		struct KeyInfo
		{
			double hitTiming;
			int key;
			std::vector<bool>* pressState;
			std::vector<bool>* nextNote;
		};

		int getJudgment(int _frameTime, int _noteTime, bool _pressState);
		std::pair<double, double> getODms(Hitobject* _prevNote, Hitobject* _currNote, Hitobject* _nextNote, bool _press);

		void processHit(std::vector<osu::TIMING>* _accTimings, Hitobject* _currNote, KeyInfo _info);
		void processMiss(std::vector<osu::TIMING>* _accTimings, Hitobject* _currNote, KeyInfo _info);
		void HandleEarlyMiss(std::vector<osu::TIMING>* _accTimings, Hitobject* _currNote, KeyInfo _info);
	};
};

#endif // !MANIA_SCORE_H