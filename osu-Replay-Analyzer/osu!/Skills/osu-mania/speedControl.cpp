#include "speedControl.h"

#include "../SkillEngine.h"

#include "../../osu_mania.h"
#include "../osuMania_skills.h"
#include "../../Score/osu-mania/score.h"


std::vector<OSUMANIA::TIMING> OSUMANIA::SPEED_CONTROL::scores, 
							  OSUMANIA::SPEED_CONTROL::skills,
							  OSUMANIA::SPEED_CONTROL::diffs;


void OSUMANIA::SPEED_CONTROL::genScore(Play* _play)
{
	double score = 0.0;
	//double maxScore = 0.0;
	double missesPunisher = 1.0;
	OSUMANIA::TIMING timing;


	/*// max score calc
	for (int i = 1; i < speedDiffs.size(); i++)
	{
		maxScore += speedDiffs[i].data * log(i);
	}*/

	// score calc
	for (int i = 0; i < OSUMANIA::accTimings.size(); i++)
	{
		if (skills[i].data != -1) // if not missed
		{
			timing.data  = (SKILLS::Time2AccScore(OSUMANIA::accTimings[i].data) * skills[i].data * log(i)) / missesPunisher;
			timing.key   = skills[i].key;
			timing.press = skills[i].press;
			timing.time  = skills[i].time;

			scores.push_back(timing);

			if (missesPunisher > 1.0)
				missesPunisher /= 2.0;
			else
				missesPunisher = 1.0;
		}
		else missesPunisher *= 2.0;
	}

//	std::tuple<double, double, double> totalScore;
//	std::get<0>(totalScore) = (score / maxScore) * 1000000.0;				// score
//	std::get<1>(totalScore) = score / OSUMANIA::accTimings.size() * 20.0;	// performance
//	std::get<2>(totalScore) = maxScore / speedDiffs.size() * 20.0;		// max performance
}

void OSUMANIA::SPEED_CONTROL::genSkill(Play* _play)
{
	int KEYS = _play->beatmap->getDiff().cs;
	std::vector<OSUMANIA::TIMING> timing;
		timing.resize(KEYS);

	for (int key = 0; key < KEYS; key++)
	{
		timing[key].key = key;
		timing[key].data = 0;
	}

	// first note is nothing
	skills.push_back(OSUMANIA::TIMING{ OSUMANIA::accTimings[0].time, -1, -1, false });

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
					double hitPeriodPrev = timing[key].time;
					double hitPeriodCurr = OSUMANIA::accTimings[i].time + OSUMANIA::accTimings[i].data;
					double kps = 1000.0 / (hitPeriodCurr - hitPeriodPrev); // keys per second

					if (OSUMANIA::accTimings[i].press == true) // if pressed
						timing[key].data = PressStrain(kps);
					else									   // if released
						timing[key].data = PressStrain(kps) * 0.1;

					timing[key].time = hitPeriodCurr;
				}
			}

			// average out the overall diff of pressing this
			double frameDiff = 0.0;
			for (int key = 0; key < KEYS; key++)
				frameDiff += timing[key].data;
			frameDiff /= KEYS;

			skills.push_back((OSUMANIA::TIMING{ OSUMANIA::accTimings[i].time, frameDiff, -1, false }));
		}
		else // miss
		{
			// place holder so diffScores and accTimings match up
			skills.push_back(OSUMANIA::TIMING{ OSUMANIA::accTimings[i].time, -1, -1, false });
		}
	}
}

void OSUMANIA::SPEED_CONTROL::genDiff(Play* _play)
{
	int KEYS = _play->beatmap->getDiff().cs;
	std::vector<OSUMANIA::TIMING> timing;
		timing.resize(KEYS);
	std::vector<int> hitPeriodPrev, hitPeriodCurr;
		hitPeriodPrev.resize(KEYS);
		hitPeriodCurr.resize(KEYS);

	for (int key = 0; key < KEYS; key++)
	{
		timing[key].key = key;
		timing[key].data = 0;
	}

	for (int i = 0; i < _play->beatmap->hitObjects.size(); i++)
	{
		// never a miss
		if (true)
		{
			bool isHoldObject = !(_play->beatmap->hitObjects[i]->getHitobjectType() & HITOBJECTYPE::CIRCLE);
			int noteXpos = _play->beatmap->hitObjects[i]->getPos().X;
			int column = OSUMANIA::getKey(noteXpos, KEYS);

			//shift: prev <- curr, curr <- next
			hitPeriodPrev[column] = hitPeriodCurr[column];
			hitPeriodCurr[column] = _play->beatmap->hitObjects[i]->getTime();

			double period = (hitPeriodCurr[column] - hitPeriodPrev[column]);

			// if previous was a slider, halve the press effect
			if (timing[column].press == false)
				period *= 2.0;

			// keys per second
			double kps = 1000.0 / period;

			// first note is nothing
			if (hitPeriodPrev[column] == 0)
				kps = 0.0;

			timing[column].data = PressStrain(kps);
			timing[column].time = hitPeriodCurr[column];
			timing[column].press = true;

			diffs.push_back(timing[column]);

			// if it's a slider, then another frame for release
			if (isHoldObject)
			{
				hitPeriodPrev[column] = hitPeriodCurr[column];
				hitPeriodCurr[column] = _play->beatmap->hitObjects[i]->getEndTime();

				double period = (hitPeriodCurr[column] - hitPeriodPrev[column]);
				double kps = 1000.0 / period; // keys per second

				timing[column].data = PressStrain(kps) * 0.1;
				timing[column].time = hitPeriodCurr[column];
				timing[column].key = column;
				timing[column].press = false;

				diffs.push_back(timing[column]);
			}
		}
	}
}



double OSUMANIA::SPEED_CONTROL::getScore()
{
	return -1; /// \TODO
}

double OSUMANIA::SPEED_CONTROL::getSkill()
{
	return -1; /// \TODO
}

double OSUMANIA::SPEED_CONTROL::getDiff()
{
	double diff = 0.0;

	for (OSUMANIA::TIMING &timing : SPEED_CONTROL::diffs)
		diff += timing.data;

	return diff;
}



double OSUMANIA::SPEED_CONTROL::getScoreAt(int _time)
{
	return -1; /// \TODO
}

double OSUMANIA::SPEED_CONTROL::getSkillAt(int _time)
{
	return -1; /// \TODO
}

double OSUMANIA::SPEED_CONTROL::getDiffAt(int _time)
{
	return -1; /// \TODO
}




double OSUMANIA::SPEED_CONTROL::getTotalScoreAt(int _time)
{
	return -1; /// \TODO
}

double OSUMANIA::SPEED_CONTROL::getTotalSkillAt(int _time)
{
	return -1; /// \TODO
}

double OSUMANIA::SPEED_CONTROL::getTotalDiffAt(int _time)
{
	return -1; /// \TODO
}