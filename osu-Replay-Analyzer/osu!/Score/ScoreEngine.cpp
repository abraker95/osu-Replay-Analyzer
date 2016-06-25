#include "ScoreEngine.h"

#include "osu-std/score.h"
#include "osu-catch/score.h"
#include "osu-taiko/score.h"
#include "osu-mania/score.h"
#include "osu-dodge/score.h"

#include <math.h>

ScoreEngine::ScoreEngine(Play* _play)
{
	play = _play;

	if((play->beatmap != nullptr) && (play->replay != nullptr))
		genScores();
}

double ScoreEngine::getTotalAccScore()
{
	return 0; /// \TODO
}

std::tuple<double, double, double> ScoreEngine::getTotalScore()
{
	double score = 0.0;
	double maxScore = 0.0;
	double missesPunisher = 1.0;

	// max score calc
	for (int i = 1; i < tappingStrains.size(); i++)
	{
		maxScore += tappingStrains[i].data * log(i);
	}

	// score calc
	for (int i = 0; i < OSUMANIA::accTimings.size(); i++)
	{
		if (diffPlyScores[i].data != -1) // if not missed
		{
			score += (Time2AccScore(OSUMANIA::accTimings[i].data) * tappingStrains[i].data * log(i)) / missesPunisher;

			if (missesPunisher > 1.0)
				missesPunisher /= 2.0;
			else
				missesPunisher = 1.0;
		}
		else missesPunisher *= 2.0;
	}

	std::tuple<double, double, double> totalScore;
		std::get<0>(totalScore) = (score / maxScore) * 1000000.0;				// score
		std::get<1>(totalScore) = score / OSUMANIA::accTimings.size() * 20.0;				// performance
		std::get<2>(totalScore) = maxScore / tappingStrains.size() * 20.0;		// max performance
		
	return totalScore;
}

void ScoreEngine::genScores()
{
	GAMEMODE gamemode = play->beatmap->getGamemode();

	switch (gamemode)
	{
		case OSU_STANDARD:
			OSUSTANDARD::genAccTimings(play);
			break;
	
		case OSU_TAIKO:
			OSUTAIKO::genAccTimings(play);
			break;
		
		case OSU_CATCH:
			OSUCATCH::genAccTimings(play);
			break;
	
		case OSU_MANIA:
			OSUMANIA::genAccTimings(play);
			break;
	
		case OSU_DODGE:
			OSUDODGE::genAccTimings(play);
			break;
	
		default:
			break;
	}

	genMaxTappingDiffs();
	genMaxTappingStains();

	genPlyTappingDiffs();
}

void ScoreEngine::genMaxTappingDiffs()
{
	int KEYS = play->beatmap->getDiff().cs;
	std::vector<TIMING> diffScore;
		diffScore.resize(KEYS);
	std::vector<int> hitPeriodPrev, hitPeriodCurr;
		hitPeriodPrev.resize(KEYS);
		hitPeriodCurr.resize(KEYS);


	for (int key = 0; key < KEYS; key++)
	{
		diffScore[key].key = key;
		diffScore[key].data = 0;
	}

	for (int i = 0; i < play->beatmap->hitObjects.size(); i++)
	{
		// never a miss
		if (true)
		{
			bool isHoldObject = !(play->beatmap->hitObjects[i]->getHitobjectType() & HITOBJECTYPE::CIRCLE);
			int noteXpos = play->beatmap->hitObjects[i]->getPos().X;
			int column = OSUMANIA::getKey(noteXpos, KEYS);

			//shift: prev <- curr, curr <- next
			hitPeriodPrev[column] = hitPeriodCurr[column];
			hitPeriodCurr[column] = play->beatmap->hitObjects[i]->getTime();

			double period = (hitPeriodCurr[column] - hitPeriodPrev[column]);

			// if previous was a slider, half the press effect
			if (diffScore[column].press == false)
				period *= 2.0;

			// keys per second
			double kps = 1000.0 / period;

			// first note is nothing
			if (hitPeriodPrev[column] == 0)
				kps = 0.0;

			diffScore[column].data		 = PressStrain(kps);
			diffScore[column].time		 = hitPeriodCurr[column];
			diffScore[column].key		 = column;
			diffScore[column].press		 = true;

			tappingDiffs.push_back(diffScore[column]);

			// if it's a slider, then another frame for release
			if (isHoldObject)
			{
				hitPeriodPrev[column] = hitPeriodCurr[column];
				hitPeriodCurr[column] = play->beatmap->hitObjects[i]->getEndTime();

				double period = (hitPeriodCurr[column] - hitPeriodPrev[column]);
				double kps = 1000.0 / period; // keys per second

				diffScore[column].data		 = PressStrain(kps) * 0.1;
				diffScore[column].time	     = hitPeriodCurr[column];
				diffScore[column].key		 = column;
				diffScore[column].press		 = false;

				tappingDiffs.push_back(diffScore[column]);
			}
		}
	}
}

void ScoreEngine::genMaxTappingStains()
{
	int KEYS = play->beatmap->getDiff().cs;
	std::vector<TIMING> tappingStrain;
		tappingStrain.resize(KEYS);

	for (int key = 0; key < KEYS; key++)
	{
		tappingStrain[key].key = key;
		tappingStrain[key].data = 0;
	}

	for (TIMING &tappingDiff : tappingDiffs)
	{
		int key = tappingDiff.key;

		tappingStrain[key].data = 0.5*tappingStrain[key].data + 0.5*tappingDiff.data;
		tappingStrain[key].time = tappingDiff.time;
		tappingStrain[key].key = key;
		tappingStrain[key].press = tappingDiff.press;

		tappingStrains.push_back(tappingStrain[key]);
	}
}

void ScoreEngine::genPlyTappingDiffs()
{
	int KEYS = play->beatmap->getDiff().cs;
	TIMING* diffScore = new TIMING[KEYS];
	
	for (int key = 0; key < KEYS; key++)
	{
		diffScore[key].key = key;
		diffScore[key].data = 0;
	}

	// first note is nothing
	diffPlyScores.push_back(TIMING{ OSUMANIA::accTimings[0].time, -1, -1, false });

	for (int i = 1; i < OSUMANIA::accTimings.size(); i++)
	{
		// if it's not a miss
		if (OSUMANIA::accTimings[i].data != INT_MAX)
		{
			for (int key = 0; key < KEYS; key++)
			{
				// process column hit difficulty based on last time hit
				if ((OSUMANIA::accTimings[i].key == key))
				{
					double hitPeriodPrev = diffScore[key].time;
					double hitPeriodCurr = OSUMANIA::accTimings[i].time + OSUMANIA::accTimings[i].data;
					double kps = 1000.0 / (hitPeriodCurr - hitPeriodPrev); // keys per second

					if(OSUMANIA::accTimings[i].press == true) // if pressed
						diffScore[key].data = PressStrain(kps);
					else // released
						diffScore[key].data = PressStrain(kps) * 0.1;

					diffScore[key].time = hitPeriodCurr;
				}
			}

			// average out the overall diff of pressing this
			double frameDiff = 0.0;
			for (int key = 0; key < KEYS; key++)
				frameDiff += diffScore[key].data;
			frameDiff /= KEYS;

			diffPlyScores.push_back((TIMING{ OSUMANIA::accTimings[i].time, frameDiff, -1, false }));
		}
		else // miss
		{
			// place holder so diffScores and accTimings match up
			diffPlyScores.push_back(TIMING{ OSUMANIA::accTimings[i].time, -1, -1, false });
		}
	}

	delete diffScore;
}

// modeled here: https://www.desmos.com/calculator/ok1qgmfe9f
double ScoreEngine::Time2AccScore(int _ms)
{
	double scale = 0.5; // 0.5 @ 60 FPS
	double o = sqrt(-pow((50.0 / 3.0), 2.0) / (2.0 * log(scale)));  

	return exp(-pow(_ms, 2.0) / (2.0 * pow(o, 2.0)));
}

double ScoreEngine::PressStrain(int _ms)
{
	return exp((2.0*_ms) / 7.0) - 1.0;
}