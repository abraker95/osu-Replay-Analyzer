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

	std::vector<osu::TIMING> getPattern(Play* _play, int _num, long _time, bool _skipSliders = false);
	osu::TIMING getNextTickPoint(std::vector<Hitobject*>& _hitobjects, long _time, long* _timeItr = nullptr);
	osu::TIMING FindClosest(Hitobject& _h1, Hitobject& _h2, int itr = 1);

	int getButtonState(int _prevKey, int _currKey);
	int getButtonState(int _prevKey, int _currKey, int _key);

	std::pair<int, int> getIndicesVisibleAt(Play* _play, int _time, double _opacity);
	std::pair<int, int> getVisiblityTimes(Hitobject& _hitobject, double _AR, bool _hidden, double _opacityStart, double _opacityEnd);
	bool isVisibleAt(Hitobject& _hitobject, int _time, double _AR, bool _hidden);
	int getNumHitobjectsVisibleAt(Play* _play, int _index, double _opacity);
	double getOpacityAt(Hitobject& _hitobject, int _time, double _AR, bool _hidden);
}

#endif