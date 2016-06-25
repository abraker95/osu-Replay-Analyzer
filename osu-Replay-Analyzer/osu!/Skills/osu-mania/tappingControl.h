#ifndef OSUMANIA_TAPPINGCONTROL_H
#define OSUMANIA_TAPPINGCONTROL_H

#include "../../osu_mania.h"
#include "../../Filestructure/Play.h"
#include "../../../utils/mathUtils.h"


namespace OSUMANIA
{
	extern std::vector<OSUMANIA::TIMING> tappingDiffs, tappingStrains;

	namespace TAPPING_CONTROL
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