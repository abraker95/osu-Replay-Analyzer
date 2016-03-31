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
			double wheelSpeed;
			core::position2di Position;
			bool LeftButtonDown, LeftButtonEdge;
			SMouseState() : LeftButtonDown(false), LeftButtonEdge(false) { }
		};


		virtual bool OnEvent(const SEvent& event);
		const SMouseState &GetMouseState(void) const;

	private:
		SMouseState MouseState;
};

#endif