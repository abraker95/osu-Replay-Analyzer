#ifndef OSUMANIA_SPEEDCONTROL_H
#define OSUMANIA_SPEEDCONTROL_H

#include "../../osu_mania.h"
#include "../../Filestructure/Play.h"
#include "../../../utils/mathUtils.h"


namespace OSUMANIA
{
	namespace SPEED_CONTROL
	{
		extern std::vector<OSUMANIA::TIMING> scores, skills, diffs;

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

		namespace
		{
			double PressStrain(double _ms)
			{
				return exp((2.0*_ms) / 7.0) - 1.0;
			}
		}
	};
};

#endif // !OSUMANIA_STAMINA_H


