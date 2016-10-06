#include "Overlaps.h"
#include "../../osu_standard.h"
#include "../../../utils/geometry.h"
#include "../../osuCalc.h"

Analyzer_Overlaps::Analyzer_Overlaps() : Analyzer("overlaps") {}
Analyzer_Overlaps::~Analyzer_Overlaps() {}

void Analyzer_Overlaps::AnalyzeStd(Play* _play)
{
	std::vector<Hitobject*>& hitobjects = _play->beatmap->getHitobjects();
	osu::TIMING timing;
	timing.data = 0;

	int i = 0, previ = 0;
	double delay = 10.0;
	int iobj = 0;

	for (int ms = 0; ms < hitobjects[hitobjects.size() - 1]->getTime(); ms += delay)
	{
		// Let index catch up to the time. Bail if out of bounds
		while (hitobjects[i]->getEndTime() < ms) i++;
		if (i >= hitobjects.size()) break;

		// Bounds check
		if (!BTWN(1, i, hitobjects.size() - 1))
			continue;

		// Reset overlap data only when we go to the next object
		if (previ != i)
		{
			timing.data = 0.0;
			iobj = i;
		}

		// Go back all the way when the current note first becomes visible and iterate over the visible notes.
		std::pair<int, int> visibilityTimes = _play->beatmap->getIndicesVisibleAt(ms - AR2ms(_play->getMod()->getAR()), 0.0);

		for (int i2 = visibilityTimes.first; i2 < visibilityTimes.second; i2++)
		{
			// break if we reached the current object
			if (i2 == i)
				break;

			double overlap = 0.0;
			if (previ != i) // compute overlap only if we go to the next object
			{
				/// \TODO: Maybe a function to calculate hitobject length to circle size ratio?

				double circleArea = getCircleArea(CS2px(_play->getMod()->getCS()) / 2.0);
				overlap = GetHitobjectOverlapArea(_play, i, i2) / circleArea;

				// If it's a slider, account for its length (size) by dividing the overlap 
				// factor by the ratio between slider size and total circle size
				if (hitobjects[i]->isHitobjectLong())
					overlap /= hitobjects[i]->getSlider()->getLength() / CS2px(_play->beatmap->getMods().getCS());

				// Record earliest note resposible for 75% or more overlap
				if (overlap > 0.75)
					iobj = std::min(i2, iobj);
			}

			timing.data += overlap;
		}

		previ = i;

		// compile info and record
		timing.time = ms;
		timing.key = i - iobj;  // allows to determine if there are any notes between the overlapping note
		timing.pos = hitobjects[i]->getPos();
		timing.press = false;

		data.push_back(timing);
	}
}

void Analyzer_Overlaps::AnalyzeCatch(Play* _play)
{

}

void Analyzer_Overlaps::AnalyzeTaiko(Play* _play)
{

}

void Analyzer_Overlaps::AnalyzeMania(Play* _play)
{

}

void Analyzer_Overlaps::AnalyzeDodge(Play* _play)
{

}