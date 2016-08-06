#ifndef OSU_MANIA_H
#define OSU_MANIA_H

#include "../utils/mathUtils.h"
#include "osuStructs.h"

namespace OSUMANIA
{
	inline int getKey(int _xpos, int _keys)
	{
		float localWDivisor = 512.0f / _keys;
		return std::min((int)std::floor(_xpos / localWDivisor), _keys - 1);
	}
}

#endif // !OSU_MANIA_H