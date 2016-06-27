#include "osuMania_skills.h"

double OSUMANIA::SKILLS::getScore(SkillEngine::SKILL _skill)
{
	switch (_skill)
	{
		case SkillEngine::ANY_TAPPINGCONTROL:
			return OSUMANIA::TAPPING_CONTROL::getScore();
			break;

		case SkillEngine::ANY_SPEEDCONTROL:
			return OSUMANIA::SPEED_CONTROL::getScore();
			break;

		case SkillEngine::ANY_READING:
			return OSUMANIA::READING::getScore();
			break;

		case SkillEngine::MANIA_FINGERBALANCE:
			return OSUMANIA::FINGER_BALANCE::getScore();
			break;

		case SkillEngine::MANIA_HANDBALANCE:
			return OSUMANIA::HAND_BALANCE::getScore();
			break;

		case SkillEngine::MANIA_ACCURACY:
			return OSUMANIA::ACCURACY::getScore();
			break;

		default:
			return -1;
			break;
	}
}

double OSUMANIA::SKILLS::getSkill(SkillEngine::SKILL _skill)
{
	switch (_skill)
	{
		case SkillEngine::ANY_TAPPINGCONTROL:
			return OSUMANIA::TAPPING_CONTROL::getSkill();
			break;

		case SkillEngine::ANY_SPEEDCONTROL:
			return OSUMANIA::SPEED_CONTROL::getSkill();
			break;

		case SkillEngine::ANY_READING:
			return OSUMANIA::READING::getSkill();
			break;

		case SkillEngine::MANIA_FINGERBALANCE:
			return OSUMANIA::FINGER_BALANCE::getSkill();
			break;

		case SkillEngine::MANIA_HANDBALANCE:
			return OSUMANIA::HAND_BALANCE::getSkill();
			break;

		case SkillEngine::MANIA_ACCURACY:
			return OSUMANIA::ACCURACY::getSkill();
			break;
	
		default:
			return -1;
			break;
	}
}

double OSUMANIA::SKILLS::getDiff(SkillEngine::SKILL _skill)
{
	switch (_skill)
	{
		case SkillEngine::ANY_TAPPINGCONTROL:
			return OSUMANIA::TAPPING_CONTROL::getDiff();
			break;

		case SkillEngine::ANY_SPEEDCONTROL:
			return OSUMANIA::SPEED_CONTROL::getDiff();
			break;

		case SkillEngine::ANY_READING:
			return OSUMANIA::READING::getDiff();
			break;

		case SkillEngine::MANIA_FINGERBALANCE:
			return OSUMANIA::FINGER_BALANCE::getDiff();
			break;

		case SkillEngine::MANIA_HANDBALANCE:
			return OSUMANIA::HAND_BALANCE::getDiff();
			break;

		case SkillEngine::MANIA_ACCURACY:
			return OSUMANIA::ACCURACY::getDiff();
			break;
	
		default:
			return -1;
			break;
	}
}


double OSUMANIA::SKILLS::getScoreAt(SkillEngine::SKILL _skill, int _time)
{
	switch (_skill)
	{
		case SkillEngine::ANY_TAPPINGCONTROL:
			return OSUMANIA::TAPPING_CONTROL::getScoreAt(_time);
			break;

		case SkillEngine::ANY_SPEEDCONTROL:
			return OSUMANIA::SPEED_CONTROL::getScoreAt(_time);
			break;

		case SkillEngine::ANY_READING:
			return OSUMANIA::READING::getScoreAt(_time);
			break;

		case SkillEngine::MANIA_FINGERBALANCE:
			return OSUMANIA::FINGER_BALANCE::getScoreAt(_time);
			break;

		case SkillEngine::MANIA_HANDBALANCE:
			return OSUMANIA::HAND_BALANCE::getScoreAt(_time);
			break;

		case SkillEngine::MANIA_ACCURACY:
			return OSUMANIA::ACCURACY::getScoreAt(_time);
			break;

		default:
			return -1;
			break;
	}
}

double OSUMANIA::SKILLS::getSkillAt(SkillEngine::SKILL _skill, int _time)
{
	switch (_skill)
	{
		case SkillEngine::ANY_TAPPINGCONTROL:
			return OSUMANIA::TAPPING_CONTROL::getSkillAt(_time);
			break;

		case SkillEngine::ANY_SPEEDCONTROL:
			return OSUMANIA::SPEED_CONTROL::getSkillAt(_time);
			break;
	
		case SkillEngine::ANY_READING:
			return OSUMANIA::READING::getSkillAt(_time);
			break;

		case SkillEngine::MANIA_FINGERBALANCE:
			return OSUMANIA::FINGER_BALANCE::getSkillAt(_time);
			break;

		case SkillEngine::MANIA_HANDBALANCE:
			return OSUMANIA::HAND_BALANCE::getSkillAt(_time);
			break;

		case SkillEngine::MANIA_ACCURACY:
			return OSUMANIA::ACCURACY::getSkillAt(_time);
			break;
	
		default:
			return -1;
			break;
	}
}

double OSUMANIA::SKILLS::getDiffAt(SkillEngine::SKILL _skill, int _time)
{
	switch (_skill)
	{
		case SkillEngine::ANY_TAPPINGCONTROL:
			return OSUMANIA::TAPPING_CONTROL::getDiffAt(_time);
			break;
	
		case SkillEngine::ANY_SPEEDCONTROL:
			return OSUMANIA::SPEED_CONTROL::getDiffAt(_time);
			break;

		case SkillEngine::ANY_READING:
			return OSUMANIA::READING::getDiffAt(_time);
			break;

		case SkillEngine::MANIA_FINGERBALANCE:
			return OSUMANIA::FINGER_BALANCE::getDiffAt(_time);
			break;
	
		case SkillEngine::MANIA_HANDBALANCE:
			return OSUMANIA::HAND_BALANCE::getDiffAt(_time);
			break;

		case SkillEngine::MANIA_ACCURACY:
			return OSUMANIA::ACCURACY::getDiffAt(_time);
			break;

		default:
			return -1;
			break;
	}
}


double OSUMANIA::SKILLS::getTotalScoreAt(SkillEngine::SKILL _skill, int _time)
{
	switch (_skill)
	{
		case SkillEngine::ANY_TAPPINGCONTROL:
			return OSUMANIA::TAPPING_CONTROL::getTotalScoreAt(_time);
			break;

		case SkillEngine::ANY_SPEEDCONTROL:
			return OSUMANIA::SPEED_CONTROL::getTotalScoreAt(_time);
			break;

		case SkillEngine::ANY_READING:
			return OSUMANIA::READING::getTotalScoreAt(_time);
			break;

		case SkillEngine::MANIA_FINGERBALANCE:
			return OSUMANIA::FINGER_BALANCE::getTotalScoreAt(_time);
			break;

		case SkillEngine::MANIA_HANDBALANCE:
			return OSUMANIA::HAND_BALANCE::getTotalScoreAt(_time);
			break;

		case SkillEngine::MANIA_ACCURACY:
			return OSUMANIA::ACCURACY::getTotalScoreAt(_time);
			break;

		default:
			return -1;
			break;
	}
}

double OSUMANIA::SKILLS::getTotalSkillAt(SkillEngine::SKILL _skill, int _time)
{
	switch (_skill)
	{
		case SkillEngine::ANY_TAPPINGCONTROL:
			return OSUMANIA::TAPPING_CONTROL::getTotalSkillAt(_time);
			break;

		case SkillEngine::ANY_SPEEDCONTROL:
			return OSUMANIA::SPEED_CONTROL::getTotalSkillAt(_time);
			break;

		case SkillEngine::ANY_READING:
			return OSUMANIA::READING::getTotalSkillAt(_time);
			break;

		case SkillEngine::MANIA_FINGERBALANCE:
			return OSUMANIA::FINGER_BALANCE::getTotalSkillAt(_time);
			break;

		case SkillEngine::MANIA_HANDBALANCE:
			return OSUMANIA::HAND_BALANCE::getTotalSkillAt(_time);
			break;

		case SkillEngine::MANIA_ACCURACY:
			return OSUMANIA::ACCURACY::getTotalSkillAt(_time);
			break;

		default:
			return -1;
			break;
	}
}

double OSUMANIA::SKILLS::getTotalDiffAt(SkillEngine::SKILL _skill, int _time)
{
	switch (_skill)
	{
		case SkillEngine::ANY_TAPPINGCONTROL:
			return OSUMANIA::TAPPING_CONTROL::getTotalDiffAt(_time);
			break;

		case SkillEngine::ANY_SPEEDCONTROL:
			return OSUMANIA::SPEED_CONTROL::getTotalDiffAt(_time);
			break;

		case SkillEngine::ANY_READING:
			return OSUMANIA::READING::getTotalDiffAt(_time);
			break;

		case SkillEngine::MANIA_FINGERBALANCE:
			return OSUMANIA::FINGER_BALANCE::getTotalDiffAt(_time);
			break;

		case SkillEngine::MANIA_HANDBALANCE:
			return OSUMANIA::HAND_BALANCE::getTotalDiffAt(_time);
			break;

		case SkillEngine::MANIA_ACCURACY:
			return OSUMANIA::ACCURACY::getTotalDiffAt(_time);
			break;

		default:
			return -1;
			break;
	}
}


double OSUMANIA::SKILLS::Time2AccScore(int _ms)
{
	// scale is basically OD and is modeled here https://www.desmos.com/calculator/lcxkub6pkz
	double scale = 0.5; // 0.5 @ 60 FPS	 aka set a MAX in mania = 50%. This makes 1M scores in mania equivelant to 500k here assuming all hit timings are no less than 16ms
	double o = sqrt(-pow((50.0 / 3.0), 2.0) / (2.0 * log(scale)));

	return exp(-pow(_ms, 2.0) / (2.0 * pow(o, 2.0)));
}