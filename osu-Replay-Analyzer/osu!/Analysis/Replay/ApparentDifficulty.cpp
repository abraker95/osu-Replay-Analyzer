#include "ApparentDifficulty.h"

#include "../../osu_standard.h"
#include "../../osu_mania.h"

#include "../AnalysisStruct.h"

Analyzer_ApperantDifficulty::Analyzer_ApperantDifficulty() : Analyzer("Apperant Difficulty (%/log)") {}
Analyzer_ApperantDifficulty::~Analyzer_ApperantDifficulty() {}

void Analyzer_ApperantDifficulty::AnalyzeStd(Play* _play)
{
	
}

void Analyzer_ApperantDifficulty::AnalyzeCatch(Play* _play)
{

}

void Analyzer_ApperantDifficulty::AnalyzeTaiko(Play* _play)
{

}

void Analyzer_ApperantDifficulty::AnalyzeMania(Play* _play)
{
	osu::TIMING timing;
		timing.key = 0;

	const double DEVIATION_THRESHOLD = 5; // ms
	double currDiff = 1.0;

	Analyzer* analyzer = AnalysisStruct::beatmapAnalysis.getAnalyzer("Tap Deviation (ms)");
	if (analyzer == nullptr) return;

	int numPoints = analyzer->getData()->size();
	for (osu::TIMING tapDeviation : *(analyzer->getData()))
	{
		// (log(deviation + 1.0) + 1.0)

		double deviation = ABS(tapDeviation.data);
		if (deviation > DEVIATION_THRESHOLD) currDiff += (deviation + 2.0) / currDiff;		// Higher diff
		else								 currDiff -= currDiff / (deviation + 2.0);		// Lower diff

		timing.data = currDiff;
		timing.time = tapDeviation.time;

		data.push_back(timing);
	}
}

void Analyzer_ApperantDifficulty::AnalyzeDodge(Play* _play)
{

}