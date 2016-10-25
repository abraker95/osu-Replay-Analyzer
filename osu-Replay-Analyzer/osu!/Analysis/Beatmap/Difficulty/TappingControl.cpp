#include "TappingControl.h"
#include "../../AnalysisStruct.h"

#include "../../../osu_standard.h"
#include "../../../osu_mania.h"

Analyzer_TappingControl::Analyzer_TappingControl() : Analyzer("Tapping Control") {}
Analyzer_TappingControl::~Analyzer_TappingControl() {}

void Analyzer_TappingControl::AnalyzeStd(Play* _play) 
{
	Analyzer* analyzerRate = AnalysisStruct::beatmapAnalysis.getAnalyzer("notes/s^2 per key");
	Analyzer* analyzerChng = AnalysisStruct::beatmapAnalysis.getAnalyzer("note rate (notes/s)");

	std::vector<osu::TIMING>& noteRateChanges = *(analyzerRate->getData());
	std::vector<osu::TIMING>& noteRates = *(analyzerChng->getData());

	if (noteRateChanges.size() == 0) return;
	if (noteRates.size() == 0) return;

	osu::TIMING timing;
		timing.data = 0;
		timing.key = 0;

	double rawControl = 0.0;
	for (int i = 1; i < noteRateChanges.size(); i++)
	{
		if (noteRateChanges[i].data == INFINITY) continue;
		int key = noteRateChanges[i].key;

		osu::TIMING noteRate = noteRates[osu::FindTimingAt(noteRates, noteRateChanges[i].time)];

		const double MAX_INFLUENCE = 0.9;
		double influencers = noteRate.data /* pow(noteRateChanges[i - 1].data / 10.0, 10)*/;
		double influence = MAX_INFLUENCE * (-exp(-influencers / 20.0) + 1);
		
		if (noteRateChanges[i].data > 0) rawControl += 0.9 * noteRateChanges[i].data;
		if (noteRateChanges[i].data < 0) rawControl -= 0.10*rawControl;
		else							 rawControl -= 0.25*rawControl;

		timing.data = rawControl;
		timing.key = MAX(noteRateChanges[i].key, rawControl);
		timing.pos = noteRateChanges[i].pos;
		timing.time = noteRateChanges[i].time;

		data.push_back(timing);
	}
}

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

	std::vector<double> rawControls;
		rawControls.resize(KEYS);

	for (osu::TIMING noteRateChange : noteRateChanges)
	{
		if (noteRateChange.data == INFINITY) continue;
		int key = noteRateChange.key;

		osu::TIMING noteRate = noteRates[osu::FindTimingAt(noteRates, noteRateChange.time)];

		if (noteRateChange.data > 0)	  rawControls[key] += 0.90*noteRateChange.data*noteRate.data;
		else if(noteRateChange.data < 0) rawControls[key] -= 0.70*rawControls[key] * (1.0/noteRate.data);
		else							 rawControls[key] -= 0.50*rawControls[key] * (1.0/noteRate.data);

		double maxControl = INT_MIN;
		for (double rawControl : rawControls)
		{
			if (maxControl < rawControl)
				maxControl = rawControl;
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