#include "Mods.h"
#include "../osuCalc.h"
#include <algorithm>

Mods::Mods()
{
	AR = 0.0;
	CS = 0.0;
	OD = 0.0;
	TM = 0.0;
}

void Mods::setModifier(MODS _mod)
{
	switch (_mod)
	{
		case HT:
			mod.HT = true;
			mod.DT = false;
			break;

		case DT: case NC:
			mod.DT = true;
			mod.HT = false;
			break;

		case EZ:
			mod.EZ = true;
			mod.HR = false;
			break;

		case HR:
			mod.HR = true;
			mod.EZ = false;
			break;

		case HD:
			mod.HD = true;
			mod.FI = false;
			break;

		case FI:
			mod.FI = true;
			mod.HD = false;
			break;

		case FL:
			mod.FL = true;
			break;

		default:
			break;
	}

	ApplyDiffMods();
}

Mods::Modifier Mods::getModifier()
{
	return mod;
}

void Mods::setAR(double _AR)
{
	mod.EZ = false;
	mod.HR = false;

	if(AR < 13.0)
		AR = _AR;
}

void Mods::setCS(double _CS)
{
	mod.EZ = false;
	mod.HR = false;

	/// \TODO calculate the value for which the circle's size would be 0
	// if(CS <= ??) 
	CS = _CS;
}

void Mods::setOD(double _OD)
{
	mod.EZ = false;
	mod.HR = false;

	/// \TODO calculate the value for which the tapping window would be 0
	// if(OD <= ??) /// \TODO
	OD = _OD;
}

void Mods::setTM(double _TM)
{
	mod.DT = false;
	mod.HT = false;

	TM = _TM;
}

double Mods::getAR()
{
	return AR;
}

double Mods::getCS()
{
	return CS;
}

double Mods::getOD()
{
	return OD;
}

double Mods::getTM()
{
	return TM;
}

void Mods::ApplyDiffMods()
{
	if (mod.DT)
	{
		TM *= 1.5;
		AR = std::min(ms2AR(AR2ms(AR) / 1.5), 11.0);
	}

	if (mod.HT)
	{
		TM *= 0.75;
		AR = ms2AR(AR2ms(AR) / 0.75);
	}

	if (mod.EZ)
	{
		AR *= 0.5;
		OD *= 0.5;
		CS *= 0.5;
	}

	if (mod.HR)
	{		
		AR = std::min(AR*1.4, 10.0);
		OD = std::min(CS*1.4, 10.0);
		CS = std::min(CS*1.3, 10.0);
	}
}