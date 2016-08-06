#ifndef STANDARD_SCORE_H
#define STANDARD_SCORE_H

#include <vector>

#include "../../osu_standard.h"
#include "../../Filestructure/Play.h"
#include "../../osuStructs.h"

namespace OSUSTANDARD
{
	extern std::vector<osu::TIMING> accTimings;

	void genAccTimings(Play* _play);
	bool sortAccTimings(osu::TIMING i, osu::TIMING j);

	std::tuple<long, int, int> getNextEvent(int* _iFrame);
	Hitobject* getNextNote(int* _iNote);

	namespace SCORE
	{
		struct KeyInfo
		{
			double hitTiming;
			int key;
			std::vector<bool>* pressState;
			bool* nextNote;
		};

		int getJudgment(int _frameTime, int _noteTime, bool _pressState);

		void processHit(std::vector<osu::TIMING>* _accTimings, Hitobject* _currNote, KeyInfo _info);
		void processMiss(std::vector<osu::TIMING>* _accTimings, Hitobject* _currNote, KeyInfo _info);
		void HandleEarlyMiss(std::vector<osu::TIMING>* _accTimings, Hitobject* _currNote, KeyInfo _info);
	};
};

#endif