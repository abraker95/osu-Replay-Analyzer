#ifndef OSU_STANDARD_H
#define OSU_STANDARD_H

#include <math.h>
#include <vector>
#include "../irrlicht/include/vector2d.h"
#include "Filestructure\Hitobject.h"
#include "Filestructure\SliderHitObject.h"
#include "osuStructs.h"

namespace OSUSTANDARD
{
	bool isHitobjectAt(std::vector<Hitobject*>& _hitobjects, long _prevTime, long _currTime);

	std::vector<std::pair<irr::core::vector2d<double>, double>> getPattern(std::vector<Hitobject*>* _hitobjects, int _num, double _interval, double _time, bool _skipSliders = false);
	int getButtonState(int _prevKey, int _currKey);
}

#endif