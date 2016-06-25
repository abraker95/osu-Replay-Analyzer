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
		double getSkill(SkillEngine::SKILL _skill)
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


		double getDiff(SkillEngine::SKILL _skill)
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


		double getSkillAt(SkillEngine::SKILL _skill, int _time)
		{
			switch (_skill)
			{
				case SkillEngine::ANY_TAPPINGCONTROL:
					return OSUTAIKO::TAPPING_CONTROL::getSkillAt(_time);
					break;

				case SkillEngine::ANY_SPEEDCONTROL:
					return OSUTAIKO::SPEED_CONTROL::getSkillAt(_time);
					break;

				case SkillEngine::ANY_READING:
					return OSUTAIKO::READING::getSkillAt(_time);
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


		double getTotalSkillAt(SkillEngine::SKILL _skill, int _time)
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


		double getTotalDiffAt(SkillEngine::SKILL _skill, int _time)
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
	};
}


#endif
