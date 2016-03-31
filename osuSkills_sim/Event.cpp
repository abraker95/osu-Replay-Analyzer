#include "Event.h"

EventReceiver::EventReceiver(){}

bool EventReceiver::OnEvent(const SEvent& event)
{
	MouseState.LeftButtonEdge = false;
	
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
				MouseState.wheelCurr += event.MouseInput.Wheel;
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

int EventReceiver::getWheel(bool _mode)
{
	if (MouseState.wheelCurr == MouseState.wheelPrev)
	{
		if (_mode == false)
			return 0;
		else
			return MouseState.wheelMove;
	}
	else
	{
		MouseState.wheelMove = MouseState.wheelCurr - MouseState.wheelPrev;
		MouseState.wheelPrev = MouseState.wheelCurr;
		return MouseState.wheelMove;
	}	
}