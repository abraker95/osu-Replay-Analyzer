#ifndef SCORENGINE_H
#define SCORENGINE_H

#include <vector>

#include "Play.h"

class ScoreEngine
{
	public:
		ScoreEngine(Play* _play);

		double getTotalScore();
		double getTotalAccScore();

		struct TIMING
		{
			long time;
			double timingDiff;
			int key;
			bool press;
		};

		std::vector<TIMING> accTimings;
		std::vector<TIMING> diffScores;

	private:
		void genScores();
		void genAccTimings();
		void genDifficulties();

		double getAccScoreFor(int _time);
		double getDiffScoreFor(int _time);
		double getScoreFor(int _time);

		
		std::vector<double> TotalScore;

		double Time2AccScore(int _ms);
		double PressStrain(int _ms);

		std::pair<double, double> getODms(Hitobject* _prevNote, Hitobject* _currNote, Hitobject* _nextNote, bool _press);
		std::tuple<long, int, int> getNextEvent(int* _iFrame);

		Hitobject* getNextNoteOnColumn(int _column, int* _iNote);
		Play* play;
};

#endif