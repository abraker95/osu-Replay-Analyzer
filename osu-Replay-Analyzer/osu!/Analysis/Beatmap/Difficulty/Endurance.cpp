#include "Endurance.h"

#include "../../../osu_standard.h"
#include "../../../osu_mania.h"

#include "../../../osuCalc.h"

#include "../../AnalysisStruct.h"

#include <map>

Analyzer_Endurance::Analyzer_Endurance() : Analyzer("endurance diff") {}
Analyzer_Endurance::~Analyzer_Endurance() {}

void Analyzer_Endurance::AnalyzeStd(Play* _play)
{
	
}

void Analyzer_Endurance::AnalyzeCatch(Play* _play)
{

}

void Analyzer_Endurance::AnalyzeTaiko(Play* _play)
{

}

void Analyzer_Endurance::AnalyzeMania(Play* _play)
{
	Analyzer* analyzer = AnalysisStruct::beatmapAnalysis.getAnalyzer("max note rate for key (notes/s)");
	std::vector<osu::TIMING>& noteRates = *(analyzer->getData());
	if (noteRates.size() == 0) return;

	osu::TIMING timing;
		timing.data = 0;

	const double staminaThreshold = 5.0, buildupThreshold = 2.0;  // taps/sec

	int KEYS = _play->beatmap->getMods().getCS();

	std::vector<double> rawEndurances, notes;
		rawEndurances.resize(KEYS);
		notes.resize(KEYS);

	for (osu::TIMING noteRate : noteRates)
	{
		if (noteRate.data == INFINITY) continue;
		int key = noteRate.key;

		if (noteRate.data >= staminaThreshold)	rawEndurances[key] += noteRate.data;
		else									rawEndurances[key] -= rawEndurances[key] * 0.1;

		if (noteRate.data >= buildupThreshold)	notes[key]++;
		else									notes[key] -= notes[key]*0.01;

		rawEndurances[key] += log10(notes[key] + 1);

		double maxEndure = INT_MIN;
		for (double rawEndurance : rawEndurances)
		{
			if (maxEndure < rawEndurance)
				maxEndure = rawEndurance;
		}

		timing.data = maxEndure;
		timing.pos = noteRate.pos;
		timing.time = noteRate.time;

		data.push_back(timing);
	}
}

void Analyzer_Endurance::AnalyzeDodge(Play* _play)
{

}