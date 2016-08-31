#include "NoteRatePerKeyPrecise.h"
#include "../../../utils/geometry.h"

#include "../../osu_standard.h"
#include "../../osu_mania.h"

#include "../AnalysisStruct.h"

Analyzer_NoteRatePerKeyPrec::Analyzer_NoteRatePerKeyPrec() : Analyzer("max note rate for key (notes/s)") {}
Analyzer_NoteRatePerKeyPrec::~Analyzer_NoteRatePerKeyPrec() {}

/// \TODO: Maybe detect stream vs single tap speeds? Players can technically do a bit more than ~7 notes/s for single tap
/// Which is 4.5 note/s alternating. Just figure out which one would go to which
void Analyzer_NoteRatePerKeyPrec::AnalyzeStd(Play* _play){}

void Analyzer_NoteRatePerKeyPrec::AnalyzeCatch(Play* _play){}

void Analyzer_NoteRatePerKeyPrec::AnalyzeTaiko(Play* _play){}

void Analyzer_NoteRatePerKeyPrec::AnalyzeMania(Play* _play)
{
	int KEYS = _play->beatmap->getMods().getCS();
	std::vector<Hitobject*>& hitobjects = _play->beatmap->getHitobjects();
	osu::TIMING timing;
		timing.data = 0;
		data.push_back({ hitobjects[0]->getTime() - 1, 0});

	Analyzer* analyzer = AnalysisStruct::beatmapAnalysis.getAnalyzer("note rate (notes/s)");
	if (analyzer == nullptr) return;

	std::vector<osu::TIMING>& noteRates = *(analyzer->getData());
	for (int i=0; i<noteRates.size(); i++)
	{
		long currTime = noteRates[i].time;

		long minInterval = INT_MAX;
		int recKey = -1;

		for (int key = 0; key < KEYS; key++)
		{
			int currI = OSUMANIA::FindHitobjectAt(_play, currTime, key, false);
			int nextI = OSUMANIA::getNextIndexOnColumn(hitobjects, key, KEYS, currI);

			if (currI == OSUMANIA::MANIA_END)	continue;
			if (nextI == OSUMANIA::MANIA_END)	continue;

			long interval = hitobjects[nextI]->getTime() - hitobjects[currI]->getTime();
			if (minInterval > interval)
			{
				minInterval = interval;
				recKey = key;
			}
		}

		if (minInterval == INT_MAX) continue;

		timing.time = currTime;
		timing.data = 1000.0 / (double)minInterval;
		timing.key = recKey;

		data.push_back(timing);
	}
}

void Analyzer_NoteRatePerKeyPrec::AnalyzeDodge(Play* _play)
{

}