#ifndef OSUTAIKO_SKILLS_H
#define OSUTAIKO_SKILLS_H

#include "SkillEngine.h"

#include "osu-taiko/tappingControl.h"
#include "osu-taiko/speedControl.h"
#include "osu-taiko/reading.h"

namespace OSUTAIKO
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
