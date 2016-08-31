#include "AimScore.h"

#include "../../osu_standard.h"
#include "../../../utils/mathUtils.h"

#include "../AnalysisStruct.h"
#include "../Replay/aim-deviation.h"

Analyzer_AimScore::Analyzer_AimScore() : Analyzer("Aim Score (/M)"){}
Analyzer_AimScore::~Analyzer_AimScore(){}

void Analyzer_AimScore::AnalyzeStd(Play* _play)
{
	osu::TIMING timing;
		timing.key = 0;

	Analyzer* analyzer = AnalysisStruct::beatmapAnalysis.getAnalyzer("Aim Deviation (px)");
	if (analyzer == nullptr) return;

	int numPoints = analyzer->getData()->size();
	for (osu::TIMING aimDeviation : *(analyzer->getData()))
	{
		timing.data = 1000000.0 * (Guassian(aimDeviation.data, 15.0)/(double)numPoints);  // inidivual score
		timing.key += timing.data;														  // cumalative score
		timing.pos = aimDeviation.pos;
		timing.time = aimDeviation.time;

		data.push_back(timing);
	}
}

void Analyzer_AimScore::AnalyzeCatch(Play* _play)
{

}

void Analyzer_AimScore::AnalyzeTaiko(Play* _play)
{

}

void Analyzer_AimScore::AnalyzeMania(Play* _play)
{

}

void Analyzer_AimScore::AnalyzeDodge(Play* _play)
{

}