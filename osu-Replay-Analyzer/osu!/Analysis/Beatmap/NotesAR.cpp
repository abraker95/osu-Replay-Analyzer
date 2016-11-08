#include "NotesAR.h"

#include "../../osuCalc.h"

#include "../../osu_standard.h"
#include "../../osu_mania.h"

Analyzer_NotesAR::Analyzer_NotesAR() : Analyzer("Notes for AR (notes * ms)") {}
Analyzer_NotesAR::~Analyzer_NotesAR() {}

void Analyzer_NotesAR::AnalyzeStd(Play* _play)
{
	std::vector<Hitobject*>& hitobjects = _play->beatmap->getHitobjects();
	osu::TIMING timing;
		timing.data = 0;

	double ARms = AR2ms(_play->getMod()->getAR());

	for (int i = 1; i < hitobjects.size(); i++)
	{
		int j = i;
		while (j--) if (hitobjects[i]->getTime() - hitobjects[j]->getTime() >= ARms) break;

		timing.data = ARms * (i - j);
		timing.pos = hitobjects[i]->getPos();
		timing.key = i;
		timing.press = false;
		timing.time = hitobjects[i]->getTime();

		data.push_back(timing);
	}
}

void Analyzer_NotesAR::AnalyzeCatch(Play* _play)
{

}

void Analyzer_NotesAR::AnalyzeTaiko(Play* _play)
{

}

void Analyzer_NotesAR::AnalyzeMania(Play* _play)
{
	// TODO: Choose a speed reference point and take SV into account
}

void Analyzer_NotesAR::AnalyzeDodge(Play* _play)
{

}