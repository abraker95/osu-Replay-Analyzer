#ifndef OSUMANIA_READING_H
#define OSUMANIA_READING_H

#include "../../osu_mania.h"
#include "../../Filestructure/Play.h"
#include "../../../utils/mathUtils.h"


namespace OSUMANIA
{
	namespace READING
	{
		extern std::vector<osu::TIMING> scores, skills, diffs;

		void genScore(Play* _play);
		void genSkill(Play* _play);
		void genDiff(Play* _play);

		double getScore();
		double getSkill();
		double getDiff();

		double getScoreAt(int _time);
		double getSkillAt(int _time);
		double getDiffAt(int _time);

		double getTotalScoreAt(int _time);
		double getTotalSkillAt(int _time);
		double getTotalDiffAt(int _time);
	};
};

#endif // READING_H