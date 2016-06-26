#include "osuCatch_skills.h"

double OSUCATCH::SKILLS::getScore(SkillEngine::SKILL _skill)
{
	switch (_skill)
	{
		case SkillEngine::ANY_TAPPINGCONTROL:
			return OSUCATCH::TAPPING_CONTROL::getScore();
			break;

		case SkillEngine::ANY_SPEEDCONTROL:
			return OSUCATCH::SPEED_CONTROL::getScore();
			break;

		case SkillEngine::ANY_READING:
			return OSUCATCH::READING::getScore();
			break;

		case SkillEngine::CTB_PRECISION:
			return OSUCATCH::PRECISION::getScore();
			break;

		default:
			return -1;
			break;
	}
}

double OSUCATCH::SKILLS::getSkill(SkillEngine::SKILL _skill)
{
	switch (_skill)
	{
		case SkillEngine::ANY_TAPPINGCONTROL:
			return OSUCATCH::TAPPING_CONTROL::getSkill();
			break;

		case SkillEngine::ANY_SPEEDCONTROL:
			return OSUCATCH::SPEED_CONTROL::getSkill();
			break;

		case SkillEngine::ANY_READING:
			return OSUCATCH::READING::getSkill();
			break;

		case SkillEngine::CTB_PRECISION:
			return OSUCATCH::PRECISION::getSkill();
			break;

		default:
			return -1;
			break;
	}
}

double OSUCATCH::SKILLS::getDiff(SkillEngine::SKILL _skill)
{
	switch (_skill)
	{
		case SkillEngine::ANY_TAPPINGCONTROL:
			return OSUCATCH::TAPPING_CONTROL::getDiff();
			break;

		case SkillEngine::ANY_SPEEDCONTROL:
			return OSUCATCH::SPEED_CONTROL::getDiff();
			break;

		case SkillEngine::ANY_READING:
			return OSUCATCH::READING::getDiff();
			break;

		case SkillEngine::CTB_PRECISION:
			return OSUCATCH::PRECISION::getDiff();
			break;
	
		default:
			return -1;
			break;
	}
}


double OSUCATCH::SKILLS::getScoreAt(SkillEngine::SKILL _skill, int _time)
{
	switch (_skill)
	{
		case SkillEngine::ANY_TAPPINGCONTROL:
			return OSUCATCH::TAPPING_CONTROL::getScoreAt(_time);
			break;

		case SkillEngine::ANY_SPEEDCONTROL:
			return OSUCATCH::SPEED_CONTROL::getScoreAt(_time);
			break;

		case SkillEngine::ANY_READING:
			return OSUCATCH::READING::getScoreAt(_time);
			break;

		case SkillEngine::CTB_PRECISION:
			return OSUCATCH::PRECISION::getScoreAt(_time);
			break;

		default:
			return -1;
			break;
	}
}

double OSUCATCH::SKILLS::getSkillAt(SkillEngine::SKILL _skill, int _time)
{
	switch (_skill)
	{
		case SkillEngine::ANY_TAPPINGCONTROL:
			return OSUCATCH::TAPPING_CONTROL::getScoreAt(_time);
			break;

		case SkillEngine::ANY_SPEEDCONTROL:
			return OSUCATCH::SPEED_CONTROL::getScoreAt(_time);
			break;

		case SkillEngine::ANY_READING:
			return OSUCATCH::READING::getScoreAt(_time);
			break;

		case SkillEngine::CTB_PRECISION:
			return OSUCATCH::PRECISION::getScoreAt(_time);
			break;

		default:
			return -1;
			break;
	}
}

double OSUCATCH::SKILLS::getDiffAt(SkillEngine::SKILL _skill, int _time)
{
	switch (_skill)
	{
		case SkillEngine::ANY_TAPPINGCONTROL:
			return OSUCATCH::TAPPING_CONTROL::getDiffAt(_time);
			break;

		case SkillEngine::ANY_SPEEDCONTROL:
			return OSUCATCH::SPEED_CONTROL::getDiffAt(_time);
			break;

		case SkillEngine::ANY_READING:
			return OSUCATCH::READING::getDiffAt(_time);
			break;

		case SkillEngine::CTB_PRECISION:
			return OSUCATCH::PRECISION::getDiffAt(_time);
			break;

		default:
			return -1;
			break;
	}
}


double OSUCATCH::SKILLS::getTotalScoreAt(SkillEngine::SKILL _skill, int _time)
{
	switch (_skill)
	{
		case SkillEngine::ANY_TAPPINGCONTROL:
			return OSUCATCH::TAPPING_CONTROL::getTotalScoreAt(_time);
			break;

		case SkillEngine::ANY_SPEEDCONTROL:
			return OSUCATCH::SPEED_CONTROL::getTotalScoreAt(_time);
			break;

		case SkillEngine::ANY_READING:
			return OSUCATCH::READING::getTotalScoreAt(_time);
			break;

		case SkillEngine::CTB_PRECISION:
			return OSUCATCH::PRECISION::getTotalScoreAt(_time);
			break;

		default:
			return -1;
			break;
	}
}

double OSUCATCH::SKILLS::getTotalSkillAt(SkillEngine::SKILL _skill, int _time)
{
	switch (_skill)
	{
		case SkillEngine::ANY_TAPPINGCONTROL:
			return OSUCATCH::TAPPING_CONTROL::getTotalSkillAt(_time);
			break;

		case SkillEngine::ANY_SPEEDCONTROL:
			return OSUCATCH::SPEED_CONTROL::getTotalSkillAt(_time);
			break;

		case SkillEngine::ANY_READING:
			return OSUCATCH::READING::getTotalSkillAt(_time);
			break;

		case SkillEngine::CTB_PRECISION:
			return OSUCATCH::PRECISION::getTotalSkillAt(_time);
			break;

		default:
			return -1;
			break;
	}
}

double OSUCATCH::SKILLS::getTotalDiffAt(SkillEngine::SKILL _skill, int _time)
{
	switch (_skill)
	{
		case SkillEngine::ANY_TAPPINGCONTROL:
			return OSUCATCH::TAPPING_CONTROL::getTotalDiffAt(_time);
			break;

		case SkillEngine::ANY_SPEEDCONTROL:
			return OSUCATCH::SPEED_CONTROL::getTotalDiffAt(_time);
			break;

		case SkillEngine::ANY_READING:
			return OSUCATCH::READING::getTotalDiffAt(_time);
			break;

		case SkillEngine::CTB_PRECISION:
			return OSUCATCH::PRECISION::getTotalDiffAt(_time);
			break;

		default:
			return -1;
			break;
	}
}