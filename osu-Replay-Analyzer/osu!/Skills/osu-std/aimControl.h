#ifndef OSUSTANDARD_AIMCONTROL_H
#define OSUSTANDARD_AIMCONTROL_H

#include "../../osu_standard.h"
#include "../../Filestructure/Play.h"
#include "../../../utils/mathUtils.h"


namespace OSUSTANDARD
{
	extern std::vector<OSUSTANDARD::TIMING> aimControlDiffs, aimControlStrains;

	namespace AIM_CONTROL
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