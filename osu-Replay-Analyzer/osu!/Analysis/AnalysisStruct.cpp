#include "AnalysisStruct.h"

Analysis AnalysisStruct::beatmapAnalysis;
Analysis AnalysisStruct::replayAnalysis;
Analysis AnalysisStruct::scorepAnalysis;

AnalysisStruct::AnalysisStruct()
{
	beatmapAnalysis.AddAnalyzer(new Analyzer_Velocity());
	beatmapAnalysis.AddAnalyzer(new Analyzer_DistSize());
	beatmapAnalysis.AddAnalyzer(new Analyzer_NoteRate());
	beatmapAnalysis.AddAnalyzer(new Analyzer_TapEventIntervals());
	///beatmapAnalysis.AddAnalyzer(new Analyzer_Overlaps());
	//beatmapAnalysis.AddAnalyzer(new Analyzer_NoteRatePerKey());
	beatmapAnalysis.AddAnalyzer(new Analyzer_NoteRatePerKeyPrec());

	beatmapAnalysis.AddAnalyzer(new Analyzer_TapDeviation());
	beatmapAnalysis.AddAnalyzer(new Analyzer_AimDeviation());

	///beatmapAnalysis.AddAnalyzer(new Analyzer_Reading());

	beatmapAnalysis.AddAnalyzer(new Analyzer_AimScore());
	beatmapAnalysis.AddAnalyzer(new Analyzer_TapScore());
}

static AnalysisStruct analysisStruct;