#ifndef OSUSTANDARD_SKILLS_H
#define OSUSTANDARD_SKILLS_H

#include "SkillEngine.h"

#include "osu-std/tappingControl.h"
#include "osu-std/speedControl.h"
#include "osu-std/reading.h"
#include "osu-std/aimControl.h"
#include "osu-std/movementControl.h"
#include "osu-std/precisionControl.h"

namespace OSUSTANDARD
{
	namespace SKILLS
	{
		double getSkill(SkillEngine::SKILL _skill)
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


		double getDiff(SkillEngine::SKILL _skill)
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


		double getSkillAt(SkillEngine::SKILL _skill, int _time)
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


		double getDiffAt(SkillEngine::SKILL _skill, int _time)
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


		double getTotalSkillAt(SkillEngine::SKILL _skill, int _time)
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


		double getTotalDiffAt(SkillEngine::SKILL _skill, int _time)
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
	};
}

#endif
