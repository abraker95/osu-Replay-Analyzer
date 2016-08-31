#ifndef ANALYSISSTRUCT_H
#define ANALYSISSTRUCT_H

#include "Analysis.h"

struct AnalysisStruct
{
	static Analysis beatmapAnalysis;
	static Analysis replayAnalysis;
	static Analysis scorepAnalysis;

	AnalysisStruct();
};

#endif