#include "AutoHackSync.h"

#include "../../osu_standard.h"
#include "../../osu_mania.h"

#include "../AnalysisStruct.h"

Analyzer_AutoHackSync::Analyzer_AutoHackSync() : Analyzer("Auto Hack Synchronization") {}
Analyzer_AutoHackSync::~Analyzer_AutoHackSync() {}

void Analyzer_AutoHackSync::AnalyzeStd(Play* _play)
{

}

void Analyzer_AutoHackSync::AnalyzeCatch(Play* _play)
{

}

void Analyzer_AutoHackSync::AnalyzeTaiko(Play* _play)
{

}


void Analyzer_AutoHackSync::AnalyzeMania(Play* _play)
{
	Analyzer* analyzer = AnalysisStruct::beatmapAnalysis.getAnalyzer("Tap Deviation (ms)");
	if (analyzer == nullptr) return;

	std::vector<osu::TIMING>& tapDeviations = *(analyzer->getData());
	if (tapDeviations.size() == 0) return;

	osu::TIMING timing;
		timing.data = 0;

	int currTime = tapDeviations[0].time;
	int notes = 0;
	long minTime = INT_MAX, maxTime = INT_MIN;

	/// \TODO: Trigger cheat detect if 0 for extended period of time(s)

	// Records the range between presses on a row of notes
	for (osu::TIMING tapDeviation : tapDeviations)
	{
		long noteTime = tapDeviation.time;
		if (!BTWN(noteTime-1, currTime, noteTime+1)) // Do btwn to give some wiggle room due to rounding errors
		{
			// Can't figure if it's is only one note
			if (notes > 1)
			{
				timing.data = maxTime - minTime;
				timing.time = tapDeviation.time;
				data.push_back(timing);
			}

			notes = 1;
			currTime = noteTime;
			minTime = INT_MAX, maxTime = INT_MIN;
		}
		else
		{
			minTime = MIN(tapDeviation.time + tapDeviation.data, minTime);
			maxTime = MAX(tapDeviation.time + tapDeviation.data, maxTime);
			notes++;
		}
	}
}

void Analyzer_AutoHackSync::AnalyzeDodge(Play* _play)
{

}