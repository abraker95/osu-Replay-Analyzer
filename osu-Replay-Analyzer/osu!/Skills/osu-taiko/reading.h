#ifndef OSUTAIKO_READINGL_H
#define OSUTAIKO_READING_H

#include "../../osu_taiko.h"
#include "../../Filestructure/Play.h"
#include "../../../utils/mathUtils.h"


namespace OSUTAIKO
{
	extern std::vector<OSUTAIKO::TIMING> readingDiffs, readingStrains;

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