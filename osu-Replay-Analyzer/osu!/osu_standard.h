#ifndef OSU_STANDARD_H
#define OSU_STANDARD_H

#include <math.h>
#include <vector>
#include "Filestructure\Hitobject.h"
#include "Filestructure\SliderHitObject.h"

namespace OSUSTANDARD
{
	struct TIMING
	{
		long time;
		double data;
		int key;
		bool press;
	};

	int FindTimingAt(std::vector<TIMING>& _timings, double _time);
	int FindHitobjectAt(std::vector<Hitobject*>* _hitobjects, double _time);
	std::vector<std::pair<irr::core::vector2d<double>, double>> getPattern(std::vector<Hitobject*>* _hitobjects, int _num, double _interval, double _time, bool _skipSliders = false);
}

#endif