#include "osuTaiko_skills.h"

double OSUTAIKO::SKILLS::getScore(SkillEngine::SKILL _skill)
{
	switch (_skill)
	{
		case SkillEngine::ANY_TAPPINGCONTROL:
			return OSUTAIKO::TAPPING_CONTROL::getScore();
			break;

		case SkillEngine::ANY_SPEEDCONTROL:
			return OSUTAIKO::SPEED_CONTROL::getScore();
			break;

		case SkillEngine::ANY_READING:
			return OSUTAIKO::READING::getScore();
			break;

		default:
			return -1;
			break;
	}
}

double OSUTAIKO::SKILLS::getSkill(SkillEngine::SKILL _skill)
{
	switch (_skill)
	{
		case SkillEngine::ANY_TAPPINGCONTROL:
			return OSUTAIKO::TAPPING_CONTROL::getSkill();
			break;

		case SkillEngine::ANY_SPEEDCONTROL:
			return OSUTAIKO::SPEED_CONTROL::getSkill();
			break;

		case SkillEngine::ANY_READING:
			return OSUTAIKO::READING::getSkill();
			break;

		default:
			return -1;
			break;
	}
}

double OSUTAIKO::SKILLS::getDiff(SkillEngine::SKILL _skill)
{
	switch (_skill)
	{
		case SkillEngine::ANY_TAPPINGCONTROL:
			return OSUTAIKO::TAPPING_CONTROL::getDiff();
			break;

		case SkillEngine::ANY_SPEEDCONTROL:
			return OSUTAIKO::SPEED_CONTROL::getDiff();
			break;

		case SkillEngine::ANY_READING:
			return OSUTAIKO::READING::getDiff();
			break;

		default:
			return -1;
			break;
	}
}


double OSUTAIKO::SKILLS::getScoreAt(SkillEngine::SKILL _skill, int _time)
{
	switch (_skill)
	{
		case SkillEngine::ANY_TAPPINGCONTROL:
			return OSUTAIKO::TAPPING_CONTROL::getScoreAt(_time);
			break;

		case SkillEngine::ANY_SPEEDCONTROL:
			return OSUTAIKO::SPEED_CONTROL::getScoreAt(_time);
			break;

		case SkillEngine::ANY_READING:
			return OSUTAIKO::READING::getScoreAt(_time);
			break;

		default:
			return -1;
			break;
	}
}

double OSUTAIKO::SKILLS::getSkillAt(SkillEngine::SKILL _skill, int _time)
{
	switch (_skill)
	{
		case SkillEngine::ANY_TAPPINGCONTROL:
			return OSUTAIKO::TAPPING_CONTROL::getScoreAt(_time);
			break;

		case SkillEngine::ANY_SPEEDCONTROL:
			return OSUTAIKO::SPEED_CONTROL::getScoreAt(_time);
			break;

		case SkillEngine::ANY_READING:
			return OSUTAIKO::READING::getScoreAt(_time);
			break;

		default:
			return -1;
			break;
	}
}

double OSUTAIKO::SKILLS::getDiffAt(SkillEngine::SKILL _skill, int _time)
{
	switch (_skill)
	{
		case SkillEngine::ANY_TAPPINGCONTROL:
			return OSUTAIKO::TAPPING_CONTROL::getDiffAt(_time);
			break;

		case SkillEngine::ANY_SPEEDCONTROL:
			return OSUTAIKO::SPEED_CONTROL::getDiffAt(_time);
			break;

		case SkillEngine::ANY_READING:
			return OSUTAIKO::READING::getDiffAt(_time);
			break;

		default:
			return -1;
			break;
	}
}


double OSUTAIKO::SKILLS::getTotalScoreAt(SkillEngine::SKILL _skill, int _time)
{
	switch (_skill)
	{
		case SkillEngine::ANY_TAPPINGCONTROL:
			return OSUTAIKO::TAPPING_CONTROL::getTotalScoreAt(_time);
			break;

		case SkillEngine::ANY_SPEEDCONTROL:
			return OSUTAIKO::SPEED_CONTROL::getTotalScoreAt(_time);
			break;

		case SkillEngine::ANY_READING:
			return OSUTAIKO::READING::getTotalScoreAt(_time);
			break;

		default:
			return -1;
			break;
	}
}

double OSUTAIKO::SKILLS::getTotalSkillAt(SkillEngine::SKILL _skill, int _time)
{
	switch (_skill)
	{
		case SkillEngine::ANY_TAPPINGCONTROL:
			return OSUTAIKO::TAPPING_CONTROL::getTotalSkillAt(_time);
			break;

		case SkillEngine::ANY_SPEEDCONTROL:
			return OSUTAIKO::SPEED_CONTROL::getTotalSkillAt(_time);
			break;

		case SkillEngine::ANY_READING:
			return OSUTAIKO::READING::getTotalSkillAt(_time);
			break;

		default:
			return -1;
			break;
	}
}

double OSUTAIKO::SKILLS::getTotalDiffAt(SkillEngine::SKILL _skill, int _time)
{
	switch (_skill)
	{
		case SkillEngine::ANY_TAPPINGCONTROL:
			return OSUTAIKO::TAPPING_CONTROL::getTotalDiffAt(_time);
			break;

		case SkillEngine::ANY_SPEEDCONTROL:
			return OSUTAIKO::SPEED_CONTROL::getTotalDiffAt(_time);
			break;

		case SkillEngine::ANY_READING:
			return OSUTAIKO::READING::getTotalDiffAt(_time);
			break;

		default:
			return -1;
			break;
	}
}