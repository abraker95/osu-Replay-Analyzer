#include "tap-deviation.h"

#include "../../../utils/geometry.h"

#include "../../osu_standard.h"
#include "../../osu_mania.h"

#include "../AnalysisStruct.h"

Analyzer_TapDeviation::Analyzer_TapDeviation() : Analyzer("Tap Deviation (ms)") {}
Analyzer_TapDeviation::~Analyzer_TapDeviation() {}

void Analyzer_TapDeviation::AnalyzeStd(Play* _play)
{
	if (_play->replay->isValid() == false) return;
	std::vector<Hitobject*>& hitobjects = _play->beatmap->getHitobjects();

	osu::TIMING timing;
	timing.data = 0;

	// recHitobject is the hitobject we expect to record
	int recHitobject = 0;
	bool recordRelease = false;

	int numFrames = _play->replay->getNumFrames();
	for (int i = 0; i<numFrames; i++)
	{
		// Check if we are done with going through the objects
		if (recHitobject >= hitobjects.size()) break;

		// get replay frames
		osu::TIMING currFrame = _play->replay->getFrame(i);
		osu::TIMING prevFrame = _play->replay->getFrame(i);
		if(i > 0)
			prevFrame = _play->replay->getFrame(i - 1);

		int pressState;
		if (i > 0) pressState = OSUSTANDARD::getButtonState(prevFrame.key, currFrame.key);
		else	   pressState = OSUSTANDARD::getButtonState(0, currFrame.key);

		bool press   = ((pressState & 1) != 0);
		bool release = ((pressState & 2) != 0);

		// Only look at key release or key press events
		if (!press && !release) continue;

		long timePlayer = currFrame.time,
			 timeTarget = hitobjects[recHitobject]->getCloserTime(currFrame.time);

		// find the object closest to this event
		int index = OSUSTANDARD::FindHitobjectAt(hitobjects, currFrame.time, timePlayer <= timeTarget);
		
		// Skip spinners
		if (hitobjects[index]->getHitobjectType() == HITOBJECTYPE::SPINNER)
		{
			recHitobject++;
			continue;
		}

		// We don't need to precess releases if we are not expecting one
		if (!recordRelease && release) continue;

	// [NON TAP MISS]
		if ((index != recHitobject)) // we are still on previous hitobject
		{
			timing.data = INT_MAX;
			timing.pos = currFrame.pos;
			timing.press = false;
			timing.time = currFrame.time;

			///data.push_back(timing);
			recHitobject = index + 1; // catch up the recorded hitobjects to the current one
			
			continue;
		}

		int deviation = timePlayer - timeTarget;
	
	// [EARLY/LATE MISS]
		if (deviation < -500) // too early
		{
			continue;
		}
		else if (deviation > 500) // too late
		{
			timing.data = INT_MAX;
			timing.pos = currFrame.pos;
			timing.press = false;
			timing.time = currFrame.time;

			///data.push_back(timing);
			recHitobject = index + 1; // catch up the recorded hitobjects to the current one

			continue; 
		}

	// [HIT]
		// mark to record release if it is a hold and we pressed
		if (hitobjects[index]->isHitobjectLong() && press) recordRelease = true;
		else											   recordRelease = false;

		// We are are at the same hitobject if we are expecting a release
		if (!recordRelease) recHitobject++;

		// record timing data
		timing.data = deviation;
		timing.pos = currFrame.pos;
		timing.press = press;
		timing.time = currFrame.time;

		data.push_back(timing);
	}
}

void Analyzer_TapDeviation::AnalyzeCatch(Play* _play)
{

}

void Analyzer_TapDeviation::AnalyzeTaiko(Play* _play)
{

}


/// \TODO: Fix bug where there are no timings at last set of notes
void Analyzer_TapDeviation::AnalyzeMania(Play* _play)
{
	if (_play->replay->isValid() == false) return;

	std::vector<Hitobject*>& hitobjects = _play->beatmap->getHitobjects();
	int KEYS = _play->beatmap->getMods().getCS();

	std::vector<long> recTimes;	recTimes.resize(KEYS);
	for (long& recTime : recTimes) recTime = INT_MIN;

	osu::TIMING timing;
		timing.data = 0;

	int MISS_DEVIATION = 100;	// Max deviation +/- where a miss is not counted
	int NOHIT_DEVIATION = 150;	// // Min deviation /- where a key is not counted

	Analyzer* analyzer = AnalysisStruct::beatmapAnalysis.getAnalyzer("Press-Release Intervals (ms)");
	if (analyzer == nullptr) return;

	std::vector<osu::TIMING>& tapReleaseIntervals = *(analyzer->getData());
	if (tapReleaseIntervals.size() == 0) return;

	// Split the stuff up by keys to make things easier to search
	std::vector<std::vector<osu::TIMING>> tapRelaseIntervalsKeys; tapRelaseIntervalsKeys.resize(KEYS);
	for (osu::TIMING tapReleaseIterval : tapReleaseIntervals)
	{
		int key = tapReleaseIterval.key;
		tapRelaseIntervalsKeys[key].push_back(tapReleaseIterval);
	}

	// Parse the map
	for (int i = 0; i < hitobjects.size(); i++)
	{
		long currTime = hitobjects[i]->getTime();
		int key = OSUMANIA::getKey(hitobjects[i]->getPos().X, KEYS);
		
		std::vector<osu::TIMING>& tapReleases = tapRelaseIntervalsKeys[key];
		int index = osu::FindTimingAt(tapReleases, currTime);
		
		// Fastforward to latest recorded note
		while (tapReleases[index].time <= recTimes[key])
		{
			if (index == tapReleases.size() - 1) break;
			index++;
		}

		// Find easliest non recorded timing
		if (index > 0)
		{
			while (tapReleases[index - 1].time > recTimes[key])
			{
				index--;
				if (index == 0) break;
			}
		}
		
		// No count if outside the no-hit threshold
		if (tapReleases[index].time <= currTime - NOHIT_DEVIATION)
		{
			recTimes[key] = tapReleases[index].time;
			i--;  // hitobject not counted, so go back and redo

			continue;
		}

		// MISS if within the no-hit threshold
		if (tapReleases[index].time <= currTime - MISS_DEVIATION)
		{	
			timing.data = currTime - tapReleases[index].time;
			timing.key = key;
			timing.press = false;
			timing.time = currTime;

			data.push_back(timing);
			recTimes[key] = tapReleases[index].time;

			continue; // not to do the below	
		}

		// MISS if within the no-hit threshold
		if (tapReleases[index].time >= currTime + MISS_DEVIATION)
		{
			timing.data = currTime - tapReleases[index].time;
			timing.key = key;
			timing.press = false;
			timing.time = currTime;

			data.push_back(timing);
			// Don't record this timing since it's not intended for this note

			continue; // not to do the below	
		}

		if (hitobjects[i]->isHitobjectLong())
		{
			// MISS if tap when the hold note finished
			if (tapReleases[index].time >= hitobjects[i]->getEndTime())
			{
				timing.data = currTime - tapReleases[index].time;
				timing.key = key;
				timing.press = false;
				timing.time = currTime;

				data.push_back(timing);
				// Don't record this timing since it's not intended for this note

				continue; // not to do the below
			}
		}

	// HIT

		// Normal note
		int deviation = tapReleases[index].time - currTime;

		timing.data = deviation;
		timing.key = key;
		timing.press = true;
		timing.time = currTime;

		data.push_back(timing);
		recTimes[key] = tapReleases[index].time;

		// If a hold note
		if (hitobjects[i]->isHitobjectLong())	
		{
			deviation = (tapReleases[index].time + tapReleases[index].data) - hitobjects[i]->getEndTime();

			timing.data = deviation;
			timing.key = key;
			timing.press = true;
			timing.time = hitobjects[i]->getEndTime();

			data.push_back(timing);
			recTimes[key] = tapReleases[index].time + tapReleases[index].data;
		}
	}

	osu::SortByTime(data);
}

void Analyzer_TapDeviation::AnalyzeDodge(Play* _play)
{

}