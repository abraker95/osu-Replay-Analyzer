#include "score.h"

#include <tuple>
#include <iostream>

std::vector<OSUMANIA::TIMING> OSUMANIA::accTimings;
static Play* play;


const double MAX_PRESS_TIME   = +100;
const double MIN_PRESS_TIME   = -100;
const double MAX_RELEASE_TIME = +100;
const double MIN_RELEASE_TIME = -100;


struct KeyInfo
{
	double hitTiming;
	int key;
	std::vector<bool>* pressState;
	std::vector<bool>* nextNote;
};

bool sortAccTimings(OSUMANIA::TIMING i, OSUMANIA::TIMING j)
{
	return i.time < j.time;
}

int OSUMANIA::getJudgment(int _frameTime, int _noteTime, bool _pressState)
{
	bool hit = false;
	bool miss = true;  // assume miss to start with
	int hitTiming = _frameTime - _noteTime;

	// check if the note is long gone or not
	if (_frameTime > _noteTime + 150)
		return 2;

	// judge
	{
		// if hit
		if (_pressState == true)
		{
			hit  = BTWN(-100, hitTiming, 100);
			miss = BTWN(-150, hitTiming, -100) || BTWN(+100, hitTiming, +150);
		}

		// if release
		if (_pressState == false)
		{
			hit  = BTWN(-150, hitTiming, 150);
			miss = BTWN(-200, hitTiming, -150) || BTWN(+150, hitTiming, +200);
		}
	}

		 if (hit == true)	return 0;
	else if (miss == true)	return 1;
	else					return 3;	
}


void processHit(std::vector<OSUMANIA::TIMING>* _accTimings, Hitobject* _currNote, KeyInfo _info)
{
	bool isHoldObject = !(_currNote->getHitobjectType() & HITOBJECTYPE::CIRCLE);
	bool different = true;
	int key = _info.key;

	// make sure it's not first object to see if this is a different hit/release
	if (_accTimings->size() != 0)
	{
		// if time and key is different
		if ((*_info.pressState)[key] == true) // if we are pressing
			different = (_currNote->getTime() != (*_accTimings)[_accTimings->size() - 1].time) || 
						(_info.key != (*_accTimings)[_accTimings->size() - 1].key);
		else                                   // if we are releasing
			different = (_currNote->getEndTime() != (*_accTimings)[_accTimings->size() - 1].time) || 
						(_info.key != (*_accTimings)[_accTimings->size() - 1].key);
	}

	// record only if it's not the same thing as the last
	if (different)
	{
		if((*_info.pressState)[key] == true) // if we are pressing
			_accTimings->push_back((OSUMANIA::TIMING{ _currNote->getTime(), _info.hitTiming, _info.key, (*_info.pressState)[key] }));
		else						  // if we are releasing
			_accTimings->push_back((OSUMANIA::TIMING{ _currNote->getEndTime(), _info.hitTiming, _info.key, (*_info.pressState)[key] }));
	}

	// go to next note only if it's not a hold. We are expecting a release otherwise
	if (!isHoldObject)
	{
		(*_info.nextNote)[key] = true;    // set to fetch next note if we actually hit/released the note and not a blank spot
	}
	else
	{
		if ((*_info.pressState)[key] == false) // if we were expecting release
			(*_info.nextNote)[key] = true;    // set to fetch next note since we are done with this one

		(*_info.pressState)[key] = !(*_info.pressState)[key]; // we are expecting a release then now we expect a press. The same vice-versas
	}
}


void processMiss(std::vector<OSUMANIA::TIMING>* _accTimings, Hitobject* _currNote, KeyInfo _info)
{
	bool isHoldObject = !(_currNote->getHitobjectType() & HITOBJECTYPE::CIRCLE);
	int key = _info.key;

	if (!isHoldObject)			
		_accTimings->push_back((OSUMANIA::TIMING{ _currNote->getTime(), (double)INT_MAX, _info.key, (*_info.pressState)[key] }));
	else
		_accTimings->push_back((OSUMANIA::TIMING{ _currNote->getEndTime(), (double)INT_MAX, _info.key, (*_info.pressState)[key] }));

	(*_info.nextNote)[key] = true;		// set to fetch next note
	(*_info.pressState)[key] = true;	// we are expecting a press next
}

void HandleEarlyMiss(std::vector<OSUMANIA::TIMING>* _accTimings, Hitobject* _currNote, KeyInfo _info)
{
	int key = _info.key;

	if ((*_info.pressState)[key] == true) // if we are pressing
		_accTimings->push_back((OSUMANIA::TIMING{ _currNote->getTime(), (double)INT_MAX, _info.key, (*_info.pressState)[key] }));
	else						  // if we are releasing
		_accTimings->push_back((OSUMANIA::TIMING{ _currNote->getEndTime(), (double)INT_MAX, _info.key, (*_info.pressState)[key] }));

	(*_info.nextNote)[key] = true;		// set to fetch next note
	(*_info.pressState)[key] = true;	// we are expecting a press next
}


void OSUMANIA::genAccTimings(Play* _play)
{
	accTimings.clear();
	std::vector<OSUMANIA::TIMING>().swap(accTimings);

	play = _play;

	std::tuple<long, int, int> frame;
	std::vector<Hitobject*> prevNotes, currNotes, nextNotes;

	// Frame vars
	int iFrame = -1;
	bool stop = false;

	// process each column individualy
	int KEYS = play->beatmap->getMods().getCS();	
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
			bool valid = (((keyPresses & (1 << key)) > 0) && (pressStates[key] == true)) ||
						 (((keyReleases & (1 << key)) > 0) && (pressStates[key] == false));

			// if there is a key event on a column er are expecting
			if (valid)
			{
				int hitTiming = INT_MAX;
				int hitState = 1;

				if (currNotes[key] != nullptr)
				{
					if (pressStates[key] == true)	// if we are pressing the key
					{
						hitState = getJudgment(std::get<0>(frame), currNotes[key]->getTime(), pressStates[key]);
						hitTiming = std::get<0>(frame) - currNotes[key]->getTime();
					}
					else							// if we are releasing the key
					{
						hitState = getJudgment(std::get<0>(frame), currNotes[key]->getEndTime(), pressStates[key]);
						hitTiming = std::get<0>(frame) - currNotes[key]->getEndTime();
					}

					KeyInfo info = {};
						info.hitTiming = hitTiming;
						info.key = key;
						info.nextNote = &nextNote;
						info.pressState = &pressStates;

					switch (hitState)
					{
						case 0:		// hit
							processHit(&accTimings, currNotes[key], info);
						break;

						case 1:		// hit and miss
							processMiss(&accTimings, currNotes[key], info);
							break;

						case 2:     // never hit the note
							processMiss(&accTimings, currNotes[key], info);

							// get next note
							if (nextNote[key] == true)
							{
								prevNotes[key] = currNotes[key];
								currNotes[key] = nextNotes[key];
								nextNotes[key] = getNextNoteOnColumn(key, &iNote[key]);

								nextNote[key] = false;
							}

							// redo this frame
							key--;
							continue;
							break;

						case 3:		// no hit, no miss
							break;

						default:
							break;
					};
				}
			}

			// get next note
			if (nextNote[key] == true)
			{
				prevNotes[key] = currNotes[key];
				currNotes[key] = nextNotes[key];
				nextNotes[key] = getNextNoteOnColumn(key, &iNote[key]);

				nextNote[key] = false;
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
	for (; (*_iNote) < play->beatmap->getHitobjects().size(); (*_iNote)++)
	{
		int KEYS = play->beatmap->getMods().getCS();
		int noteXpos = play->beatmap->getHitobjects()[*_iNote]->getPos().X;

		float localWDivisor = 512.0f / KEYS;
		int column = std::min((int)std::floor(noteXpos / localWDivisor), KEYS - 1);

		if (column == _column)
			return play->beatmap->getHitobjects()[*_iNote];
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