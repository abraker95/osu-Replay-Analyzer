#ifndef OSUMANIA_SPEEDCONTROL_H
#define OSUMANIA_SPEEDCONTROL_H

#include "../../osu_mania.h"
#include "../../Filestructure/Play.h"
#include "../../../utils/mathUtils.h"


namespace OSUMANIA
{
	extern std::vector<OSUMANIA::TIMING> speedDiffs, speedStrains;

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

#endif // !OSUMANIA_STAMINA_H


