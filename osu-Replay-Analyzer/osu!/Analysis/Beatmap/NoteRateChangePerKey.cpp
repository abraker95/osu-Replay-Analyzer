#include "NoteRateChangePerKey.h"
#include "../AnalysisStruct.h"

#include "../../osu_standard.h"
#include "../../osu_mania.h"

Analyzer_NoteRateChangePerKey::Analyzer_NoteRateChangePerKey() : Analyzer("notes/s^2 per key") {}
Analyzer_NoteRateChangePerKey::~Analyzer_NoteRateChangePerKey() {}

void Analyzer_NoteRateChangePerKey::AnalyzeStd(Play* _play)
{
	Analyzer* analyzer = AnalysisStruct::beatmapAnalysis.getAnalyzer("note rate (notes/s)");
	std::vector<osu::TIMING>& noteRates = *(analyzer->getData());
	if (noteRates.size() == 0) return;

	int KEYS = _play->beatmap->getMods().getCS();

	osu::TIMING timing;
	timing.data = 0;

	for (int i = 1; i < noteRates.size(); i++)
	{
		timing.time = noteRates[i].time;
		timing.data = noteRates[i].data - noteRates[i - 1].data;
		timing.key = noteRates[i].key;

		data.push_back(timing);
	}
}

void Analyzer_NoteRateChangePerKey::AnalyzeCatch(Play* _play){}

void Analyzer_NoteRateChangePerKey::AnalyzeTaiko(Play* _play){}

void Analyzer_NoteRateChangePerKey::AnalyzeMania(Play* _play)
{
	Analyzer* analyzer = AnalysisStruct::beatmapAnalysis.getAnalyzer("max note rate for key (notes/s)");
	std::vector<osu::TIMING>& noteRates = *(analyzer->getData());
	if (noteRates.size() == 0) return;

	int KEYS = _play->beatmap->getMods().getCS();

	osu::TIMING timing;
		timing.data = 0;

	for(int i=1; i < noteRates.size(); i++)
	{
		timing.time = noteRates[i].time;
		timing.data = noteRates[i].data - noteRates[i - 1].data;
		timing.key = noteRates[i].key;

		data.push_back(timing);
	}
}

void Analyzer_NoteRateChangePerKey::AnalyzeDodge(Play* _play)
{

}