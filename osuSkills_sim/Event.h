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
			int wheelCurr, wheelPrev;
			int wheelMove;
			core::position2di Position;
			bool LeftButtonDown, LeftButtonEdge;
			SMouseState() : LeftButtonDown(false), LeftButtonEdge(false) { }
		};


		virtual bool OnEvent(const SEvent& event);
		const SMouseState &GetMouseState(void) const;
		int getWheel(bool _mode = false); // _mode = 0: 0 if no change,  _mode = 1: get the last value

	private:
		SMouseState MouseState;
};

#endif