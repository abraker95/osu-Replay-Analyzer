#ifndef SKILLENGINE_H
#define SKILLENGINE_H

#include "../Filestructure/Play.h"

/*
	Score - How well the player did out of the difficulty the map has to offer. Accuracy is scaled logrithmically

	Skill - The difficulty of a player's play

	Diff  - The difficulty of the map's skill. All skills are finite except accuracy, 
			which is infinite or at least very high in shorter due the impossibility of hitting 0ms for every note
*/


class SkillEngine
{
	public:
		enum SKILL
		{
			ANY_TAPPINGCONTROL,	   // all gamemodes	|	Ability to have control over abnormal tapping times.
			ANY_SPEEDCONTROL,	   // all gamemodes |   Ability to have control over fast tapping times.
			ANY_READING,	       // all gamemodes |   Ability to read the map.
			
			MANIA_FINGERBALANCE,   // mania         |   Ability to use each finger independently
			MANIA_HANDBALANCE,     // mania         |	Ability to use each hand independently
			MANIA_ACCURACY,	       // mania         |	Ability to time finger tapping

			STD_MOVEMENTCONTROL,   // standard      |	Ability to control the degree of cursor's movement
			STD_AIMCONTROL,		   // standard      |	Ability to control the cursor's non-imidiate location
			STD_PRECISIONCONTROL,  // standard      |	Ability to control the cursor's imidiate location

			CTB_PRECISION,		   // catch         |	Ability to time movement
				 
			TAIKO_ACCURACY	       // taiko         |	Ability to time finger tapping
		};


		SkillEngine(Play* _play);
		void Generate(Play* _play);

		double getScore(SKILL _skill);
		double getSkill(SKILL _skill);
		double getDiff(SKILL _skill);

		double getScoreAt(SKILL _skill, int _time);
		double getSkillAt(SKILL _skill, int _time);
		double getDiffAt(SKILL _skill, int _time);

		double getTotalScoreAt(SKILL _skill, int _time);
		double getTotalSkillAt(SKILL _skill, int _time);
		double getTotalDiffAt(SKILL _skill, int _time);

	private:
		GAMEMODE gamemode;

		void genScores(Play* _play);
		void genSkills(Play* _play);
		void genDiffs(Play* _play);
};

#endif // !SKILLENGINE_H