#ifndef OSUMANIA_SKILLS_H
#define OSUMANIA_SKILLS_H

#include "SkillEngine.h"

#include "osu-mania/tappingControl.h"
#include "osu-mania/speedControl.h"
#include "osu-mania/reading.h"
#include "osu-mania/fingerBalance.h"
#include "osu-mania/handBalance.h"
#include "osu-mania/accuracy.h"

namespace OSUMANIA
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


		double Time2AccScore(int _ms);
	};
}

#endif // !OSUMANIA_SKILLS_H
