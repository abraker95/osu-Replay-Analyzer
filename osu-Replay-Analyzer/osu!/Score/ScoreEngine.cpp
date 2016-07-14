#include "ScoreEngine.h"

#include "osu-std/score.h"
#include "osu-catch/score.h"
#include "osu-taiko/score.h"
#include "osu-mania/score.h"
#include "osu-dodge/score.h"

#include <math.h>

ScoreEngine::ScoreEngine(Play* _play)
{
	genAccTimings(_play);
}

void ScoreEngine::genAccTimings(Play* _play)
{
	if (!_play->beatmap->isValid())	return;

	GAMEMODE gamemode = _play->beatmap->getGamemode();
	switch (gamemode)
	{
		case OSU_STANDARD:
			OSUSTANDARD::genAccTimings(_play);
			/// OSUSTANDARD::genPrcTimings(_play);   \TODO
			break;
	
		case OSU_TAIKO:
			OSUTAIKO::genAccTimings(_play);
			break;
		
		case OSU_CATCH:
			OSUCATCH::genAccTimings(_play);
			break;
	
		case OSU_MANIA:
			OSUMANIA::genAccTimings(_play);
			break;
	
		case OSU_DODGE:
			OSUDODGE::genAccTimings(_play);
			break;
	
		default:
			break;
	}
}