#ifndef OSU_MANIA_H
#define OSU_MANIA_H

#include "../utils/mathUtils.h"
#include "../osu!/Filestructure/Play.h"
#include "osuStructs.h"

namespace OSUMANIA
{
	const int MANIA_END = -1;

	// Returns the key column based on the xpos of the note and the number of keys there are
	inline int getKey(int _xpos, int _keys)
	{
		float localWDivisor = 512.0f / _keys;
		return std::min((int)std::floor(_xpos / localWDivisor), _keys - 1);
	}

	// Fetches the next note in the specified column. Returns nullptr and sets the index to -1 if it has reached the end
	inline Hitobject* getNextNoteOnColumn(Play* _play, int _column, int* _iNote)
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
}

#endif // !OSU_MANIA_H