#ifndef SCORENGINE_H
#define SCORENGINE_H

#include <vector>

#include "../Filestructure/Play.h"
#include "../osu_mania.h"

class ScoreEngine
{
	public:
		ScoreEngine(Play* _play);

		//std::tuple<double, double, double> getTotalScore();
		//double getTotalAccScore();

		/*struct TIMING
		{
			long time;
			double data;
			int key;
			bool press;
		};*/

		//std::vector<TIMING> diffPlyScores, diffMaxScores;
		//std::vector<TIMING> tappingDiffs, tappingStrains;

	private:
		void genAccTimings(Play* _play);

		// Difficulty calc
		//void genPlyTappingDiffs();
		//void genMaxTappingDiffs();
		//void genMaxTappingStains();
		
		//std::vector<double> TotalScore;

		//double Time2AccScore(int _ms);
		//double PressStrain(int _ms);

		//Play* play;
};

#endif