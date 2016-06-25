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
		double getSkill(SkillEngine::SKILL _skill)
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


		double getDiff(SkillEngine::SKILL _skill)
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


		double getSkillAt(SkillEngine::SKILL _skill, int _time)
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


		double getDiffAt(SkillEngine::SKILL _skill, int _time)
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


		double getTotalSkillAt(SkillEngine::SKILL _skill, int _time)
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


		double getTotalDiffAt(SkillEngine::SKILL _skill, int _time)
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
	};
}

#endif // !OSUMANIA_SKILLS_H
