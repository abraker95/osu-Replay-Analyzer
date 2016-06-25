#include "SkillEngine.h"

#include "osuStandard_Skills.h"
#include "osuCatch_Skills.h"
#include "osuTaiko_Skills.h"
#include "osuMania_Skills.h"
#include "osuDodge_Skills.h"

SkillEngine::SkillEngine(Play* _play)
{
	gamemode = _play->beatmap->getGamemode();
}

double SkillEngine::getSkill(SKILL _skill)
{
	switch (gamemode)
	{
		case GAMEMODE::OSU_STANDARD:
			return OSUSTANDARD::SKILLS::getSkill(_skill);
			break;

		case GAMEMODE::OSU_TAIKO:
			return OSUTAIKO::SKILLS::getSkill(_skill);
			break;

		case GAMEMODE::OSU_CATCH:
			return OSUCATCH::SKILLS::getSkill(_skill);
			break;

		case GAMEMODE::OSU_MANIA:
			return OSUMANIA::SKILLS::getSkill(_skill);
			break;

		case GAMEMODE::OSU_DODGE:
			return OSUDODGE::SKILLS::getSkill(_skill);
			break;

		default:
			break;
	};
}

double SkillEngine::getDiff(SKILL _skill)
{
	switch (gamemode)
	{
		case GAMEMODE::OSU_STANDARD:
			return OSUSTANDARD::SKILLS::getDiff(_skill);
			break;

		case GAMEMODE::OSU_TAIKO:
			return OSUTAIKO::SKILLS::getDiff(_skill);
			break;

		case GAMEMODE::OSU_CATCH:
			return OSUCATCH::SKILLS::getDiff(_skill);
			break;

		case GAMEMODE::OSU_MANIA:
			return OSUMANIA::SKILLS::getDiff(_skill);
			break;

		case GAMEMODE::OSU_DODGE:
			return OSUDODGE::SKILLS::getDiff(_skill);
			break;

		default:
			break;
	};
}

double SkillEngine::getSkillAt(SKILL _skill, int _time)
{
	switch (gamemode)
	{
		case GAMEMODE::OSU_STANDARD:
			return OSUSTANDARD::SKILLS::getSkillAt(_skill, _time);
			break;

		case GAMEMODE::OSU_TAIKO:
			return OSUTAIKO::SKILLS::getSkillAt(_skill, _time);
			break;

		case GAMEMODE::OSU_CATCH:
			return OSUCATCH::SKILLS::getSkillAt(_skill, _time);
			break;

		case GAMEMODE::OSU_MANIA:
			return OSUMANIA::SKILLS::getSkillAt(_skill, _time);
			break;

		case GAMEMODE::OSU_DODGE:
			return OSUDODGE::SKILLS::getSkillAt(_skill, _time);
			break;

		default:
			break;
	};
}

double SkillEngine::getDiffAt(SKILL _skill, int _time)
{
	switch (gamemode)
	{
		case GAMEMODE::OSU_STANDARD:
			return OSUSTANDARD::SKILLS::getDiffAt(_skill, _time);
			break;

		case GAMEMODE::OSU_TAIKO:
			return OSUTAIKO::SKILLS::getDiffAt(_skill, _time);
			break;

		case GAMEMODE::OSU_CATCH:
			return OSUCATCH::SKILLS::getDiffAt(_skill, _time);
			break;

		case GAMEMODE::OSU_MANIA:
			return OSUMANIA::SKILLS::getDiffAt(_skill, _time);
			break;

		case GAMEMODE::OSU_DODGE:
			return OSUDODGE::SKILLS::getDiffAt(_skill, _time);
			break;

		default:
			break;
	};
}

double SkillEngine::getTotalSkillAt(SKILL _skill, int _time)
{
	switch (gamemode)
	{
		case GAMEMODE::OSU_STANDARD:
			return OSUSTANDARD::SKILLS::getTotalSkillAt(_skill, _time);
			break;

		case GAMEMODE::OSU_TAIKO:
			return OSUTAIKO::SKILLS::getTotalSkillAt(_skill, _time);
			break;

		case GAMEMODE::OSU_CATCH:
			return OSUCATCH::SKILLS::getTotalSkillAt(_skill, _time);
			break;

		case GAMEMODE::OSU_MANIA:
			return OSUMANIA::SKILLS::getTotalSkillAt(_skill, _time);
			break;

		case GAMEMODE::OSU_DODGE:
			return OSUDODGE::SKILLS::getTotalSkillAt(_skill, _time);
			break;

		default:
			break;
	};
}

double SkillEngine::getTotalDiffAt(SKILL _skill, int _time)
{
	switch (gamemode)
	{
		case GAMEMODE::OSU_STANDARD:
			return OSUSTANDARD::SKILLS::getTotalDiffAt(_skill, _time);
			break;

		case GAMEMODE::OSU_TAIKO:
			return OSUTAIKO::SKILLS::getTotalDiffAt(_skill, _time);
			break;

		case GAMEMODE::OSU_CATCH:
			return OSUCATCH::SKILLS::getTotalDiffAt(_skill, _time);
			break;

		case GAMEMODE::OSU_MANIA:
			return OSUMANIA::SKILLS::getTotalDiffAt(_skill, _time);
			break;

		case GAMEMODE::OSU_DODGE:
			return OSUDODGE::SKILLS::getTotalDiffAt(_skill, _time);
			break;

		default:
			break;
	};
}


// ------------ [PRIVATE] -------------


void SkillEngine::genSkills(Play* _play)
{

}

void SkillEngine::genDiffs(Play* _play)
{

}