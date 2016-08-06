#ifndef OSUTAIKO_READINGL_H
#define OSUTAIKO_READING_H

#include "../../osu_taiko.h"
#include "../../Filestructure/Play.h"
#include "../../../utils/mathUtils.h"


namespace OSUTAIKO
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

#endif