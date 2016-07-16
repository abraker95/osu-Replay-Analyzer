#ifndef OSU_STANDARD_H
#define OSU_STANDARD_H

#include <math.h>
#include <vector>
#include "Filestructure\Hitobject.h"

namespace OSUSTANDARD
{
	struct TIMING
	{
		long time;
		double data;
		int key;
		bool press;
	};

	int FindHitobjectAt(std::vector<Hitobject>* _hitobjects, double _time);
}

#endif