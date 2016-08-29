#include "NoteRatePerKeyPrecise.h"
#include "../../../utils/geometry.h"

#include "../../osu_standard.h"
#include "../../osu_mania.h"

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

	std::vector<int> i, previ, time;
		i.resize(KEYS);
		previ.resize(KEYS);
		time.resize(KEYS);

	// Setup the indices to have one in front of the other
	for (int key = 0; key < KEYS; key++)
	{
		OSUMANIA::getNextNoteOnColumn(_play, key, &i[key]);
		OSUMANIA::getNextNoteOnColumn(_play, key, &i[key]);
		
		// Since starting index is 0, make sure we don't skip
		// the first note that might happen to be on the column
		if(OSUMANIA::getKey(hitobjects[0]->getPos().X, KEYS) != key)
			OSUMANIA::getNextNoteOnColumn(_play, key, &previ[key]);
	}

	while (true)
	{
		if (OSUMANIA::isAtEnd(KEYS, i, hitobjects)) break;

		// operate per-key
		for (int key = 0; key < KEYS; key++)
		{
			// Check to see if this column came to the end
			if (i[key] == OSUMANIA::MANIA_END) continue;

			// Check to see if the time was already recorded
			bool differentTime = false;
			time[key] = hitobjects[i[key]]->getTime();
			for (int key2 = 0; key2 < KEYS; key2++)
				differentTime |= (time[key2] == time[key]);

			// Recond timing info
			if (differentTime)
			{
				double timeInterval = time[key] - hitobjects[previ[key]]->getTime();
				
				// Irrlitch crashes when trying to convert inf -> str
				if (timeInterval == 0) timing.data = INT_MAX;
				else            	   timing.data = 1000.0/timeInterval;
			
				timing.pos = vector2df(OSUMANIA::getKey(hitobjects[i[key]]->getPos().X, KEYS), 0);
				timing.press = false;
				timing.time = time[key];

				data.push_back(timing);
			}

			// Skip if this one is ahead of at least another column
			bool get = OSUMANIA::isBehindOtherColumns(KEYS, key, i, hitobjects);
			if (get == true)
			{
				OSUMANIA::getNextNoteOnColumn(_play, key, &i[key]);
				OSUMANIA::getNextNoteOnColumn(_play, key, &previ[key]);
			}
			else
			{
				continue;
			}
		}
	}

	osu::SortByTime(data);
}

void Analyzer_NoteRatePerKeyPrec::AnalyzeDodge(Play* _play)
{

}