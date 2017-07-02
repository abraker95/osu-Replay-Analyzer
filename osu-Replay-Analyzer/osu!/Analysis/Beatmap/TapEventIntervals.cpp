#include "TapEventIntervals.h"
#include "../../../utils/geometry.h"

#include "../../osu_standard.h"
#include "../../osu_mania.h"

Analyzer_TapEventIntervals::Analyzer_TapEventIntervals() : Analyzer("tap event intervals (ms)"){}
Analyzer_TapEventIntervals::~Analyzer_TapEventIntervals(){}

// Press release patterns for timing.key:
//	0 = release -> tap
//  1 = tap -> release
//  2 = tap -> tap
//  3 = tap -> hold

void Analyzer_TapEventIntervals::AnalyzeStd(Play* _play)
{
	Database<Hitobject>& hitobjects = _play->beatmap->getHitobjects();
	osu::TIMING timing;
	timing.data = 0;

	for (int i = 0; i < hitobjects.size(); i++)
	{

	// get tap event interval from previous object's end to this object's beginning
		if (i > 0)
		{
			// record the current press-release event interval
			long currInterval = abs(hitobjects[i - 1]->getEndTime() - hitobjects[i]->getTime());
			if (currInterval > 1000) continue; // if it's more than one second, then don't care

			timing.data = currInterval;

			// record the press-release pattern
			long prevInterval = hitobjects[i - 1]->getEndTime() - hitobjects[i - 1]->getTime();
			if (prevInterval > 150)	timing.key = 1; // tap -> release
			else					timing.key = 2; // tap -> tap

			// record position
			timing.pos = hitobjects[i]->getPos();

			// record current time
			timing.time = hitobjects[i]->getTime();

			data.push_back(timing);
		}

	// get tap event interval from this object's biggining to this object's end

		// record the current press-release event interval
		long currInterval = abs(hitobjects[i]->getTime() - hitobjects[i]->getEndTime());
		if (currInterval == 0) continue; // if it's not a hold note, then don't care
		timing.data = currInterval;

		// record the press-release pattern
		timing.key = 3; // tap -> hold

		// record position
		timing.pos = hitobjects[i]->getPos();

		// record current time
		timing.time = hitobjects[i]->getEndTime();

		data.push_back(timing);

	// get tap event interval from this object's end to this next object's beginning
		if (i < hitobjects.size() - 1)
		{
			// record the current press-release event interval
			long currInterval = abs(hitobjects[i]->getEndTime() - hitobjects[i + 1]->getTime());
			if (currInterval > 1000) continue; // if it's more than one second, then don't care

			timing.data = currInterval;

			// record the press-release pattern
			if (currInterval > 150)	timing.key = 0; // release -> tap
			else					timing.key = 2; // tap -> hold

			// record position
			timing.pos = hitobjects[i]->getPos();

			// record current time
			timing.time = hitobjects[i + 1]->getTime();

			data.push_back(timing);
		}
	}

	osu::SortByTime(data);
}

void Analyzer_TapEventIntervals::AnalyzeCatch(Play* _play)
{

}

void Analyzer_TapEventIntervals::AnalyzeTaiko(Play* _play)
{

}

void Analyzer_TapEventIntervals::AnalyzeMania(Play* _play)
{
	Database<Hitobject>& hitobjects = _play->beatmap->getHitobjects();
	int KEYS = _play->beatmap->getMods().getCS();
	
	osu::TIMING timing;
		timing.data = 0;

	long currTime = hitobjects[0]->getTime();
	std::vector<long> holdnoteEnds;
		holdnoteEnds.resize(KEYS);

	for (int i = 0; i < hitobjects.size(); i++)
	{
		if (hitobjects[i]->isHitobjectLong())
			holdnoteEnds[OSUMANIA::getKey(hitobjects[i]->getPos().X, KEYS)] = hitobjects[i]->getEndTime();

		long hitobjectTime = hitobjects[i]->getTime();
		if (hitobjectTime > currTime)
		{
			timing.data = hitobjectTime - currTime;
			timing.time = hitobjectTime;

			// True = just presses, False = Presses and Releases
			timing.press = true;
			for (long holdnoteEnd : holdnoteEnds)
				timing.press &= (holdnoteEnd == hitobjectTime);

			data.push_back(timing);

			currTime = hitobjects[i]->getTime();
		}
	}
}

void Analyzer_TapEventIntervals::AnalyzeDodge(Play* _play)
{

}