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
	std::vector<double> hitPeriodPrev, hitPeriodCurr;
		hitPeriodPrev.resize(KEYS);
		hitPeriodCurr.resize(KEYS);

	for (int key = 0; key < KEYS; key++)
	{
		timing[key].key = key;  // this doesn't need to be set again
		timing[key].data = 0.0;
	}

	for (OSUMANIA::TIMING &accTiming : OSUMANIA::accTimings)
	{
		// if it's not a miss
		if (accTiming.data != INT_MAX)
		{
			bool isHoldObject = !accTiming.press;
			int column = accTiming.key;

			//shift: prev <- curr, curr <- next
			hitPeriodPrev[column] = timing[column].time;
			hitPeriodCurr[column] = accTiming.time + accTiming.data;

			// if pressing down
			if (!isHoldObject)
			{
				double period = (hitPeriodCurr[column] - hitPeriodPrev[column]);

				// if previous was a slider, halve the press effect. Same applies if previous was miss.
				if (timing[column].press == false)
					period *= 4.0;

				// keys per second
				double kps = 1000.0 / period;
				
				// first note is nothing
				if (hitPeriodPrev[column] == 0)
					kps = 0.0;

				timing[column].data  = PressStrain(kps);
				timing[column].time  = hitPeriodCurr[column];
				timing[column].press = true;					// this is a press

				skills.push_back(timing[column]);
			}
			else
			{
				timing[column].data  = 0.0;						// release has no strain on tapping
				timing[column].time  = hitPeriodCurr[column];
				timing[column].press = false;					// this is a release

				skills.push_back(timing[column]);
			}
		}
		else // miss
		{
			// place holder so diffScores and accTimings match up
			skills.push_back(OSUMANIA::TIMING{ accTiming.time, 0.0, -1, false });
		}
	}

	// The longer it lasts, the more effective it is
	for(int i = 1; i < skills.size(); i++)
		skills[i].data = 0.5*skills[i].data + 0.5*skills[i - 1].data;
}

void OSUMANIA::SPEED_CONTROL::genDiff(Play* _play)
{
	int KEYS = _play->beatmap->getDiff().cs;
	std::vector<OSUMANIA::TIMING> timing, releases;
		timing.resize(KEYS);
		releases.resize(KEYS);
	std::vector<double> hitPeriodPrev, hitPeriodCurr;
		hitPeriodPrev.resize(KEYS);
		hitPeriodCurr.resize(KEYS);

	for (int key = 0; key < KEYS; key++)
	{
		timing[key].key = key;  // this doesn't need to be set again
		timing[key].data = 0.0;
		releases[key].press = true; // no note releases yet
	}

	for (Hitobject *hitobject : _play->beatmap->hitObjects)
	{
		// never a miss
		if (true)
		{
			bool isHoldObject = !(hitobject->getHitobjectType() & HITOBJECTYPE::CIRCLE);
			int noteXpos = hitobject->getPos().X;
			int column = OSUMANIA::getKey(noteXpos, KEYS);

			//shift: prev <- curr, curr <- next
			hitPeriodPrev[column] = hitPeriodCurr[column];
			hitPeriodCurr[column] = hitobject->getTime();

			// check if we have a note release first
			for (OSUMANIA::TIMING &release : releases)
			{
				if (release.press == false)			// if there is something waiting
				{
					if (hitPeriodCurr[column] >= release.time)
					{
						diffs.push_back(release);
						release.press = true;			// set it to inactive
					}
				}
			}

			double period = (hitPeriodCurr[column] - hitPeriodPrev[column]);

			// if previous was a slider, halve the press effect
			if (timing[column].press == false)
				period *= 2.0;

			// keys per second
			double kps = 1000.0 / period;

			// first note is nothing
			if (hitPeriodPrev[column] == 0)
				kps = 0.0;

			timing[column].data  = PressStrain(kps);
			timing[column].time  = hitPeriodCurr[column];
			timing[column].press = true;						// this is a press

			diffs.push_back(timing[column]);
			
			// if it's a slider, then another frame for release
			if (isHoldObject)
			{
				//shift: prev <- curr, curr <- next
				timing[column].data  = 0.0;						// release has no strain on tapping
				timing[column].time  = hitobject->getEndTime();
				timing[column].press = false;					// this is a release

				releases[column] = timing[column];				// wait until we have reached the time
			}
		}
	}

	// push any remaining releases
	for (OSUMANIA::TIMING &release : releases)
	{
		if (release.press == false)			// if there is something waiting
		{
			diffs.push_back(release);
			release.press = true;			// set it to inactive
		}
	}

}



double OSUMANIA::SPEED_CONTROL::getScore()
{
	double score = 0.0;

	for (OSUMANIA::TIMING &timing : SPEED_CONTROL::scores)
	{
		score += timing.data;
	}

	return score;
}

double OSUMANIA::SPEED_CONTROL::getSkill()
{
	double skill = 0.0;

	for (OSUMANIA::TIMING &timing : SPEED_CONTROL::skills)
		skill += timing.data;

	return skill;
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