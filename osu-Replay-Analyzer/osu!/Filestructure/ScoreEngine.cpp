#include "ScoreEngine.h"
#include <math.h>

ScoreEngine::ScoreEngine(Play* _play)
{
	play = _play;

	if((play->beatmap != nullptr) && (play->replay != nullptr))
		genScores();
}

double ScoreEngine::getTotalAccScore()
{
	double score = 0.0;
	double maxScore = 0.0;
	double missesPunisher = 1.0;

	// max score calc
	for (int i = 0; i < accTimings.size(); i++)
	{
		if (diffScores[i].timingDiff != -1) // if not missed
		{
			maxScore += (diffScores[i].timingDiff * log(accTimings.size()))/missesPunisher;

			if (missesPunisher > 1.0)
				missesPunisher /= 2.0;
			else
				missesPunisher = 1.0;
		}
		else missesPunisher *= 2.0;
	}
	
	// score calc
	for (int i = 0; i < accTimings.size(); i++)
	{
		if (diffScores[i].timingDiff != -1) // if not missed
		{
			score += (Time2AccScore(abs(accTimings[i].timingDiff)) * diffScores[i].timingDiff * log(accTimings.size()))/missesPunisher;
			
			if(missesPunisher > 1.0)
				missesPunisher /= 2.0;
			else
				missesPunisher = 1.0;
		}
		else missesPunisher *= 2.0;
	}

	std::pair<double, double> totalScore;
		totalScore.first = (score / maxScore) * 1000000.0;
		totalScore.second = score / 100.0;

double ScoreEngine::getTotalScore()
{
	// (diffScore[i] * accScore[i] * log10(i)) / missesPunisher 
	return 0; /// \TODO
}

void ScoreEngine::genScores()
{
	genAccTimings();
	genDifficulties();
}

void ScoreEngine::genAccTimings()
{
	std::tuple<long, int, int> frame;
	std::vector<Hitobject*> prevNotes, currNotes, nextNotes;

	// Frame vars
	int iFrame = -1;
	bool stop = false;

	// process each column individualy
	int KEYS = play->beatmap->getDiff().cs;		/// \TODO: Detect number of keys
	int* iNote = new int[KEYS];
	bool* nextNote = new bool[KEYS];

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
			// process key presess
			if (keyPresses & (1 << key))
			{
				if(currNotes[key] != nullptr) // make sure we have a valid note
				{
					std::pair<double, double> ODinterval = ScoreEngine::getODms(prevNotes[key], currNotes[key], nextNotes[key], true);
					bool isHoldObject = currNotes[key]->getHitobjectType() & (~HITOBJECTYPE::CIRCLE);
					int hitTiming = std::get<0>(frame) - currNotes[key]->getTime();

					if (BTWN(-100, hitTiming, 100)) // record if it is within accepted hit/release period
					{
						bool different = true;
						if (accTimings.size() != 0)
						{
							// if time and key is different
							different = (currNotes[key]->getTime() != accTimings[accTimings.size() - 1].time) || (key != accTimings[accTimings.size() - 1].key);
						}

						if(different) // record only if it's not the same thing as the last
							accTimings.push_back((TIMING{ currNotes[key]->getTime(), (double)hitTiming, key, true }));
						
						if(currNotes[key]->getHitobjectType() & HITOBJECTYPE::CIRCLE) // go to next note only if it's not a hold. We are expecting a release otherwise
							nextNote[key] = true;  // set to fetch next note if we actually hit/released the note and not a blank spot
					}
					else if (hitTiming > 100) // else record a miss as infinite timing period
					{
						bool different = true;
						if (accTimings.size() != 0)
						{
							// if time and key is different
							different = (currNotes[key]->getTime() != accTimings[accTimings.size() - 1].time) || (key != accTimings[accTimings.size() - 1].key);
						}

						if (different) // record only if it's not the same thing as the last
							accTimings.push_back((TIMING{ currNotes[key]->getTime(), (double)INT_MAX, key, true }));
						nextNote[key] = true;  // set to fetch next note if we actually hit/released the note and not a blank spot
					}
				}
			}

			// process key release
			if (keyReleases & (1 << key))
			{
				if (currNotes[key] != nullptr) // make sure we have a valid note
				{
					std::pair<double, double> ODinterval = ScoreEngine::getODms(prevNotes[key], currNotes[key], nextNotes[key], false);
					bool isHoldObject = !(currNotes[key]->getHitobjectType() & HITOBJECTYPE::CIRCLE);
					int hitTiming = std::get<0>(frame) - currNotes[key]->getEndTime();
					
					// count releases only for holds
					if (isHoldObject)
					{
						if (BTWN(-100, hitTiming, 100)) // record if it is within accepted hit/release period
						{
							accTimings.push_back((TIMING{ currNotes[key]->getEndTime(), (double)hitTiming, key, false }));
							nextNote[key] = true;  // set to fetch next note if we actually hit/released the note and not a blank spot
						}
						else if (hitTiming > 100) // else record a miss as infinite timing period
						{
							accTimings.push_back((TIMING{ currNotes[key]->getEndTime(), (double)INT_MAX, key, false }));
							nextNote[key] = true;  // set to fetch next note if we missed the note
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
				if(currNotes[key] != nullptr)
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
}

void ScoreEngine::genMaxTappingDiffs()
{
	int KEYS = play->beatmap->getDiff().cs;
	std::vector<TIMING> diffScore;
		diffScore.resize(KEYS);
	std::vector<int> hitPeriodPrev, hitPeriodCurr;
		hitPeriodPrev.resize(KEYS);
		hitPeriodCurr.resize(KEYS);


	for (int key = 0; key < KEYS; key++)
	{
		diffScore[key].key = key;
		diffScore[key].timingDiff = 0;
	}

	// first note is nothing
	diffPlyScores.push_back(TIMING{ play->beatmap->hitObjects[0]->getTime(), -1, -1, false });
	if (play->beatmap->hitObjects[0]->getHitobjectType() & (~HITOBJECTYPE::CIRCLE))
		diffPlyScores.push_back(TIMING{ play->beatmap->hitObjects[0]->getTime(), -1, -1, false });

	for (int i = 1; i < play->beatmap->hitObjects.size(); i++)
	{
		bool isHoldObject = play->beatmap->hitObjects[i]->getHitobjectType() & (~HITOBJECTYPE::CIRCLE);
		int noteXpos = play->beatmap->hitObjects[i]->getPos().X;
		int column = (noteXpos - 64) / 128;

		// never a miss
		if (true)
		{
			hitPeriodPrev[column] = hitPeriodCurr[column];
			hitPeriodCurr[column] = play->beatmap->hitObjects[i]->getTime();
			double kps = 1000.0 / (hitPeriodCurr[column] - hitPeriodPrev[column]); // keys per second

			diffScore[column].timingDiff = PressStrain(kps);
			diffScore[column].time = hitPeriodCurr[column];

			// average out the overall diff of pressing this
			double frameDiff = 0.0;
			for (int key = 0; key < KEYS; key++)
				frameDiff += diffScore[key].timingDiff;
			frameDiff /= KEYS;

			diffMaxScores.push_back((TIMING{ play->beatmap->hitObjects[i]->getTime(), frameDiff, column, false }));

			// if it's a slider, then another frame for release
			if (isHoldObject)
			{
				hitPeriodPrev[column] = hitPeriodCurr[column];
				hitPeriodCurr[column] = play->beatmap->hitObjects[i]->getEndTime();
				double kps = 1000.0 / (hitPeriodCurr[column] - hitPeriodPrev[column]); // keys per second

				diffScore[column].timingDiff = PressStrain(kps) * 0.1;

				// average out the overall diff of pressing this
				double frameDiff = 0.0;
				for (int key = 0; key < KEYS; key++)
					frameDiff += diffScore[key].timingDiff;
				frameDiff /= KEYS;

				diffMaxScores.push_back((TIMING{ play->beatmap->hitObjects[i]->getEndTime(), frameDiff, column, false }));
			}
		}
	}
}


void ScoreEngine::genPlyTappingDiffs()
{
	int KEYS = play->beatmap->getDiff().cs;
	TIMING* diffScore = new TIMING[KEYS];
	
	for (int key = 0; key < KEYS; key++)
	{
		diffScore[key].key = key;
		diffScore[key].timingDiff = 0;
	}

	// first note is nothing
	diffPlyScores.push_back(TIMING{ accTimings[0].time, -1, -1, false });

	for (int i = 1; i < accTimings.size(); i++)
	{
		// if it's not a miss
		if (accTimings[i].timingDiff != INT_MAX)
		{
			for (int key = 0; key < KEYS; key++)
			{
				// process column hit difficulty based on last time hit
				if ((accTimings[i].key == key))
				{
					double hitPeriodPrev = diffScore[key].time;
					double hitPeriodCurr = accTimings[i].time + accTimings[i].timingDiff;
					double kps = 1000.0 / (hitPeriodCurr - hitPeriodPrev); // keys per second

					if(accTimings[i].press == true) // if pressed
						diffScore[key].timingDiff = /*(kps / 10.0)*diffScore[key].timingDiff +*/ PressStrain(kps);
					else // released
						diffScore[key].timingDiff = /*(kps / 10.0)*diffScore[key].timingDiff +*/ PressStrain(kps) * 0.5;

					diffScore[key].time = hitPeriodCurr;
				}
			}

			// average out the overall diff of pressing this
			double frameDiff = 0.0;
			for (int key = 0; key < KEYS; key++)
				frameDiff += diffScore[key].timingDiff;
			frameDiff /= KEYS;

			diffPlyScores.push_back((TIMING{ accTimings[i].time, frameDiff, -1, false }));
		}
		else // miss
		{
			// place holder so diffScores and accTimings match up
			diffPlyScores.push_back(TIMING{ accTimings[i].time, -1, -1, false });
		}
	}

	delete diffScore;
}

// modeled here: https://www.desmos.com/calculator/ok1qgmfe9f
double ScoreEngine::Time2AccScore(int _ms)
{
	double scale = 0.5; // 0.5 @ 60 FPS
	double o = sqrt(-pow((50.0 / 3.0), 2.0) / (2.0 * log(scale)));  

	return exp(-pow(_ms, 2.0) / (2.0 * pow(o, 2.0)));
}

double ScoreEngine::PressStrain(int _ms)
{
	return exp((2.0*_ms) / 7.0 - 1.0);
}

std::tuple<long, int, int> ScoreEngine::getNextEvent(int* _iFrame)
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

	unsigned int presses  = ( currKeyMask & ~(0x0)) & (~prevKeyMask & ~(0x0));  // curr -> press,        prev -> not pressed
	unsigned int releases = (~currKeyMask & ~(0x0)) & ( prevKeyMask & ~(0x0));  // curr -> not pressed   prev -> pressed

	return std::tuple<long, int, int>(std::get<0>(frame), presses, releases);
}

Hitobject* ScoreEngine::getNextNoteOnColumn(int _column, int* _iNote)
{
	(*_iNote) += 1;
	for (; (*_iNote) < play->beatmap->hitObjects.size(); (*_iNote)++)
	{
		int noteXpos = play->beatmap->hitObjects[*_iNote]->getPos().X;
		int column = (noteXpos - 64) / 128;

		if (column == _column)
			return play->beatmap->hitObjects[*_iNote];
	}

	return nullptr;
}

// note's valid hitRange
std::pair<double, double> ScoreEngine::getODms(Hitobject* _prevNote, Hitobject* _currNote, Hitobject* _nextNote, bool _press)
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
		// cap interval to -100ms -> +100ms
		BOUND(_currNote->getTime() - 100, earliestTime, _currNote->getTime()      );
		BOUND(_currNote->getTime(),       latestTime,   _currNote->getTime() + 100);
	}
	else // release
	{
		// cap interval to -100ms -> +100ms
		BOUND(_currNote->getEndTime() - 100, earliestTime, _currNote->getEndTime()      );
		BOUND(_currNote->getEndTime(),       latestTime,   _currNote->getEndTime() + 100);
	}

	return std::pair<double, double>(earliestTime, latestTime);
}