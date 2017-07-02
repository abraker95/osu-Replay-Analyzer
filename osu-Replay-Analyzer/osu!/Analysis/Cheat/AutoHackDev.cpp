#include "AutoHackDev.h"

#include "../../osu_standard.h"
#include "../../osu_mania.h"

#include "../AnalysisStruct.h"

Analyzer_AutoHackDev::Analyzer_AutoHackDev() : Analyzer("Auto Hack Deviation") {}
Analyzer_AutoHackDev::~Analyzer_AutoHackDev() {}

void Analyzer_AutoHackDev::AnalyzeStd(Play* _play)
{

}

void Analyzer_AutoHackDev::AnalyzeCatch(Play* _play)
{

}

void Analyzer_AutoHackDev::AnalyzeTaiko(Play* _play)
{

}


void Analyzer_AutoHackDev::AnalyzeMania(Play* _play)
{
	Analyzer* analyzer = AnalysisStruct::beatmapAnalysis.getAnalyzer("Tap Deviation (ms)");
	if (analyzer == nullptr) return;

	std::vector<osu::TIMING>& tapDeviations = *(analyzer->getData());
	if (tapDeviations.size() == 0) return;

	osu::TIMING timing;
		timing.data = 0;

	const double w = 0.1;
	double avg = 0;
	double variance = 0; 

	/// \TODO: Trigger cheat detect if +/-0.2 for extended period of time(s)

	for (osu::TIMING tapDeviation : tapDeviations)
	{
		avg = w*tapDeviation.data + (1 - w)*avg;
		variance = w*ABS(tapDeviation.data - avg) + (1 - w)*variance;

		timing.data = variance;
		timing.time = tapDeviation.time;
		data.push_back(timing);
	}
}

void Analyzer_AutoHackDev::AnalyzeDodge(Play* _play)
{

}