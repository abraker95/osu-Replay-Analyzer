#ifndef EVENT_H
#define EVENT_H

#include "irrlicht/include/irrlicht.h"

using namespace irr;
using namespace core;

class EventReceiver : public IEventReceiver
{
	public:
		EventReceiver();

		struct SMouseState
		{
			int wheelCurr, wheelPrev, wheelMove;
			core::position2di positionCurr, positionPrev, positionMove;
			bool LeftButtonDown, LeftButtonEdge;
			SMouseState() : LeftButtonDown(false), LeftButtonEdge(false) { }
		};

		virtual bool OnEvent(const SEvent& event);
		void Update();

		virtual bool IsKeyDown(EKEY_CODE keyCode) const;
		const SMouseState &GetMouseState(void) const;
		int getWheel(bool _mode = false); // _mode = 0: 0 if no change,  _mode = 1: get the last value
		vector2di getCursorDelta();

	private:
		SMouseState MouseState;

		// We use this array to store the current state of each key
		bool KeyIsDown[KEY_KEY_CODES_COUNT];
};

#endif