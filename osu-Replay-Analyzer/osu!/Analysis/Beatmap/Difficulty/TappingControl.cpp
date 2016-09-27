#include "TappingControl.h"
#include "../../AnalysisStruct.h"

#include "../../../osu_standard.h"
#include "../../../osu_mania.h"

Analyzer_TappingControl::Analyzer_TappingControl() : Analyzer("Tapping Control") {}
Analyzer_TappingControl::~Analyzer_TappingControl() {}

void Analyzer_TappingControl::AnalyzeStd(Play* _play) {}

void Analyzer_TappingControl::AnalyzeCatch(Play* _play) {}

void Analyzer_TappingControl::AnalyzeTaiko(Play* _play) {}

void Analyzer_TappingControl::AnalyzeMania(Play* _play)
{
	Analyzer* analyzerRate = AnalysisStruct::beatmapAnalysis.getAnalyzer("notes/s^2 per key");
	Analyzer* analyzerChng = AnalysisStruct::beatmapAnalysis.getAnalyzer("max note rate for key (notes/s)");
	
	std::vector<osu::TIMING>& noteRateChanges = *(analyzerRate->getData());
	std::vector<osu::TIMING>& noteRates = *(analyzerChng->getData());

	if (noteRateChanges.size() == 0) return;
	if (noteRates.size() == 0) return;

	int KEYS = _play->beatmap->getMods().getCS();

	osu::TIMING timing;
		timing.data = 0;

	std::vector<double> rawControl;
		rawControl.resize(KEYS);

	for (osu::TIMING noteRateChange : noteRateChanges)
	{
		if (noteRateChange.data == INFINITY) continue;
		int key = noteRateChange.key;

		osu::TIMING noteRate = noteRates[osu::FindTimingAt(noteRates, noteRateChange.time)];

		if (noteRateChange.data > 0)	 rawControl[key] += 0.90*noteRateChange.data*noteRate.data;
		else if(noteRateChange.data < 0) rawControl[key] -= 0.70*rawControl[key] * (1.0/noteRate.data);
		else							 rawControl[key] -= 0.50*rawControl[key] * (1.0/noteRate.data);

		double maxControl = INT_MIN;
		for (double rawEndurance : rawControl)
		{
			if (maxControl < rawEndurance)
				maxControl = rawEndurance;
		}

		timing.data = maxControl;
		timing.key = noteRateChange.key;
		timing.pos = noteRateChange.pos;
		timing.time = noteRateChange.time;

		data.push_back(timing);
	}
}

void Analyzer_TappingControl::AnalyzeDodge(Play* _play)
{

}