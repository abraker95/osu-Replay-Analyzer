#include "score.h"

std::vector<OSUSTANDARD::TIMING> OSUSTANDARD::accTimings;
static Play* play;

struct KeyInfo
{
	double hitTiming;
	int key;
	std::vector<bool>* pressState;
	bool* nextNote;
};

bool sortAccTimings(OSUSTANDARD::TIMING i, OSUSTANDARD::TIMING j)
{
	return i.time < j.time;
}


int OSUSTANDARD::getJudgment(int _frameTime, int _noteTime, bool _pressState)
{
	bool hit = false;
	bool miss = true;  // assume miss to start with
	int hitTiming = _frameTime - _noteTime;

	// check if the note is long gone or not
	if (_pressState != false)	// standard has slider leniency
		if (_frameTime > _noteTime + 200)
			return 2;

	// judge
	{
		// if hit
		if (_pressState == true)
		{
			hit = BTWN(-200, hitTiming, 200);
			miss = BTWN(-300, hitTiming, -200) || BTWN(+100, hitTiming, +200);
		}

		// if release
		if (_pressState == false)
		{
			// standard has slider leniency
			hit = true;   //BTWN(-300, hitTiming, 300);
			miss = false; // BTWN(-350, hitTiming, -300) || BTWN(+300, hitTiming, +350);
		}
	}

	if (hit == true)	return 0;
	else if (miss == true)	return 1;
	else					return 3;
}

void processHit(std::vector<OSUSTANDARD::TIMING>* _accTimings, Hitobject* _currNote, KeyInfo _info)
{
	bool isHoldObject = !(_currNote->getHitobjectType() & HITOBJECTYPE::CIRCLE);
	bool different = true;
	int key = _info.key;

	// make sure it's not first object to see if this is a different hit/release
	if (_accTimings->size() != 0)
	{
		// if time and key is different
		if ((*_info.pressState)[key] == true) // if we are pressing
			different = (_currNote->getTime() != (*_accTimings)[_accTimings->size() - 1].time);
		else                                   // if we are releasing
			different = (_currNote->getEndTime() != (*_accTimings)[_accTimings->size() - 1].time);
	}

	// record only if it's not the same thing as the last
	if (different)
	{
		if ((*_info.pressState)[key] == true) // if we are pressing
			_accTimings->push_back((OSUSTANDARD::TIMING{ _currNote->getTime(), _info.hitTiming, _info.key, (*_info.pressState)[key] }));
		else						  // if we are releasing
			_accTimings->push_back((OSUSTANDARD::TIMING{ _currNote->getEndTime(), _info.hitTiming, _info.key, (*_info.pressState)[key] }));
	}

	// go to next note only if it's not a hold. We are expecting a release otherwise
	if (!isHoldObject)
	{
		*_info.nextNote = true;    // set to fetch next note if we actually hit/released the note and not a blank spot
	}
	else
	{
		if ((*_info.pressState)[key] == false) // if we were expecting release
			*_info.nextNote = true;    // set to fetch next note since we are done with this one

		(*_info.pressState)[key] = !(*_info.pressState)[key]; // we are expecting a release then now we expect a press. The same vice-versas
	}
}


void processMiss(std::vector<OSUSTANDARD::TIMING>* _accTimings, Hitobject* _currNote, KeyInfo _info)
{
	bool isHoldObject = !(_currNote->getHitobjectType() & HITOBJECTYPE::CIRCLE);
	int key = _info.key;

	if (!isHoldObject)
		_accTimings->push_back((OSUSTANDARD::TIMING{ _currNote->getTime(), (double)INT_MAX, _info.key, (*_info.pressState)[key] }));
	else
		_accTimings->push_back((OSUSTANDARD::TIMING{ _currNote->getEndTime(), (double)INT_MAX, _info.key, (*_info.pressState)[key] }));

	*_info.nextNote = true;		// set to fetch next note
	(*_info.pressState)[key] = true;	// we are expecting a press next
}

void HandleEarlyMiss(std::vector<OSUSTANDARD::TIMING>* _accTimings, Hitobject* _currNote, KeyInfo _info)
{
	int key = _info.key;

	if ((*_info.pressState)[key] == true) // if we are pressing
		_accTimings->push_back((OSUSTANDARD::TIMING{ _currNote->getTime(), (double)INT_MAX, _info.key, (*_info.pressState)[key] }));
	else						  // if we are releasing
		_accTimings->push_back((OSUSTANDARD::TIMING{ _currNote->getEndTime(), (double)INT_MAX, _info.key, (*_info.pressState)[key] }));

	*_info.nextNote = true;		// set to fetch next note
	(*_info.pressState)[key] = true;	// we are expecting a press next
}


void OSUSTANDARD::genAccTimings(Play* _play)
{
	play = _play;
	const int KEYS = 2;
	std::tuple<long, int, int> frame;
	Hitobject *prevNotes, *currNotes, *nextNotes;

	// Frame vars
	int iFrame = -1;
	bool stop = false;

	// process each column individualy
	int iNote;					// keep track where we are on the map
	bool nextNote;				// whether to fetch next note on column or not
	std::vector<bool> pressStates;			// which state we are expecting
	pressStates.resize(KEYS);

	// init vars for each column
	{
		iNote = -1;

		currNotes = getNextNote(&iNote);
		nextNotes = getNextNote(&iNote);
		
		nextNote = false;

		for (int key = 0; key < KEYS; key++)
			pressStates[key] = true;
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

				if (currNotes != nullptr)
				{
					if (pressStates[key] == true)	// if we are pressing the key
					{
						hitState = getJudgment(std::get<0>(frame), currNotes->getTime(), pressStates[key]);
						hitTiming = std::get<0>(frame) - currNotes->getTime();
					}
					else							// if we are releasing the key
					{
						hitState = getJudgment(std::get<0>(frame), currNotes->getEndTime(), pressStates[key]);
						hitTiming = std::get<0>(frame) - currNotes->getEndTime();
					}

					KeyInfo info = {};
						info.hitTiming = hitTiming;
						info.key = key;
						info.nextNote = &nextNote;
						info.pressState = &pressStates;

					switch (hitState)
					{
						case 0:		// hit
							processHit(&accTimings, currNotes, info);
							break;

						case 1:		// hit and miss
							processMiss(&accTimings, currNotes, info);
							break;

						case 2:     // never hit the note
							processMiss(&accTimings, currNotes, info);

							// get next note
							if (nextNote == true)
							{
								prevNotes = currNotes;
								currNotes = nextNotes;
								nextNotes = getNextNote(&iNote);

								nextNote = false;
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
			if (nextNote == true)
			{
				prevNotes = currNotes;
				currNotes = nextNotes;
				nextNotes = getNextNote(&iNote);

				nextNote = false;
			}
		}

		frame = getNextEvent(&iFrame); // get next press/release whatever column it is
		stop = !(iFrame < play->replay->getNumFrames()); // stop when we reached end of replay
	} while (!stop);

	std::sort(accTimings.begin(), accTimings.end(), sortAccTimings);
}

Hitobject* OSUSTANDARD::getNextNote(int* _iNote)
{
	(*_iNote) += 1;
	if (BTWN(0, *_iNote, play->beatmap->getHitobjects().size() - 1))
		return &play->beatmap->getHitobjects()[*_iNote];

	return nullptr;
}


std::tuple<long, int, int> OSUSTANDARD::getNextEvent(int* _iFrame)
{
	int currKeyMask = 0, prevKeyMask = 0;
	std::tuple<long, irr::core::vector2df, int> frame;

	*_iFrame += 1;
	if (*_iFrame > 1)
		currKeyMask = std::get<2>(play->replay->getFrame((*_iFrame) - 1));

	for (; *_iFrame < play->replay->getNumFrames(); (*_iFrame)++)
	{
		frame = play->replay->getFrame(*_iFrame);

		prevKeyMask = currKeyMask;
		currKeyMask = std::get<2>(frame);

		if ((prevKeyMask ^ currKeyMask) != 0)
			break;
	}

	unsigned int presses = (currKeyMask & ~(0x0)) & (~prevKeyMask & ~(0x0));  // curr -> press,        prev -> not pressed
	unsigned int releases = (~currKeyMask & ~(0x0)) & (prevKeyMask & ~(0x0));  // curr -> not pressed   prev -> pressed

	return std::tuple<long, int, int>(std::get<0>(frame), presses, releases);
}