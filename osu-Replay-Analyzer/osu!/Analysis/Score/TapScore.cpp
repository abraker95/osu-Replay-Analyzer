#include "TapScore.h"

#include "../../osu_standard.h"
#include "../../../utils/mathUtils.h"

#include "../AnalysisStruct.h"
#include "../Replay/tap-deviation.h"

Analyzer_TapScore::Analyzer_TapScore() : Analyzer("Tap Score (/M)"){}
Analyzer_TapScore::~Analyzer_TapScore(){}

void Analyzer_TapScore::AnalyzeStd(Play* _play)
{
	osu::TIMING timing;
		timing.key = 0;

	std::vector<osu::TIMING>* tapDeviations = AnalysisStruct::beatmapAnalysis.getAnalyzer("tap deviation (ms)")->getData();
	if (tapDeviations == nullptr) return;

	int numPoints = tapDeviations->size();
	for (osu::TIMING TapDeviation : *tapDeviations)
	{
		timing.data = 1000000.0 * (Guassian(TapDeviation.data, 40.0)/(double)numPoints);  // inidivual score
		timing.key += timing.data;														  // cumalative score
		timing.pos = TapDeviation.pos;
		timing.time = TapDeviation.time;

		data.push_back(timing);
	}
}

void Analyzer_TapScore::AnalyzeCatch(Play* _play)
{

}

void Analyzer_TapScore::AnalyzeTaiko(Play* _play)
{

}

void Analyzer_TapScore::AnalyzeMania(Play* _play)
{
	osu::TIMING timing;
		timing.key = 0;

	Analyzer* analyzer = AnalysisStruct::beatmapAnalysis.getAnalyzer("Tap Deviation (ms)");
	if (analyzer == nullptr) return;

	std::vector<osu::TIMING>* tapDeviations = analyzer->getData();
	int numPoints = tapDeviations->size();

	for (osu::TIMING TapDeviation : *tapDeviations)
	{
		timing.data = 1000000.0 * (Guassian(TapDeviation.data, 40.0) / (double)numPoints);  // inidivual score
		timing.key += timing.data;														    // cumalative score
		timing.pos = TapDeviation.pos;
		timing.time = TapDeviation.time;

		data.push_back(timing);
	}
}

void Analyzer_TapScore::AnalyzeDodge(Play* _play)
{

}