#ifndef SKILLENGINE_H
#define SKILLENGINE_H

#include "../Filestructure/Play.h"

class SkillEngine
{
	public:
		enum SKILL
		{
			ANY_TAPPINGCONTROL,	   // all gamemodes
			ANY_SPEEDCONTROL,	   // all gamemodes
			ANY_READING,	       // all gamemodes
			
			MANIA_FINGERBALANCE,   // mania
			MANIA_HANDBALANCE,     // mania
			MANIA_ACCURACY,	       // mania

			STD_MOVEMENTCONTROL,   // standard
			STD_AIMCONTROL,		   // standard
			STD_PRECISIONCONTROL,  // standard

			CTB_PRECISION		   // catch
		};


		SkillEngine(Play* _play);

		double getSkill(SKILL _skill);
		double getDiff(SKILL _skill);

		double getSkillAt(SKILL _skill, int _time);
		double getDiffAt(SKILL _skill, int _time);

		double getTotalSkillAt(SKILL _skill, int _time);
		double getTotalDiffAt(SKILL _skill, int _time);

	private:
		GAMEMODE gamemode;

		void genSkills(Play* _play);
		void genDiffs(Play* _play);
};

#endif // !SKILLENGINE_H