#ifndef OSUCATCH_SPEEDCONTROL_H
#define OSUCATCH_SPEEDCONTROL_H

#include "../../osu_catch.h"
#include "../../Filestructure/Play.h"
#include "../../../utils/mathUtils.h"


namespace OSUCATCH
{
	extern std::vector<OSUCATCH::TIMING> speedControlDiffs, speedControlStrains;

	namespace SPEED_CONTROL
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
