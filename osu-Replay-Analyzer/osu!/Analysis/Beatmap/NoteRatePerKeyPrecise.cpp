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
	Database<Hitobject>& hitobjects = _play->beatmap->getHitobjects();
	osu::TIMING timing;
		timing.data = 0;
		timing.time = hitobjects[0]->getTime() - 1;
	
	data.push_back(timing);

	std::vector<int> prevIs, currIs;
		prevIs.resize(KEYS);
		currIs.resize(KEYS);

	for(int& prevI : prevIs) prevI = -1;
	for(int& currI : currIs) currI = -1;

	std::vector<int> currTimes, prevTimes;
		currTimes.resize(KEYS);
		prevTimes.resize(KEYS);

	for (int& currTime : currTimes) currTime = OSUMANIA::MANIA_END;
	for (int& prevTime : prevTimes) prevTime = OSUMANIA::MANIA_END;

	long minInterval = INT_MAX;
	int recKey = -1;

	for (int i = 0; i < hitobjects.size(); i++)
	{
		// Get current key
		int key = OSUMANIA::getKey(hitobjects[i]->getPos().X, KEYS);

		prevTimes[key] = currTimes[key];
		currTimes[key] = hitobjects[i]->getTime();

		// Record/reset only if the time has changed
		if (currTimes[key] != prevTimes[key])
		{
			// Make sure we found the min interval before recording it
			if (minInterval != INT_MAX)
			{
				timing.time = prevTimes[recKey];
				timing.data = 1000.0 / (double)minInterval;
				timing.key = recKey;

				data.push_back(timing);
			}

			minInterval = INT_MAX;
			recKey = -1;
		}

		// Make sure we are not at beginning nor end
		if (prevTimes[key] == OSUMANIA::MANIA_END || currTimes[key] == OSUMANIA::MANIA_END) continue;

		// Calc interval
		long interval = currTimes[key] - prevTimes[key];

		// Record if it is smaller than already recorded interval
		if (minInterval > interval)
		{
			minInterval = interval;
			recKey = key;
		}
	}

	osu::SortByTime(data);
}

void Analyzer_NoteRatePerKeyPrec::AnalyzeDodge(Play* _play)
{

}