#include "IntensityForHand.h"
#include "../../../utils/geometry.h"

#include "../../osu_standard.h"
#include "../../osu_mania.h"

Analyzer_IntensityForHand::Analyzer_IntensityForHand() : Analyzer("max note rate for key (notes/s)") {}
Analyzer_IntensityForHand::~Analyzer_IntensityForHand() {}

/// \TODO: Maybe detect stream vs single tap speeds? Players can technically do a bit more than ~7 notes/s for single tap
/// Which is 4.5 note/s alternating. Just figure out which one would go to which
void Analyzer_IntensityForHand::AnalyzeStd(Play* _play){}

void Analyzer_IntensityForHand::AnalyzeCatch(Play* _play){}

void Analyzer_IntensityForHand::AnalyzeTaiko(Play* _play){}

void Analyzer_IntensityForHand::AnalyzeMania(Play* _play)
{
	const int EMPTY   = 0;
	const int PRESS   = 1;
	const int RELEASE = 2;
	const int HOLD    = 3;

	int KEYS = _play->beatmap->getMods().getCS();
	std::vector<Hitobject*>& hitobjects = _play->beatmap->getHitobjects();
	osu::TIMING timing;
		timing.data = 0;

	std::vector<int> prevKeyActionStatus, currKeyActionStatus, nextKeyActionStatus;
		prevKeyActionStatus.resize(KEYS);
		currKeyActionStatus.resize(KEYS);
		nextKeyActionStatus.resize(KEYS);

	std::vector<long> releaseTimes;
		releaseTimes.resize(KEYS);

	int iCurr = -1;
	while (true)
	{

// [STATE SNAPSHOT PART]		

		// Get how many keys are pressed on the next closest timing
		int numPressesNext = OSUMANIA::getNumPressesOnNextTiming(_play, &iCurr);
		if (numPressesNext == 0) return;

		prevKeyActionStatus = currKeyActionStatus;
		currKeyActionStatus = nextKeyActionStatus;

		// Check for hold releases and reset states
		int currTime = hitobjects[iCurr]->getTime();
		for (int key = 0; key < KEYS; key++)
		{
			if (releaseTimes[key] == currTime)		// if it's release time, mark for release
				nextKeyActionStatus[key] = RELEASE;
			else if(releaseTimes[key] < currTime)	// if it's not release time yet, mark for hold
				nextKeyActionStatus[key] = HOLD;
			else									// otherwise mark for empty
				nextKeyActionStatus[key] = EMPTY;
		}

		/// \TODO: Make sure starting and ending index is correct
		for (int i = iCurr - numPressesNext; i < iCurr; i++)
		{
			int key = OSUMANIA::getKey(hitobjects[iCurr]->getPos().X, KEYS);

			///assert(nextKeyActionStatus[key] == 0); /// \TODO: Make sure this is always true
			nextKeyActionStatus[key] = PRESS;

			// If it's a hold, remember it
			if (hitobjects[iCurr]->isHitobjectLong())
				releaseTimes[key] = hitobjects[iCurr]->getEndTime();
		}

// [SINGLE LANE OPERATIONS]

		std::vector<int> keyMultiplier, handDependenceMask;
			keyMultiplier.resize(KEYS);
			handDependenceMask.resize(KEYS);

		
		if (KEYS == 1)  handDependenceMask = { 1 };
		if (KEYS == 2)  handDependenceMask = { 1, 1 };
		if (KEYS == 3)  handDependenceMask = { 1, 1, 2 };
		if (KEYS == 4)  handDependenceMask = { 1, 1, 2, 2 };
		if (KEYS == 5)  handDependenceMask = { 1, 1, 2, 3, 3 };
		if (KEYS == 6)  handDependenceMask = { 1, 1, 1, 2, 2, 2 };
		if (KEYS == 7)  handDependenceMask = { 1, 1, 1, 2, 3, 3, 3 };
		if (KEYS == 8)  handDependenceMask = { 1, 1, 1, 2, 3, 4, 4, 4 };
		if (KEYS == 9)  handDependenceMask = { 1, 1, 1, 1, 2, 3, 4, 4, 4 };
		if (KEYS == 10) handDependenceMask = { 1, 1, 1, 1, 2, 3, 4, 4, 4, 4 };
		if (KEYS == 12) handDependenceMask = { 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2 };
		if (KEYS == 14) handDependenceMask = { 1, 1, 1, 1, 1, 2, 2, 3, 3, 4, 4, 4, 4, 4 };
		if (KEYS == 16) handDependenceMask = { 1, 1, 1, 1, 1, 1, 2, 2, 3, 3, 4, 4, 4, 4, 4 };
		if (KEYS == 18) handDependenceMask = { 1, 1, 1, 1, 1, 1, 1, 2, 2, 3, 3, 4, 4, 4, 4, 4, 4 };

		
		// Pointer = 1
		// Middle  = 2
		// Thumb   = 2
		// Index   = 4
		// Pinky   = 8
		// All else follows the power of 2 rule
		
		if (KEYS == 1)  keyMultiplier = { 1 };
		if (KEYS == 2)  keyMultiplier = { 1, 1 };
		if (KEYS == 3)  keyMultiplier = { 1, 1, 2 };
		if (KEYS == 4)  keyMultiplier = { 2, 1, 1, 2 };
		if (KEYS == 5)  keyMultiplier = { 2, 1, 2, 1, 2 };
		if (KEYS == 6)  keyMultiplier = { 4, 2, 1, 1, 1, 4 };
		if (KEYS == 7)  keyMultiplier = { 4, 2, 1, 2, 1, 2, 4 };
		if (KEYS == 8)  keyMultiplier = { 4, 2, 1, 2, 2, 1, 2, 4 };
		if (KEYS == 9)  keyMultiplier = { 8, 4, 2, 1, 2, 2, 1, 2, 4 };
		if (KEYS == 10) keyMultiplier = { 8, 4, 2, 1, 2, 2, 1, 2, 4, 8 };
		if (KEYS == 12) keyMultiplier = { 16, 8, 4, 2, 1, 2, 2, 1, 2, 4, 8, 16 };
		if (KEYS == 14) keyMultiplier = { 32, 16, 8, 4, 2, 1, 2, 2, 1, 2, 4, 8, 16, 32 };
		if (KEYS == 16) keyMultiplier = { 64, 16, 8, 4, 2, 1, 2, 2, 1, 2, 4, 8, 16, 32, 64 };
		if (KEYS == 18) keyMultiplier = { 128, 64, 16, 8, 4, 2, 1, 2, 2, 1, 2, 4, 8, 16, 32, 64, 128 };

		int laneMultiplier = 1;
		double snapshotDiff = 0;
		for (int key = 0; key < KEYS; key++)
		{
			// check for actions which require finger independence
			if (key < KEYS - 2)
			{
				// make sure the keys are on the same hand
				if (handDependenceMask[key] == handDependenceMask[key + 1] == handDependenceMask[key + 2])
				{
					int key1 = currKeyActionStatus[key];
					int key2 = currKeyActionStatus[key + 1];
					int key3 = currKeyActionStatus[key + 2];

					// press-hold-press, release-hold-release, etc
					if ((key1 == key3) && (key2 != key1) && (key2 != 0))
						snapshotDiff += 1.5 * (keyMultiplier[key] + keyMultiplier[key + 1] + keyMultiplier[key + 2]);
				}				
			}

			snapshotDiff += keyMultiplier[key] * currKeyActionStatus[key];
		}
		/// \TODO: Lane multiplier

// [MULTI LANE OPERATIONS]

		/// \TODO: Action adder

// [MULTI SNAPSHOT OPERATIONS]

		/// Timing Difficulty
	}

	osu::SortByTime(data);
}

void Analyzer_IntensityForHand::AnalyzeDodge(Play* _play)
{

}