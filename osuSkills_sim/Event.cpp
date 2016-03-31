#include "Event.h"

EventReceiver::EventReceiver(){}

bool EventReceiver::OnEvent(const SEvent& event)
{
	MouseState.LeftButtonEdge = false;
	MouseState.wheelSpeed = 0.0;

	// Remember the mouse state
	if (event.EventType == irr::EET_MOUSE_INPUT_EVENT)
	{
		switch (event.MouseInput.Event)
		{
			case EMIE_LMOUSE_PRESSED_DOWN:
				if (MouseState.LeftButtonDown == false)
					MouseState.LeftButtonEdge = true;
				MouseState.LeftButtonDown = true;
				break;

			case EMIE_LMOUSE_LEFT_UP:
				MouseState.LeftButtonDown = false;
				break;

			case EMIE_MOUSE_MOVED:
				MouseState.Position.X = event.MouseInput.X;
				MouseState.Position.Y = event.MouseInput.Y;
				break;

			case EMIE_MOUSE_WHEEL:
				MouseState.wheelSpeed = event.MouseInput.Wheel;
				break;

			default:
				// Wheel
				break;
		}
	}

	return false;
}

const EventReceiver::SMouseState &EventReceiver::GetMouseState(void) const
{
	return MouseState;
}
