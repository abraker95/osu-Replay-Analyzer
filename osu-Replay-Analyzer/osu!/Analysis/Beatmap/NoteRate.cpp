#include "NoteRate.h"
#include "../../../utils/geometry.h"

#include "../../osu_standard.h"
#include "../../osu_mania.h"

Analyzer_NoteRate::Analyzer_NoteRate() : Analyzer("note rate (notes/ms)") {}
Analyzer_NoteRate::~Analyzer_NoteRate() {}

void Analyzer_NoteRate::AnalyzeStd(Play* _play)
{
	std::vector<Hitobject*>& hitobjects = _play->beatmap->getHitobjects();
	osu::TIMING timing;
	timing.data = 0;

	int i = 0, previ = 0;
	double delay = 10.0;

	for (int ms = 0; ms < hitobjects[hitobjects.size() - 1]->getTime(); ms += delay)
	{
		// Let index catch up to the time. Bail if out of bounds
		while (hitobjects[i]->getEndTime() < ms) i++;
		if (i >= hitobjects.size()) break;

		// Out of bound check
		if (!BTWN(1, i, hitobjects.size() - 1))
			continue;

		// previ - i needs to be at least 1 second apart
		while (hitobjects[i]->getTime() - hitobjects[previ]->getTime() > 1000.0)
		{
			if (previ < hitobjects.size() - 1) previ++;
			else break;
		}

		timing.data = i - previ;
		timing.pos = vector2df(hitobjects[i]->getPos().X, hitobjects[i]->getPos().Y);
		timing.press = false;
		timing.time = ms;

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
	int KEYS = _play->beatmap->getMods().getCS();

	std::vector<Hitobject*>& hitobjects = _play->beatmap->getHitobjects();
	osu::TIMING timing;
	timing.data = 0;

	int i = 0, previ = 0;
	double delay = 10.0;

	for (int ms = 0; ms < hitobjects[hitobjects.size() - 1]->getTime(); ms += delay)
	{
		// Let index catch up to the time. Bail if out of bounds
		while (hitobjects[i]->getEndTime() < ms) i++;
		if (i >= hitobjects.size()) break;

		// Out of bound check
		if (!BTWN(1, i, hitobjects.size() - 1))
			continue;

		// previ - i needs to be at least 1 second apart
		while (hitobjects[i]->getTime() - hitobjects[previ]->getTime() > 1000.0)
		{
			if (previ < hitobjects.size() - 1) previ++;
			else break;
		}

		timing.data = i - previ;
		timing.pos = vector2df(OSUMANIA::getKey(hitobjects[i]->getPos().X, KEYS), 0);
		timing.press = false;
		timing.time = ms;

		data.push_back(timing);
	}
}

void Analyzer_NoteRate::AnalyzeDodge(Play* _play)
{

}