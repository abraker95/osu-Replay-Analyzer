#ifndef OSU_MANIA_H
#define OSU_MANIA_H

#include <vector>

#include "../utils/mathUtils.h"
#include "../osu!/Filestructure/Play.h"
#include "osuStructs.h"

namespace OSUMANIA
{
	struct Column
	{
		std::vector<long> timings;
		std::vector<int>  indices;

		void Clear();
		void Add(long _timing, int _index);
		int getSize();
	};


	const int MANIA_END = -1;

	// dir = true -> look forward;				false -> look backward
	// start = true -> find start timing;		false -> find end timing
	int FindHitobjectAt(Play* _play, long _time, int _key, bool _dir = false);

	// Returns the key column based on the xpos of the note and the number of keys there are
	int getKey(int _xpos, int _keys);

	// Fetches the next note in the specified column. Returns nullptr and sets the index to -1 if it has reached the end
	Hitobject* getNextNoteOnColumn(Play* _play, int _column, int* _iNote);

	// Fetches the index of the next note in the specified column. Returns -1 if it has reached the end
	int getNextIndexOnColumn(std::vector<Hitobject*>& _hitobjects, int _column, int _keys, int _iNote);

	int getNumPressesOnNextTiming(Play* _play, int* _iNote);

	// Checks to see if all indices came to an end in terms of time indicated by indices _i
	bool isAtEnd(int _KEYS, std::vector<int>& _i, std::vector<Hitobject*>& _hitobjects);

	// Checks to see of _keyCheck is behind all other columns in terms of time indicated by indices _i
	bool isBehindOtherColumns(int _KEYS, int _keyCheck, std::vector<int>& _i, std::vector<Hitobject*>& _hitobjects);

	int getButtonState(int _prevKey, int _currKey, int _key);
}

#endif // !OSU_MANIA_H