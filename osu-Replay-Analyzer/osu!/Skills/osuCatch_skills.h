#ifndef OSUCATCH_SKILLS_H
#define OSUCATCH_SKILLS_H

#include "SkillEngine.h"

#include "osu-catch/tappingControl.h"
#include "osu-catch/speedControl.h"
#include "osu-catch/reading.h"
#include "osu-catch/precision.h"

namespace OSUCATCH
{
	namespace SKILLS
	{
		double getSkill(SkillEngine::SKILL _skill)
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


		double getDiff(SkillEngine::SKILL _skill)
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


		double getSkillAt(SkillEngine::SKILL _skill, int _time)
		{
			switch (_skill)
			{
				case SkillEngine::ANY_TAPPINGCONTROL:
					return OSUCATCH::TAPPING_CONTROL::getSkillAt(_time);
					break;

				case SkillEngine::ANY_SPEEDCONTROL:
					return OSUCATCH::SPEED_CONTROL::getSkillAt(_time);
					break;

				case SkillEngine::ANY_READING:
					return OSUCATCH::READING::getSkillAt(_time);
					break;

				case SkillEngine::CTB_PRECISION:
					return OSUCATCH::PRECISION::getSkillAt(_time);
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


		double getTotalSkillAt(SkillEngine::SKILL _skill, int _time)
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


		double getTotalDiffAt(SkillEngine::SKILL _skill, int _time)
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
	};
}

#endif
