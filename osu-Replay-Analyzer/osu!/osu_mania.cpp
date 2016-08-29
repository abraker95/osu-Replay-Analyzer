#include "osu_mania.h"
#include <string>

using namespace OSUMANIA;

std::vector<std::vector<int>> hitobjectAtlas;
std::string md5;

void GenHitobjectAtlas(Play* _play)
{
	// Clear the vector
	for (std::vector<int>& indices : hitobjectAtlas)
	{
		indices.clear();
		std::vector<int>().swap(indices);
	}
	
	hitobjectAtlas.clear();
	std::vector<std::vector<int>>().swap(hitobjectAtlas);

	// Reserve space in vector
	std::vector<Hitobject*> hitobjects = _play->beatmap->getHitobjects();
	hitobjectAtlas.resize(_play->beatmap->getMods().getCS());

	// Fill in vector with list of hitobject idices correspending the column the hitobject is at
	for (int i = 0; i < hitobjects.size(); i++)
		hitobjectAtlas[getKey(hitobjects[i]->getPos().X, _play->beatmap->getMods().getCS())].push_back(i);

	// record beatmap's signature for later checking
	md5 = _play->beatmap->getMD5();
}

// dir = true -> look forward
// dir = false -> look backward
int OSUMANIA::FindHitobjectAt(Play* _play, long _time, int _key, bool _dir)
{
	// Regenerate the hitobject atlas if it's not the same beatmap
	if (_play->beatmap->getMD5() != md5)
		GenHitobjectAtlas(_play);

	int start = 0;
	int end = hitobjectAtlas[_key].size() - 2;
	int mid;

	std::vector<Hitobject*> hitobjects = _play->beatmap->getHitobjects();
	while (start <= end)
	{
		mid = (start + end) / 2;

		int icurr = hitobjectAtlas[_key].at(mid);
		int inext = hitobjectAtlas[_key].at(mid + 1);

		// Between ends of a hold object
		if (BTWN(hitobjects[icurr]->getTime(), _time, hitobjects[icurr]->getEndTime()))
		{
			// Return next object if next object's timings overlap with this one
			if (BTWN(hitobjects[inext]->getTime(), _time, hitobjects[inext]->getEndTime()))
				return hitobjectAtlas[_key].at(mid + 1);
			else
				return hitobjectAtlas[_key].at(mid);
		}

		// Between some two objects
		if (BTWN(hitobjects[icurr]->getEndTime(), _time, hitobjects[inext]->getTime()))
			return hitobjectAtlas[_key].at(mid + (long)_dir);

		if (_time < hitobjects[icurr]->getTime())
			end = mid - 1;
		else
			start = mid + 1;
	}

	return -1;
}

// Returns the key column based on the xpos of the note and the number of keys there are
int OSUMANIA::getKey(int _xpos, int _keys)
{
	float localWDivisor = 512.0f / _keys;
	return std::min((int)std::floor(_xpos / localWDivisor), _keys - 1);
}

// Fetches the next note in the specified column. Returns nullptr and sets the index to -1 if it has reached the end
Hitobject* OSUMANIA::getNextNoteOnColumn(Play* _play, int _column, int* _iNote)
{
	(*_iNote) += 1;
	for (; (*_iNote) < _play->beatmap->getHitobjects().size(); (*_iNote)++)
	{
		int KEYS = _play->beatmap->getMods().getCS();
		int noteXpos = _play->beatmap->getHitobjects()[*_iNote]->getPos().X;
		int column = getKey(noteXpos, KEYS);

		if (column == _column)
			return _play->beatmap->getHitobjects()[*_iNote];
	}

	*_iNote = MANIA_END;
	return nullptr;
}

// Fetches the index of the next note in the specified column. Returns -1 if it has reached the end
int OSUMANIA::getNextIndexOnColumn(std::vector<Hitobject*>& _hitobjects, int _column, int _keys, int _iNote)
{
	_iNote += 1;
	for (; _iNote < _hitobjects.size(); _iNote++)
	{
		int noteXpos = _hitobjects[_iNote]->getPos().X;
		int column = getKey(noteXpos, _keys);

		if (column == _column)
			return _iNote;
	}

	return MANIA_END;
}

int OSUMANIA::getNumPressesOnNextTiming(Play* _play, int* _iNote)
{
	std::vector<Hitobject*> hitojects = _play->beatmap->getHitobjects();
	if (hitojects.size() <= *_iNote + 1) return 0;

	int notes = 1;

	int currTime = hitojects[++(*_iNote)]->getTime();
	if (hitojects.size() <= *_iNote + 1) return 1;

	while (hitojects[++(*_iNote)]->getTime() <= currTime)
	{
		if (hitojects.size() <= *_iNote + 1)
			return notes;

		notes++;
	}

	--(*_iNote);
	return notes;
}

// Checks to see if all indices came to an end in terms of time indicated by indices _i
bool OSUMANIA::isAtEnd(int _KEYS, std::vector<int>& _i, std::vector<Hitobject*>& _hitobjects)
{
	bool finish = true;
	for (int key = 0; key < _KEYS; key++)
		finish &= (_i[key] >= _hitobjects.size());
	return finish;
}

// Checks to see of _keyCheck is behind all other columns in terms of time indicated by indices _i
bool OSUMANIA::isBehindOtherColumns(int _KEYS, int _keyCheck, std::vector<int>& _i, std::vector<Hitobject*>& _hitobjects)
{
	// If what we are checking against has ended, then it can't possibly be behind
	if (_i[_keyCheck] == MANIA_END) return false;

	bool get = true;
	for (int key = 0; key < _KEYS; key++)
	{
		if (key == _keyCheck) continue;      // we don't want it to compare against itself
		if (_i[key] == MANIA_END) continue;  // no need to compare against something that ended

		get &= (_hitobjects[_i[_keyCheck]]->getTime() <= _hitobjects[_i[key]]->getTime());
	}
	return get;
}

int OSUMANIA::getButtonState(int _prevKey, int _currKey, int _key)
{
	int state = 0;
	int prevKey = _prevKey & (1 << _key);
	int currKey = _currKey & (1 << _key);

	// trigger down
	if ((prevKey == 0) && (currKey != 0))
		state = 1; // pressed

	// trigger up
	if ((prevKey != 0) && (currKey == 0))
		state = 2; // release

	return state;
}