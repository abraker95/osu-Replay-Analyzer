#include "NoteRate.h"


#include "../../osu_standard.h"
#include "../../osu_mania.h"

Analyzer_NoteRate::Analyzer_NoteRate() : Analyzer("note rate (notes/s)") {}
Analyzer_NoteRate::~Analyzer_NoteRate() {}

void Analyzer_NoteRate::AnalyzeStd(Play* _play)
{
	Database<Hitobject>& hitobjects = _play->beatmap->getHitobjects();
	osu::TIMING timing;
		timing.data = 0;

	for (int i = 1; i < hitobjects.size(); i++)
	{
		//int j = i;
		//while (j--) if (hitobjects[i]->getTime() - hitobjects[j]->getTime() >= 1000) break;

		timing.data = 1000.0/(hitobjects[i]->getTime() - hitobjects[i - 1]->getTime());
		timing.pos = hitobjects[i]->getPos();
		timing.key = i;
		timing.press = false;
		timing.time = hitobjects[i]->getTime();

		data.push_back(timing);
	}
}

void Analyzer_NoteRate::AnalyzeCatch(Play* _play)
{

}

void Analyzer_NoteRate::AnalyzeTaiko(Play* _play)
{

}

void Analyzer_NoteRate::AnalyzeMania(Play* _play)
{
	Database<Hitobject>& hitobjects = _play->beatmap->getHitobjects();
	int KEYS = _play->beatmap->getMods().getCS();
	
	long currTime = hitobjects[0]->getTime();
	std::vector<long> holdnoteEnds;
	holdnoteEnds.resize(KEYS);

	osu::TIMING timing;
		timing.data = 0;	// make the min be 0
		timing.time = currTime - 1;

	data.push_back(timing); 

	int noteCount = 0;

	for (int i = 0; i < hitobjects.size(); i++)
	{
		if (hitobjects[i]->isHitobjectLong())
			holdnoteEnds[OSUMANIA::getKey(hitobjects[i]->getPos().X, KEYS)] = hitobjects[i]->getEndTime();

		long hitobjectTime = hitobjects[i]->getTime();
		if (hitobjectTime > currTime)
		{
			noteCount++;
			if (i <= 1)
				noteCount--;

			// See if there are any hold notes at this time as well
			for (long holdnoteEnd : holdnoteEnds)
			{
				if (BTWN(currTime, hitobjectTime, holdnoteEnd))
					noteCount++;
			}

			timing.data = (double)noteCount / ((double)hitobjectTime - (double)currTime) * 1000.0;
			timing.time = hitobjectTime;
			timing.key = OSUMANIA::getKey(hitobjects[i]->getPos().X, KEYS);

			// True = just presses, False = Presses and Releases
			timing.press = true;
			for (long holdnoteEnd : holdnoteEnds)
				timing.press &= (holdnoteEnd == hitobjectTime);

			data.push_back(timing);

			currTime = hitobjects[i]->getTime();
			noteCount = 0;
		}
		else
		{
			noteCount++;
		}
	}
}

void Analyzer_NoteRate::AnalyzeDodge(Play* _play)
{

}