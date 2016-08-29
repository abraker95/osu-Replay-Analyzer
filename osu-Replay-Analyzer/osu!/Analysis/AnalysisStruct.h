#ifndef ANALYSISSTRUCT_H
#define ANALYSISSTRUCT_H

#include "Analysis.h"

#include "Beatmap\Velocity.h"
#include "Beatmap\NoteRate.h"
#include "Beatmap\NoteRatePerKey.h"
#include "Beatmap\NoteRatePerKeyPrecise.h"
#include "Beatmap\Overlaps.h"
#include "Beatmap\Distance-size.h"
#include "Beatmap\TapEventIntervals.h"

#include "Beatmap\Difficulty\Reading.h"

#include "Replay\aim-deviation.h"
#include "Replay\tap-deviation.h"

#include "Score\AimScore.h"
#include "Score\TapScore.h"

struct AnalysisStruct
{
	static Analysis beatmapAnalysis;
	static Analysis replayAnalysis;
	static Analysis scorepAnalysis;

	AnalysisStruct();
};

#endif