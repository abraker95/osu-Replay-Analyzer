#ifndef SCORENGINE_H
#define SCORENGINE_H

#include <vector>

#include "../Filestructure/Play.h"
#include "../osu_mania.h"

class ScoreEngine
{
	public:
		ScoreEngine();
		void genAccTimings(Play* _play);
};

#endif