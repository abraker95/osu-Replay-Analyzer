#include "NoteRatePerKey.h"
#include "../../../utils/geometry.h"

#include "../../osu_standard.h"
#include "../../osu_mania.h"

Analyzer_NoteRatePerKey::Analyzer_NoteRatePerKey() : Analyzer("note rate/key") {}
Analyzer_NoteRatePerKey::~Analyzer_NoteRatePerKey() {}

/// \TODO: Maybe detect stream vs single tap speeds? Players can technically do a bit more than ~7 notes/s for single tap
/// Which is 4.5 note/s alternating. Just figure out which one would go to which
void Analyzer_NoteRatePerKey::AnalyzeStd(Play* _play){}

void Analyzer_NoteRatePerKey::AnalyzeCatch(Play* _play){}

void Analyzer_NoteRatePerKey::AnalyzeTaiko(Play* _play){}

void Analyzer_NoteRatePerKey::AnalyzeMania(Play* _play)
{
	int KEYS = _play->beatmap->getMods().getCS();

	std::vector<Hitobject*>& hitobjects = _play->beatmap->getHitobjects();
	osu::TIMING timing;
		timing.data = 0;

	std::vector<int> i, previ;
		i.resize(KEYS);
		previ.resize(KEYS);
	double delay = 10.0;

	for (int ms = 0; ms < hitobjects[hitobjects.size() - 1]->getTime(); ms += delay)
	{
		double maxCount = 0;

		// operate per-key
		for (int key = 0; key < KEYS; key++)
		{
			if (i[key] == OSUMANIA::MANIA_END) continue;
			while (hitobjects[i[key]]->getEndTime() < ms)
			{
				OSUMANIA::getNextNoteOnColumn(_play, key, &i[key]);
				if (i[key] == OSUMANIA::MANIA_END) break;
			}
			if(i[key] == OSUMANIA::MANIA_END) continue;

			// Out of bound check
			if (!BTWN(1, i[key], hitobjects.size() - 1)) continue;

			// Count num objects on column from 1 second ago till now
			int count = 0, tempi = previ[key];
			while(hitobjects[tempi]->getTime() != hitobjects[i[key]]->getTime())
			{
				OSUMANIA::getNextNoteOnColumn(_play, key, &tempi);
				count++;
			}

			// previ - i needs to be at least 1 second apart
			while (hitobjects[i[key]]->getTime() - hitobjects[previ[key]]->getTime() > 1000.0)
			{
				if (previ[key] < hitobjects.size() - 1) previ[key]++;
				else break;
			}

			// check for largest interval (highest density)
			if (count > maxCount)
			{
				timing.data = maxCount = count;
				timing.pos = vector2df(OSUMANIA::getKey(hitobjects[i[key]]->getPos().X, KEYS), 0);
			}
		}

		timing.press = false;
		timing.time = ms;

		data.push_back(timing);
	}
}

void Analyzer_NoteRatePerKey::AnalyzeDodge(Play* _play)
{

}