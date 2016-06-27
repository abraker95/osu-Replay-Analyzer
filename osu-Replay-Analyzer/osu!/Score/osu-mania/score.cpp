#include "score.h"

#include <tuple>
#include <iostream>

std::vector<OSUMANIA::TIMING> OSUMANIA::accTimings;
static Play* play;


const double MAX_PRESS_TIME   = +100;
const double MIN_PRESS_TIME   = -100;
const double MAX_RELEASE_TIME = +100;
const double MIN_RELEASE_TIME = -100;

bool sortAccTimings(OSUMANIA::TIMING i, OSUMANIA::TIMING j)
{
	return i.time < j.time;
}

void OSUMANIA::genAccTimings(Play* _play)
{
	play = _play;

	std::tuple<long, int, int> frame;
	std::vector<Hitobject*> prevNotes, currNotes, nextNotes;

	// Frame vars
	int iFrame = -1;
	bool stop = false;

	// process each column individualy
	int KEYS = play->beatmap->getDiff().cs;	
	std::vector<int> iNote;					// keep track where we are on the map
	std::vector<bool> nextNote;				// whether to fetch next note on column or not
	std::vector<bool> pressStates;			// which state we are expecting
		iNote.resize(KEYS);
		nextNote.resize(KEYS);
		pressStates.resize(KEYS);

	// init vars for each column
	{
		prevNotes.resize(KEYS);
		currNotes.resize(KEYS);
		nextNotes.resize(KEYS);

		for (int key = 0; key < KEYS; key++)
		{
			iNote[key] = -1;

			prevNotes[key] = currNotes[key];
			currNotes[key] = getNextNoteOnColumn(key, &iNote[key]);
			nextNotes[key] = getNextNoteOnColumn(key, &iNote[key]);

			nextNote[key] = false;
			pressStates[key] = true;
		}
	}

	// get first frame
	frame = getNextEvent(&iFrame);

	do // key press-release processor
	{
		int keyPresses = std::get<1>(frame);
		int keyReleases = std::get<2>(frame);

		for (int key = 0; key < KEYS; key++)
		{
			if (pressStates[key] == true) // if we are expecting a press
			{
				// process key presess
				if (keyPresses & (1 << key))
				{
					if (currNotes[key] != nullptr) // make sure we have a valid note
					{
						//std::pair<double, double> ODinterval = getODms(prevNotes[key], currNotes[key], nextNotes[key], true);  // not used for now
						bool isHoldObject = !(currNotes[key]->getHitobjectType() & HITOBJECTYPE::CIRCLE);
						int hitTiming = std::get<0>(frame) - currNotes[key]->getTime();

						if (BTWN(-100, hitTiming, 100)) // record if it is within accepted hit/release period
						{
							bool different = true;
							if (accTimings.size() != 0)
							{
								// if time and key is different
								different = (currNotes[key]->getTime() != accTimings[accTimings.size() - 1].time) || (key != accTimings[accTimings.size() - 1].key);
							}

							if (different) // record only if it's not the same thing as the last
								accTimings.push_back((TIMING{ currNotes[key]->getTime(), (double)hitTiming, key, true }));

							if (!isHoldObject) // go to next note only if it's not a hold. We are expecting a release otherwise
								nextNote[key] = true;  // set to fetch next note if we actually hit/released the note and not a blank spot
							else
								pressStates[key] = false;
						}
						else if (BTWN(-150, hitTiming, -100) || BTWN(+100, hitTiming, +150)) // else record a miss as infinite timing period
						{
							bool different = true;
							if (accTimings.size() != 0)
							{
								// if time and key is different
								different = (currNotes[key]->getTime() != accTimings[accTimings.size() - 1].time) || (key != accTimings[accTimings.size() - 1].key);
							}

							if (different) // record only if it's not the same thing as the last
								accTimings.push_back((TIMING{ currNotes[key]->getTime(), (double)INT_MAX, key, true }));

							if (isHoldObject)
								accTimings.push_back((TIMING{ currNotes[key]->getEndTime(), (double)INT_MAX, key, false }));

							nextNote[key] = true;  // set to fetch next note if we actually hit/released the note and not a blank spot
						}
						else
						{
							// else it's just tapping not meant for a note
						}
					}
				}
			}
			else // if we are expecting a release
			{
				// process key release
				if (keyReleases & (1 << key))
				{
					if (currNotes[key] != nullptr) // make sure we have a valid note
					{
						std::pair<double, double> ODinterval = getODms(prevNotes[key], currNotes[key], nextNotes[key], false);
						bool isHoldObject = !(currNotes[key]->getHitobjectType() & HITOBJECTYPE::CIRCLE);
						int hitTiming = std::get<0>(frame) - currNotes[key]->getEndTime();

						// count releases only for holds
						if (isHoldObject)
						{
							if (BTWN(-150, hitTiming, 150)) // record if it is within accepted hit/release period
							{
								accTimings.push_back((TIMING{ currNotes[key]->getEndTime(), (double)hitTiming, key, false }));
								nextNote[key] = true;  // set to fetch next note if we actually hit/released the note and not a blank spot
								pressStates[key] = true;
							}
							else // else record a miss as infinite timing period
							{
								accTimings.push_back((TIMING{ currNotes[key]->getEndTime(), (double)INT_MAX, key, false }));
								nextNote[key] = true;  // set to fetch next note if we missed the note
								pressStates[key] = true;
							}
						}
					}
				}
			}

			// fetch next note
			if (nextNote[key] == true)
			{
				prevNotes[key] = currNotes[key];
				currNotes[key] = nextNotes[key];
				nextNotes[key] = getNextNoteOnColumn(key, &iNote[key]);

				nextNote[key] = false;

				// see if the next the recently fetched note was actually hit
				if (currNotes[key] != nullptr)
				{
					int missFrame = iFrame;
					std::tuple<long, int, int> seekMiss;
					bool stopSeek = !(missFrame < play->replay->getNumFrames()); // stop when we reached end of replay
					bool miss = true;

					while (std::get<0>(seekMiss) < currNotes[key]->getTime() + 100)
					{
						if (stopSeek) // if we reached end of replay, we never hit the note
						{
							break;
						}

						if (std::get<1>(seekMiss) & (1 << key))
						{
							miss = false;
							break;
						}

						seekMiss = getNextEvent(&missFrame);
					}

					// record and get next note
					if (miss)
					{
						accTimings.push_back((TIMING{ currNotes[key]->getTime(), INT_MAX, key }));

						// fetch next note
						prevNotes[key] = currNotes[key];
						currNotes[key] = nextNotes[key];
						nextNotes[key] = getNextNoteOnColumn(key, &iNote[key]);

						nextNote[key] = false;
					}
				}
			}
		}

		frame = getNextEvent(&iFrame); // get next press/release whatever column it is
		stop = !(iFrame < play->replay->getNumFrames()); // stop when we reached end of replay
	} while (!stop);

	std::sort(accTimings.begin(), accTimings.end(), sortAccTimings);
}


// note's valid hitRange (unused for now)
std::pair<double, double> OSUMANIA::getODms(Hitobject* _prevNote, Hitobject* _currNote, Hitobject* _nextNote, bool _press)
{
	double earliestTime = 0.0;
	double latestTime = 0.0;

	// if it's the first note
	if (_prevNote == nullptr)
	{
		int currNoteType = _currNote->getHitobjectType();
		int nextNoteType = _nextNote->getHitobjectType();

		// calculating press time for the note
		if (_press == true)
		{
			// press normal note -> latest time = midpoint btwn curr & nxt; earliest = mirror latest
			if (currNoteType & HITOBJECTYPE::CIRCLE)
			{
				latestTime = (double)(_currNote->getTime() + _nextNote->getTime()) / 2.0;
				earliestTime = 2 * _currNote->getTime() - latestTime;
			}

			// press hold note -> latest time = midpoint btwn start & end; earliest = mirror latest on start point
			if (currNoteType & HITOBJECTYPE::MANIALONG)
			{
				latestTime = (double)(_currNote->getTime() + _currNote->getEndTime()) / 2.0;
				earliestTime = 2 * _currNote->getTime() - latestTime;
			}
		}
		else // calculating release time for the note
		{
			// release normal note -> latest time = midpoint btwn curr & nxt; earliest = mirror latest
			if (currNoteType & HITOBJECTYPE::CIRCLE)
			{
				latestTime = (double)(_currNote->getTime() + _nextNote->getTime()) / 2.0;
				earliestTime = 2 * _currNote->getTime() - latestTime;
			}

			// release hold note -> latest time = midpoint btwn curr end & nxt; earliest = midpoint btwn curr start & curr end
			if (currNoteType & HITOBJECTYPE::MANIALONG)
			{
				latestTime = (double)(_currNote->getEndTime() + _nextNote->getTime()) / 2.0;
				earliestTime = (double)(_currNote->getTime() + _currNote->getEndTime()) / 2.0;
			}
		}
	}

	// if it's not last or first note
	if (_nextNote != nullptr && _prevNote != nullptr)
	{
		int prevNoteType = _prevNote->getHitobjectType();
		int currNoteType = _currNote->getHitobjectType();
		int nextNoteType = _nextNote->getHitobjectType();

		// calculating press time for the note
		if (_press == true)
		{
			// press normal note -> earliest time = midpoint btwn prev & curr; latest time = midpoint btwn curr & nxt
			if (currNoteType & HITOBJECTYPE::CIRCLE)
			{
				earliestTime = (double)(_prevNote->getTime() + _currNote->getTime()) / 2.0;
				latestTime = (double)(_currNote->getTime() + _nextNote->getTime()) / 2.0;
			}
			
			// press hold note -> earliest time = midpoint btwn prev & curr start; latest = midpoint btwn curr start & curr end
			if (currNoteType & HITOBJECTYPE::MANIALONG)
			{
					earliestTime = (double)(_prevNote->getTime() + _currNote->getTime()) / 2.0;
					latestTime = (double)(_currNote->getTime() + _currNote->getEndTime()) / 2.0;
			}
		}
		else // calculating release time for the note
		{
			// release normal note -> earliest time = midpoint btwn prev & curr; latest time = midpoint btwn curr & nxt
			if (currNoteType & HITOBJECTYPE::CIRCLE)
			{
				earliestTime = (double)(_prevNote->getTime() + _currNote->getTime()) / 2.0;
				latestTime = (double)(_currNote->getTime() + _nextNote->getTime()) / 2.0;
			}

			// release hold note -> earliest time = midpoint btwn curr start & curr end; latest time = midpoint btwn curr end & nxt
			if (currNoteType & HITOBJECTYPE::MANIALONG)
			{
				earliestTime = (double)(_currNote->getTime() + _currNote->getEndTime()) / 2.0;
				latestTime = (double)(_currNote->getEndTime() + _nextNote->getTime()) / 2.0;
			}
		}
	}

	// if it's the last note
	if (_nextNote == nullptr)
	{
		int prevNoteType = _prevNote->getHitobjectType();
		int currNoteType = _currNote->getHitobjectType();

		// calculating press time for the note
		if (_press == true)
		{
			// press normal note -> earliest time = midpoint btwn prev & curr; latest = mirror earliest
			if (currNoteType & HITOBJECTYPE::CIRCLE)
			{
				earliestTime = (double)(_prevNote->getTime() + _currNote->getTime()) / 2.0;
				latestTime = 2*_currNote->getTime() - earliestTime;
			}
			
			// press hold note -> earliest time = midpoint btwn prev & curr start; latest = midpoint btwn curr start & curr end
			if (currNoteType & HITOBJECTYPE::MANIALONG)
			{
				earliestTime = (double)(_prevNote->getTime() + _currNote->getTime()) / 2.0;
				latestTime = (double)(_currNote->getTime() + _currNote->getEndTime()) / 2.0;
			}
		}
		else // calculating release time for the note
		{
			// release normal note -> earliest time = midpoint btwn prev & curr; latest = mirror earliest on
			if (currNoteType & HITOBJECTYPE::CIRCLE)
			{
				earliestTime = (double)(_currNote->getTime() + _prevNote->getEndTime()) / 2.0;
				latestTime = 2 * _currNote->getTime() - earliestTime;
			}
			
			// release hold note -> earliest time = midpoint btwn curr & nxt; latest = mirror earliest on end point
			if (currNoteType & HITOBJECTYPE::MANIALONG)
			{
				earliestTime = (double)(_currNote->getTime() + _currNote->getEndTime()) / 2.0;
				latestTime = 2 * _currNote->getEndTime() - earliestTime;
			}
		}
	}


	// calculating press time for the note
	if (_press == true)
	{
		// cap interval
		BOUND(_currNote->getTime() - MIN_PRESS_TIME, earliestTime, _currNote->getTime()					);
		BOUND(_currNote->getTime(),                  latestTime,   _currNote->getTime() + MAX_PRESS_TIME);
	}
	else // release
	{
		// cap interval
		BOUND(_currNote->getEndTime() - MIN_RELEASE_TIME, earliestTime, _currNote->getEndTime()					  );
		BOUND(_currNote->getEndTime(),					  latestTime,   _currNote->getEndTime() + MAX_RELEASE_TIME);
	}

	return std::pair<double, double>(earliestTime, latestTime);
}


Hitobject* OSUMANIA::getNextNoteOnColumn(int _column, int* _iNote)
{
	(*_iNote) += 1;
	for (; (*_iNote) < play->beatmap->hitObjects.size(); (*_iNote)++)
	{
		int KEYS = play->beatmap->getDiff().cs;
		int noteXpos = play->beatmap->hitObjects[*_iNote]->getPos().X;

		float localWDivisor = 512.0f / KEYS;
		int column = std::min((int)std::floor(noteXpos / localWDivisor), KEYS - 1);

		if (column == _column)
			return play->beatmap->hitObjects[*_iNote];
	}

	return nullptr;
}


std::tuple<long, int, int> OSUMANIA::getNextEvent(int* _iFrame)
{
	int currKeyMask = 0, prevKeyMask = 0;
	std::tuple<long, irr::core::vector2df, int> frame;

	*_iFrame += 1;
	if (*_iFrame > 1)
		currKeyMask = std::get<1>(play->replay->getFrame((*_iFrame) - 1)).X;

	for (; *_iFrame < play->replay->getNumFrames(); (*_iFrame)++)
	{
		frame = play->replay->getFrame(*_iFrame);

		prevKeyMask = currKeyMask;
		currKeyMask = std::get<1>(frame).X;

		if ((prevKeyMask ^ currKeyMask) != 0)
			break;
	}

	unsigned int presses = (currKeyMask & ~(0x0)) & (~prevKeyMask & ~(0x0));  // curr -> press,        prev -> not pressed
	unsigned int releases = (~currKeyMask & ~(0x0)) & (prevKeyMask & ~(0x0));  // curr -> not pressed   prev -> pressed

	return std::tuple<long, int, int>(std::get<0>(frame), presses, releases);
}