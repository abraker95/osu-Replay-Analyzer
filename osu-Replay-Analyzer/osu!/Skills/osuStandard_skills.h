#ifndef OSUSTANDARD_SKILLS_H
#define OSUSTANDARD_SKILLS_H

#include "SkillEngine.h"

#include "osu-std/tappingControl.h"
#include "osu-std/speedControl.h"
#include "osu-std/reading.h"
#include "osu-std/aimControl.h"
#include "osu-std/movementControl.h"
#include "osu-std/precisionControl.h"

namespace OSUSTANDARD
{
	namespace SKILLS
	{
		double getScore(SkillEngine::SKILL _skill);
		double getSkill(SkillEngine::SKILL _skill);
		double getDiff(SkillEngine::SKILL _skill);


		double getScoreAt(SkillEngine::SKILL _skill, int _time);
		double getSkillAt(SkillEngine::SKILL _skill, int _time);
		double getDiffAt(SkillEngine::SKILL _skill, int _time);


		double getTotalScoreAt(SkillEngine::SKILL _skill, int _time);
		double getTotalSkillAt(SkillEngine::SKILL _skill, int _time);
		double getTotalDiffAt(SkillEngine::SKILL _skill, int _time);
	};
}

#endif
