#include "osuStandard_skills.h"

double OSUSTANDARD::SKILLS::getScore(SkillEngine::SKILL _skill)
{
	switch (_skill)
	{
		case SkillEngine::ANY_TAPPINGCONTROL:
			return OSUSTANDARD::TAPPING_CONTROL::getScore();
			break;

		case SkillEngine::ANY_SPEEDCONTROL:
			return OSUSTANDARD::SPEED_CONTROL::getScore();
			break;

		case SkillEngine::ANY_READING:
			return OSUSTANDARD::READING::getScore();
			break;

		case SkillEngine::STD_AIMCONTROL:
			return OSUSTANDARD::AIM_CONTROL::getScore();
			break;

		case SkillEngine::STD_MOVEMENTCONTROL:
			return OSUSTANDARD::MOVEMENT_CONTROL::getScore();
			break;

		case SkillEngine::STD_PRECISIONCONTROL:
			return OSUSTANDARD::PRECISION_CONTROL::getScore();
			break;

		default:
			return -1;
			break;
	}
}

double OSUSTANDARD::SKILLS::getSkill(SkillEngine::SKILL _skill)
{
	switch (_skill)
	{
		case SkillEngine::ANY_TAPPINGCONTROL:
			return OSUSTANDARD::TAPPING_CONTROL::getSkill();
			break;

		case SkillEngine::ANY_SPEEDCONTROL:
			return OSUSTANDARD::SPEED_CONTROL::getSkill();
			break;

		case SkillEngine::ANY_READING:
			return OSUSTANDARD::READING::getSkill();
			break;

		case SkillEngine::STD_AIMCONTROL:
			return OSUSTANDARD::AIM_CONTROL::getSkill();
			break;

		case SkillEngine::STD_MOVEMENTCONTROL:
			return OSUSTANDARD::MOVEMENT_CONTROL::getSkill();
			break;

		case SkillEngine::STD_PRECISIONCONTROL:
			return OSUSTANDARD::PRECISION_CONTROL::getSkill();
			break;

		default:
			return -1;
			break;
	}
}

double OSUSTANDARD::SKILLS::getDiff(SkillEngine::SKILL _skill)
{
	switch (_skill)
	{
		case SkillEngine::ANY_TAPPINGCONTROL:
			return OSUSTANDARD::TAPPING_CONTROL::getDiff();
			break;

		case SkillEngine::ANY_SPEEDCONTROL:
			return OSUSTANDARD::SPEED_CONTROL::getDiff();
			break;

		case SkillEngine::ANY_READING:
			return OSUSTANDARD::READING::getDiff();
			break;

		case SkillEngine::STD_AIMCONTROL:
			return OSUSTANDARD::AIM_CONTROL::getDiff();
			break;

		case SkillEngine::STD_MOVEMENTCONTROL:
			return OSUSTANDARD::MOVEMENT_CONTROL::getDiff();
			break;

		case SkillEngine::STD_PRECISIONCONTROL:
			return OSUSTANDARD::PRECISION_CONTROL::getDiff();
			break;
	
		default:
			return -1;
			break;
	}
}


double OSUSTANDARD::SKILLS::getScoreAt(SkillEngine::SKILL _skill, int _time)
{
	switch (_skill)
	{
		case SkillEngine::ANY_TAPPINGCONTROL:
			return OSUSTANDARD::TAPPING_CONTROL::getScoreAt(_time);
			break;

		case SkillEngine::ANY_SPEEDCONTROL:
			return OSUSTANDARD::SPEED_CONTROL::getScoreAt(_time);
			break;

		case SkillEngine::ANY_READING:
			return OSUSTANDARD::READING::getScoreAt(_time);
			break;

		case SkillEngine::STD_AIMCONTROL:
			return OSUSTANDARD::AIM_CONTROL::getScoreAt(_time);
			break;

		case SkillEngine::STD_MOVEMENTCONTROL:
			return OSUSTANDARD::MOVEMENT_CONTROL::getScoreAt(_time);
			break;

		case SkillEngine::STD_PRECISIONCONTROL:
			return OSUSTANDARD::PRECISION_CONTROL::getScoreAt(_time);
			break;

		default:
			return -1;
			break;
	}
}

double OSUSTANDARD::SKILLS::getSkillAt(SkillEngine::SKILL _skill, int _time)
{
	switch (_skill)
	{
		case SkillEngine::ANY_TAPPINGCONTROL:
			return OSUSTANDARD::TAPPING_CONTROL::getSkillAt(_time);
			break;

		case SkillEngine::ANY_SPEEDCONTROL:
			return OSUSTANDARD::SPEED_CONTROL::getSkillAt(_time);
			break;

		case SkillEngine::ANY_READING:
			return OSUSTANDARD::READING::getSkillAt(_time);
			break;

		case SkillEngine::STD_AIMCONTROL:
			return OSUSTANDARD::AIM_CONTROL::getSkillAt(_time);
			break;

		case SkillEngine::STD_MOVEMENTCONTROL:
			return OSUSTANDARD::MOVEMENT_CONTROL::getSkillAt(_time);
			break;

		case SkillEngine::STD_PRECISIONCONTROL:
			return OSUSTANDARD::PRECISION_CONTROL::getSkillAt(_time);
			break;

		default:
			return -1;
			break;
	}
}

double OSUSTANDARD::SKILLS::getDiffAt(SkillEngine::SKILL _skill, int _time)
{
	switch (_skill)
	{
		case SkillEngine::ANY_TAPPINGCONTROL:
			return OSUSTANDARD::TAPPING_CONTROL::getDiffAt(_time);
			break;

		case SkillEngine::ANY_SPEEDCONTROL:
			return OSUSTANDARD::SPEED_CONTROL::getDiffAt(_time);
			break;

		case SkillEngine::ANY_READING:
			return OSUSTANDARD::READING::getDiffAt(_time);
			break;

		case SkillEngine::STD_AIMCONTROL:
			return OSUSTANDARD::AIM_CONTROL::getDiffAt(_time);
			break;

		case SkillEngine::STD_MOVEMENTCONTROL:
			return OSUSTANDARD::MOVEMENT_CONTROL::getDiffAt(_time);
			break;

		case SkillEngine::STD_PRECISIONCONTROL:
			return OSUSTANDARD::PRECISION_CONTROL::getDiffAt(_time);
			break;

		default:
			return -1;
			break;
	}
}


double OSUSTANDARD::SKILLS::getTotalScoreAt(SkillEngine::SKILL _skill, int _time)
{
	switch (_skill)
	{
		case SkillEngine::ANY_TAPPINGCONTROL:
			return OSUSTANDARD::TAPPING_CONTROL::getTotalScoreAt(_time);
			break;

		case SkillEngine::ANY_SPEEDCONTROL:
			return OSUSTANDARD::SPEED_CONTROL::getTotalScoreAt(_time);
			break;

		case SkillEngine::ANY_READING:
			return OSUSTANDARD::READING::getTotalScoreAt(_time);
			break;

		case SkillEngine::STD_AIMCONTROL:
			return OSUSTANDARD::AIM_CONTROL::getTotalScoreAt(_time);
			break;

		case SkillEngine::STD_MOVEMENTCONTROL:
			return OSUSTANDARD::MOVEMENT_CONTROL::getTotalScoreAt(_time);
			break;

		case SkillEngine::STD_PRECISIONCONTROL:
			return OSUSTANDARD::PRECISION_CONTROL::getTotalScoreAt(_time);
			break;

		default:
			return -1;
			break;
	}
}

double OSUSTANDARD::SKILLS::getTotalSkillAt(SkillEngine::SKILL _skill, int _time)
{
	switch (_skill)
	{
		case SkillEngine::ANY_TAPPINGCONTROL:
			return OSUSTANDARD::TAPPING_CONTROL::getTotalSkillAt(_time);
			break;

		case SkillEngine::ANY_SPEEDCONTROL:
			return OSUSTANDARD::SPEED_CONTROL::getTotalSkillAt(_time);
			break;

		case SkillEngine::ANY_READING:
			return OSUSTANDARD::READING::getTotalSkillAt(_time);
			break;

		case SkillEngine::STD_AIMCONTROL:
			return OSUSTANDARD::AIM_CONTROL::getTotalSkillAt(_time);
			break;

		case SkillEngine::STD_MOVEMENTCONTROL:
			return OSUSTANDARD::MOVEMENT_CONTROL::getTotalSkillAt(_time);
			break;

		case SkillEngine::STD_PRECISIONCONTROL:
			return OSUSTANDARD::PRECISION_CONTROL::getTotalSkillAt(_time);
			break;

		default:
			return -1;
			break;
	}
}

double OSUSTANDARD::SKILLS::getTotalDiffAt(SkillEngine::SKILL _skill, int _time)
{
	switch (_skill)
	{
		case SkillEngine::ANY_TAPPINGCONTROL:
			return OSUSTANDARD::TAPPING_CONTROL::getTotalDiffAt(_time);
			break;

		case SkillEngine::ANY_SPEEDCONTROL:
			return OSUSTANDARD::SPEED_CONTROL::getTotalDiffAt(_time);
			break;

		case SkillEngine::ANY_READING:
			return OSUSTANDARD::READING::getTotalDiffAt(_time);
			break;

		case SkillEngine::STD_AIMCONTROL:
			return OSUSTANDARD::AIM_CONTROL::getTotalDiffAt(_time);
			break;

		case SkillEngine::STD_MOVEMENTCONTROL:
			return OSUSTANDARD::MOVEMENT_CONTROL::getTotalDiffAt(_time);
			break;

		case SkillEngine::STD_PRECISIONCONTROL:
			return OSUSTANDARD::PRECISION_CONTROL::getTotalDiffAt(_time);
			break;

		default:
			return -1;
			break;
	}
}