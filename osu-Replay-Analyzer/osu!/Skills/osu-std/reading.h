#ifndef OSUSTANDARD_READING_H
#define OSUSTANDARD_READING_H

#include "../../osu_standard.h"
#include "../../Filestructure/Play.h"
#include "../../../utils/mathUtils.h"


namespace OSUSTANDARD
{
	extern std::vector<OSUSTANDARD::TIMING> readingDiffs, readingStrains;

	namespace READING
	{
		double genSkill(Play* _play);
		double genDiff(Play* _play);

		double getSkill();
		double getDiff();

		double getSkillAt(int _time);

		double getDiffAt(int _time);

		double getTotalSkillAt(int _time);
		double getTotalDiffAt(int _time);
	};
};

#endif