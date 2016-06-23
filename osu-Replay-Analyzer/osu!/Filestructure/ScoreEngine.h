#ifndef SCORENGINE_H
#define SCORENGINE_H

#include <vector>

#include "Play.h"

class ScoreEngine
{
	public:
		ScoreEngine(Play* _play);

		std::pair<double, double> getTotalScore();
		double getTotalAccScore();

		struct TIMING
		{
			long time;
			double timingDiff;
			int key;
			bool press;
		};

		std::vector<TIMING> accTimings;
		std::vector<TIMING> diffPlyScores, diffMaxScores;
		std::vector<TIMING> tappingDiffs, tappingStrains;

	private:
		void genScores();
		void genAccTimings();

		// Difficulty calc
		void genPlyTappingDiffs();
		void genMaxTappingDiffs();
		void genMaxTappingStains();
		
		std::vector<double> TotalScore;

		double Time2AccScore(int _ms);
		double PressStrain(int _ms);

		std::pair<double, double> getODms(Hitobject* _prevNote, Hitobject* _currNote, Hitobject* _nextNote, bool _press);
		std::tuple<long, int, int> getNextEvent(int* _iFrame);

		Hitobject* getNextNoteOnColumn(int _column, int* _iNote);
		Play* play;
};

#endif