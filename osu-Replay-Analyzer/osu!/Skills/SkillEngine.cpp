#include "SkillEngine.h"

#include "osuStandard_Skills.h"
#include "osuCatch_Skills.h"
#include "osuTaiko_Skills.h"
#include "osuMania_Skills.h"
#include "osuDodge_Skills.h"

SkillEngine::SkillEngine(Play* _play)
{
	Generate(_play);
}

void SkillEngine::Generate(Play* _play)
{
	gamemode = _play->beatmap->getGamemode();
		// dont mess up this order:  diffs -> skills -> scores
		genDiffs(_play);
		genSkills(_play);
		genScores(_play);

	genDiffs(_play);
}

double SkillEngine::getScore(SKILL _skill)
{
	switch (gamemode)
	{
		case GAMEMODE::OSU_STANDARD:
			return OSUSTANDARD::SKILLS::getScore(_skill);
			break;

		case GAMEMODE::OSU_TAIKO:
			return OSUTAIKO::SKILLS::getScore(_skill);
			break;

		case GAMEMODE::OSU_CATCH:
			return OSUCATCH::SKILLS::getScore(_skill);
			break;

		case GAMEMODE::OSU_MANIA:
			return OSUMANIA::SKILLS::getScore(_skill);
			break;

		case GAMEMODE::OSU_DODGE:
			return OSUDODGE::SKILLS::getScore(_skill);
			break;

		default:
			break;
	};
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



double SkillEngine::getScoreAt(SKILL _skill, int _time)
{
	switch (gamemode)
	{
		case GAMEMODE::OSU_STANDARD:
			return OSUSTANDARD::SKILLS::getScoreAt(_skill, _time);
			break;

		case GAMEMODE::OSU_TAIKO:
			return OSUTAIKO::SKILLS::getScoreAt(_skill, _time);
			break;

		case GAMEMODE::OSU_CATCH:
			return OSUCATCH::SKILLS::getScoreAt(_skill, _time);
			break;

		case GAMEMODE::OSU_MANIA:
			return OSUMANIA::SKILLS::getScoreAt(_skill, _time);
			break;

		case GAMEMODE::OSU_DODGE:
			return OSUDODGE::SKILLS::getScoreAt(_skill, _time);
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



double SkillEngine::getTotalScoreAt(SKILL _skill, int _time)
{
	switch (gamemode)
	{
		case GAMEMODE::OSU_STANDARD:
			return OSUSTANDARD::SKILLS::getTotalScoreAt(_skill, _time);
			break;

		case GAMEMODE::OSU_TAIKO:
			return OSUTAIKO::SKILLS::getTotalScoreAt(_skill, _time);
			break;

		case GAMEMODE::OSU_CATCH:
			return OSUCATCH::SKILLS::getTotalScoreAt(_skill, _time);
			break;

		case GAMEMODE::OSU_MANIA:
			return OSUMANIA::SKILLS::getTotalScoreAt(_skill, _time);
			break;

		case GAMEMODE::OSU_DODGE:
			return OSUDODGE::SKILLS::getTotalScoreAt(_skill, _time);
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


void SkillEngine::genScores(Play* _play)
{
	// need beatmap and replay for score generation
	if (!_play->beatmap->isValid())	return;
	if (!_play->replay->isValid())	return;

	switch (gamemode)
	{
		case GAMEMODE::OSU_STANDARD:
			OSUSTANDARD::SPEED_CONTROL::genScore(_play);
			OSUSTANDARD::TAPPING_CONTROL::genScore(_play);
			OSUSTANDARD::READING::genScore(_play);
			OSUSTANDARD::AIM_CONTROL::genScore(_play);
			OSUSTANDARD::MOVEMENT_CONTROL::genScore(_play);
			OSUSTANDARD::PRECISION_CONTROL::genScore(_play);
			break;

		case GAMEMODE::OSU_TAIKO:
			OSUTAIKO::SPEED_CONTROL::genScore(_play);
			OSUTAIKO::TAPPING_CONTROL::genScore(_play);
			OSUTAIKO::READING::genScore(_play);
			break;

		case GAMEMODE::OSU_CATCH:
			OSUCATCH::SPEED_CONTROL::genScore(_play);
			OSUCATCH::TAPPING_CONTROL::genScore(_play);
			OSUCATCH::READING::genScore(_play);
			OSUCATCH::PRECISION::genScore(_play);
			break;

		case GAMEMODE::OSU_MANIA:
			OSUMANIA::SPEED_CONTROL::genScore(_play);
			OSUMANIA::TAPPING_CONTROL::genScore(_play);
			OSUMANIA::READING::genScore(_play);
			OSUMANIA::ACCURACY::genScore(_play);
			OSUMANIA::FINGER_BALANCE::genScore(_play);
			OSUMANIA::HAND_BALANCE::genScore(_play);
			break;

		case GAMEMODE::OSU_DODGE:
			// yes?
			break;

		default:
			break;
	};
}

void SkillEngine::genSkills(Play* _play)
{
	// need beatmap and replay for score generation
	if (!_play->beatmap->isValid())	return;
	if (!_play->replay->isValid())	return;

	switch (gamemode)
	{
		case GAMEMODE::OSU_STANDARD:
			OSUSTANDARD::SPEED_CONTROL::genSkill(_play);
			OSUSTANDARD::TAPPING_CONTROL::genSkill(_play);
			OSUSTANDARD::READING::genSkill(_play);
			OSUSTANDARD::AIM_CONTROL::genSkill(_play);
			OSUSTANDARD::MOVEMENT_CONTROL::genSkill(_play);
			OSUSTANDARD::PRECISION_CONTROL::genSkill(_play);
			break;

		case GAMEMODE::OSU_TAIKO:
			OSUTAIKO::SPEED_CONTROL::genSkill(_play);
			OSUTAIKO::TAPPING_CONTROL::genSkill(_play);
			OSUTAIKO::READING::genSkill(_play);
			break;

		case GAMEMODE::OSU_CATCH:
			OSUCATCH::SPEED_CONTROL::genSkill(_play);
			OSUCATCH::TAPPING_CONTROL::genSkill(_play);
			OSUCATCH::READING::genSkill(_play);
			OSUCATCH::PRECISION::genSkill(_play);
			break;

		case GAMEMODE::OSU_MANIA:
			OSUMANIA::SPEED_CONTROL::genSkill(_play);
			OSUMANIA::TAPPING_CONTROL::genSkill(_play);
			OSUMANIA::READING::genSkill(_play);
			OSUMANIA::ACCURACY::genSkill(_play);
			OSUMANIA::FINGER_BALANCE::genSkill(_play);
			OSUMANIA::HAND_BALANCE::genSkill(_play);
			break;

		case GAMEMODE::OSU_DODGE:
			// yes?
			break;

		default:
			break;
	};
}

void SkillEngine::genDiffs(Play* _play)
{
	// need beatmap for difficulty generation
	if (!_play->beatmap->isValid())	return;

	switch (gamemode)
	{
		case GAMEMODE::OSU_STANDARD:
			OSUSTANDARD::SPEED_CONTROL::genDiff(_play);
			OSUSTANDARD::TAPPING_CONTROL::genDiff(_play);
			OSUSTANDARD::READING::genDiff(_play);
			OSUSTANDARD::AIM_CONTROL::genDiff(_play);
			OSUSTANDARD::MOVEMENT_CONTROL::genDiff(_play);
			OSUSTANDARD::PRECISION_CONTROL::genDiff(_play);
			break;

		case GAMEMODE::OSU_TAIKO:
			OSUTAIKO::SPEED_CONTROL::genDiff(_play);
			OSUTAIKO::TAPPING_CONTROL::genDiff(_play);
			OSUTAIKO::READING::genDiff(_play);
			break;

		case GAMEMODE::OSU_CATCH:
			OSUCATCH::SPEED_CONTROL::genDiff(_play);
			OSUCATCH::TAPPING_CONTROL::genDiff(_play);
			OSUCATCH::READING::genDiff(_play);
			OSUCATCH::PRECISION::genDiff(_play);
			break;

		case GAMEMODE::OSU_MANIA:
			OSUMANIA::SPEED_CONTROL::genDiff(_play);
			OSUMANIA::TAPPING_CONTROL::genDiff(_play);
			OSUMANIA::READING::genDiff(_play);
			OSUMANIA::ACCURACY::genDiff(_play);
			OSUMANIA::FINGER_BALANCE::genDiff(_play);
			OSUMANIA::HAND_BALANCE::genDiff(_play);
			break;

		case GAMEMODE::OSU_DODGE:
			// yes?
			break;

		default:
			break;
	};
}