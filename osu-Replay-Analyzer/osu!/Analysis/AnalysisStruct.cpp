#include "AnalysisStruct.h"

#include "Beatmap\Velocity.h"
#include "Beatmap\NoteRate.h"
#include "Beatmap\NoteRatePerKeyPrecise.h"
#include "Beatmap\NoteRateChangePerKey.h"
#include "Beatmap\Overlaps.h"
#include "Beatmap\Distance-size.h"
#include "Beatmap\TapEventIntervals.h"
#include "Beatmap\NumNotesVisible.h"
#include "Beatmap\PathPerpendicularity.h"
#include "Beatmap\SpatialChaos.h"
#include "Beatmap\ReadingIntensity.h"
#include "Beatmap\NotesAR.h"


#include "Beatmap\Difficulty\Reading.h"
#include "Beatmap\Difficulty\Endurance.h"
#include "Beatmap\Difficulty\TappingControl.h"

#include "Replay\PressReleaseIntervals.h"
#include "Replay\tap-deviation.h"
#include "Replay\aim-deviation.h"
#include "Replay\ApparentDifficulty.h"

#include "Score\AimScore.h"
#include "Score\TapScore.h"

Analysis AnalysisStruct::beatmapAnalysis;
Analysis AnalysisStruct::replayAnalysis;
Analysis AnalysisStruct::scorepAnalysis;

AnalysisStruct::AnalysisStruct()
{
	// **************************
	//     [RAW BEATMAP DATA]
	// **************************
	beatmapAnalysis.AddAnalyzer(new Analyzer_Velocity());
	beatmapAnalysis.AddAnalyzer(new Analyzer_DistSize());
	beatmapAnalysis.AddAnalyzer(new Analyzer_NoteRate());
	beatmapAnalysis.AddAnalyzer(new Analyzer_TapEventIntervals());
	beatmapAnalysis.AddAnalyzer(new Analyzer_NoteRatePerKeyPrec());
	beatmapAnalysis.AddAnalyzer(new Analyzer_NoteRateChangePerKey());
	beatmapAnalysis.AddAnalyzer(new Analyzer_NumNotesVisible());
	beatmapAnalysis.AddAnalyzer(new Analyzer_PathPerpendicularity());
	beatmapAnalysis.AddAnalyzer(new Analyzer_NotesAR());
	beatmapAnalysis.AddAnalyzer(new Analyzer_SpatialChaos());
	beatmapAnalysis.AddAnalyzer(new Analyzer_ReadingIntensity());
	beatmapAnalysis.AddAnalyzer(new Analyzer_Overlaps());


	

	// **************************
	//       [DIFF DATA]
	// **************************
	beatmapAnalysis.AddAnalyzer(new Analyzer_Endurance());
	beatmapAnalysis.AddAnalyzer(new Analyzer_TappingControl());
	beatmapAnalysis.AddAnalyzer(new Analyzer_Reading());


	// **************************
	//     [RAW REPLAY DATA]
	// **************************
	beatmapAnalysis.AddAnalyzer(new Analyzer_PressReleaseIntervals());
	beatmapAnalysis.AddAnalyzer(new Analyzer_TapDeviation());
	beatmapAnalysis.AddAnalyzer(new Analyzer_AimDeviation());
	beatmapAnalysis.AddAnalyzer(new Analyzer_ApperantDifficulty());



	// **************************
	//       [SCORE DATA]
	// **************************
	beatmapAnalysis.AddAnalyzer(new Analyzer_AimScore());
	beatmapAnalysis.AddAnalyzer(new Analyzer_TapScore());



	// **************************
	//       [CHEAT DATA]
	// **************************
}

static AnalysisStruct analysisStruct;