#include "SpeedHack.h"

#include "../../osu_standard.h"
#include "../../osu_mania.h"

#include "../AnalysisStruct.h"

Analyzer_SpeedHack::Analyzer_SpeedHack() : Analyzer("Speed Hack Analysis") {}
Analyzer_SpeedHack::~Analyzer_SpeedHack() {}

void Analyzer_SpeedHack::AnalyzeStd(Play* _play)
{
	
}

void Analyzer_SpeedHack::AnalyzeCatch(Play* _play)
{

}

void Analyzer_SpeedHack::AnalyzeTaiko(Play* _play)
{

}


/// \TODO: Fix bug where there are no timings at last set of notes
void Analyzer_SpeedHack::AnalyzeMania(Play* _play)
{
	Analyzer* analyzer = AnalysisStruct::beatmapAnalysis.getAnalyzer("Press-Release Intervals (ms)");
	if (analyzer == nullptr) return;

	std::vector<osu::TIMING>& tapReleaseIntervals = *(analyzer->getData());
	if (tapReleaseIntervals.size() == 0) return;

	osu::TIMING timing;
		timing.data = 0;

	for (osu::TIMING tapReleaseIterval : tapReleaseIntervals)
	{
		double mag = 1.0 / MAX(0, tapReleaseIterval.data - 20.0);

		timing.data = MIN(100, 1.0 / MAX(0, tapReleaseIterval.data - 20.0));
		timing.time = tapReleaseIterval.time;
		data.push_back(timing);
	}
}

void Analyzer_SpeedHack::AnalyzeDodge(Play* _play)
{

}