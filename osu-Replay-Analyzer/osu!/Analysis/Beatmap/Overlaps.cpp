#include "Overlaps.h"

#include "../../osu_standard.h"
#include "../../../utils/geometry.h"
#include "../../osuCalc.h"

Analyzer_Overlaps::Analyzer_Overlaps() : Analyzer("Overlap (%)") {}
Analyzer_Overlaps::~Analyzer_Overlaps() {}

void Analyzer_Overlaps::AnalyzeStd(Play* _play)
{
	Database<Hitobject>& hitobjects = _play->beatmap->getHitobjects();
	osu::TIMING timing;
		timing.data = 0;

	double arMs = AR2ms(_play->getMod()->getAR());
	double csPx = CS2px(_play->getMod()->getCS());

	for(int i=0; i<hitobjects.size(); i++)
	{
		// Go back to when the object first appears
		int j = i;
		for (; hitobjects[j]->getTime() > hitobjects[i]->getTime() - arMs; j--) { if (j <= 0) break; }
		j++;

		// Iterate through all object between then and now
		double overlapPercent = 0.0;
		for (; j < i; j++)
		{
			// \TODO: This only gets the max overlap between 2 object. This can be inaccurate, and instead needs to be overlap
			// between multiple object. However, there is difficutly in calculating overlaps between multiple objects geometricaly.
			// When calculating overlaps between 3 objects, for instance, the overlaps between object A and B and A and C need to be 
			// added up and then subracted by the overlap between A, B, and C. Getting the ovelap between A, B, C or any number of circles
			// Is the hard part. This can also be done via bruteforce, by drawing the cicles and figuring out the darkes shaded area, but 
			// that's not a viable option when calculating difficulty should take as little time as possible. 
			//
			// These sources may come in useful: 
			// http://www.ambrsoft.com/TrigoCalc/Circles3/Intersection.htm
			// http://mathworld.wolfram.com/Circle-CircleIntersection.html

			osu::TIMING closest = OSUSTANDARD::FindClosest(*hitobjects[j], *hitobjects[i], 10);
			double overlapArea = getCircleOverlapArea(csPx/2.0, closest.data);

			overlapPercent = MAX(overlapPercent, overlapArea / getCircleArea(csPx/2.0));
		}
		
		timing.data = overlapPercent;
		timing.pos = hitobjects[i]->getPos();
		timing.press = false;
		timing.time = hitobjects[i]->getTime();

		data.push_back(timing);
	}
	

	/*
	
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
		std::pair<int, int> visibilityTimes = OSUSTANDARD::getIndicesVisibleAt(_play, ms - AR2ms(_play->getMod()->getAR()), 0.0);

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
	}*/
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