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
	int FindHitobjectAt(std::vector<Hitobject*>& _hitobjects, long _time, bool _dir = false);
	bool isHitobjectAt(std::vector<Hitobject*>& _hitobjects, long _prevTime, long _currTime);

	std::vector<osu::TIMING> getPattern(std::vector<Hitobject*>& _hitobjects, int _num, double _interval, long _time, bool _skipSliders = false, bool _newHitobject = false);
	osu::TIMING getNextTickPoint(std::vector<Hitobject*>& _hitobjects, long* _time);
	
	int getButtonState(int _prevKey, int _currKey);
}

#endif