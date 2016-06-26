#ifndef OSUCATCH_SKILLS_H
#define OSUCATCH_SKILLS_H

#include "SkillEngine.h"

#include "osu-catch/tappingControl.h"
#include "osu-catch/speedControl.h"
#include "osu-catch/reading.h"
#include "osu-catch/precision.h"

namespace OSUCATCH
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
