#include "reading.h"

#include "../SkillEngine.h"
#include "../../osu_standard.h"
#include "../../osuStructs.h"
#include "../../osuCalc.h"

std::vector<osu::TIMING> OSUSTANDARD::READING::scores,
						 OSUSTANDARD::READING::skills,
						 OSUSTANDARD::READING::diffs;
std::vector<osu::TIMING> OSUSTANDARD::READING::velocities,
						 OSUSTANDARD::READING::notesRate,
						 OSUSTANDARD::READING::overlaps;
double OSUSTANDARD::READING::diff;

void OSUSTANDARD::READING::genScore(Play* _play)
{
	scores.clear();
	std::vector<osu::TIMING>().swap(scores);

	/// \TODO
}

void OSUSTANDARD::READING::genSkill(Play* _play)
{
	skills.clear();
	std::vector<osu::TIMING>().swap(skills);

	/// \TODO
}

void OSUSTANDARD::READING::genDiff(Play* _play)
{
	/// \TODO
	diff = -1;
	diffs.clear();
	std::vector<osu::TIMING>().swap(diffs);

}



double OSUSTANDARD::READING::getScore()
{
	return -1; /// \TODO
}

double OSUSTANDARD::READING::getSkill()
{
	return -1; /// \TODO
}

double OSUSTANDARD::READING::getDiff()
{
	return -1; /// \TODO
}



double OSUSTANDARD::READING::getScoreAt(int _time)
{
	return -1; /// \TODO
}

double OSUSTANDARD::READING::getSkillAt(int _time)
{
	return -1; /// \TODO
}

double OSUSTANDARD::READING::getDiffAt(int _time)
{
	return -1; /// \TODO
}



double OSUSTANDARD::READING::getTotalScoreAt(int _time)
{
	return -1; /// \TODO
}

double OSUSTANDARD::READING::getTotalSkillAt(int _time)
{
	return -1; /// \TODO
}

double OSUSTANDARD::READING::getTotalDiffAt(int _time)
{
	return -1; /// \TODO
}
