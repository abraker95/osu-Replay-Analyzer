#include "tap-deviation.h"

#include "../../../utils/geometry.h"

#include "../../osu_standard.h"
#include "../../osu_mania.h"

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

		bool press = ((pressState & 1) != 0);
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

	osu::TIMING timing;
		timing.data = 0;

	std::vector<int> recHitobject;		recHitobject.resize(KEYS);	// recHitobject is the hitobject we expect to record
	std::vector<int> currIFrame;		currIFrame.resize(KEYS);
	std::vector<int> prevIFrame;		prevIFrame.resize(KEYS);
	std::vector<bool> recordRelease;	recordRelease.resize(KEYS);

	// Get the starting notes of each column
	for(int key = 0; key < KEYS; key++)
		recHitobject[key] = OSUMANIA::getNextIndexOnColumn(hitobjects, key, KEYS, -1);

	int numFrames = _play->replay->getNumFrames();
	while(true)
	{
		for (int key = 0; key < KEYS; key++)
		{
			// Check if we are done with going through the objects on the current column. Skip if so
			if (recHitobject[key] >= hitobjects.size()) continue;

			// get update and get prev replay frame
			prevIFrame[key] = currIFrame[key];
			osu::TIMING prevFrame = _play->replay->getFrame(prevIFrame[key]);

			// Get the next replay frame on this coloumn
			osu::TIMING currFrame = _play->replay->getFrame(++currIFrame[key]);
			if (currIFrame[key] >= _play->replay->getNumFrames()) continue; // skip this column then as we are finished with it

			// Determine the press state; Blank, Press, Release, or Hold
			int pressState;
			if (currIFrame[key] > 0) pressState = OSUMANIA::getButtonState(prevFrame.pos.X, currFrame.pos.X, key);
			else					 pressState = OSUMANIA::getButtonState(0, currFrame.pos.X, key);

			bool press   = ((pressState & 1) != 0);
			bool release = ((pressState & 2) != 0);

			// Only look at key release or key press events
			if (!press && !release) continue;

			// Get note timing and player timing
			long timePlayer = currFrame.time,
				 timeTarget = hitobjects[recHitobject[key]]->getCloserTime(currFrame.time);

			// find the object closest to the player timing
			int index = OSUMANIA::FindHitobjectAt(_play, currFrame.time, key, timePlayer <= timeTarget);

			// We don't need to precess releases if we are not expecting one
			if (!recordRelease[key] && release) continue;

	// [NON TAP MISS]
			if ((index != recHitobject[key])) // we are still on previous hitobject
			{
				// If object was not found, just ignore. Can't be a miss
				if (index == -1) continue;

				timing.data = INT_MAX;
				timing.pos = currFrame.pos;
				timing.press = false;
				timing.time = currFrame.time;

				///data.push_back(timing);
				recHitobject[key] = OSUMANIA::getNextIndexOnColumn(hitobjects, key, KEYS, index); // catch up the recorded hitobjects to the current one
			
				continue;
			}

			// How early or late the player hit
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
				recHitobject[key] = OSUMANIA::getNextIndexOnColumn(hitobjects, key, KEYS, index); // catch up the recorded hitobjects to the current one

				continue; 
			}

	// [HIT]
			// mark to record release if it is a hold and we pressed
			bool isLong = hitobjects[index]->isHitobjectLong();
			if (hitobjects[index]->isHitobjectLong() && press) recordRelease[key] = true;
			else											   recordRelease[key] = false;

			// We are are at the same hitobject if we are expecting a release. Go to the next hitobject if not
			if (!recordRelease[key]) recHitobject[key] = OSUMANIA::getNextIndexOnColumn(hitobjects, key, KEYS, recHitobject[key]);

			// record timing data
			timing.data = deviation;
			timing.key = key;
			timing.pos = currFrame.pos;
			timing.press = press;
			timing.time = currFrame.time;

			data.push_back(timing);
		}

		// Check if we are done with every column
		bool done = true;
		for (int key = 0; key < KEYS; key++)
			done &= (OSUMANIA::getNextIndexOnColumn(hitobjects, key, KEYS, recHitobject[key]) == OSUMANIA::MANIA_END);
		if (done) break;
	}

	osu::SortByTime(data);
}

void Analyzer_TapDeviation::AnalyzeDodge(Play* _play)
{

}