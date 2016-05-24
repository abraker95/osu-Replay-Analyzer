#include "Event.h"

EventReceiver::EventReceiver()
{
	for (u32 i = 0; i<KEY_KEY_CODES_COUNT; ++i)
		KeyIsDown[i] = false;
}

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
				MouseState.positionCurr.X = event.MouseInput.X;
				MouseState.positionCurr.Y = event.MouseInput.Y;
				break;

			case EMIE_MOUSE_WHEEL:
				MouseState.wheelCurr += event.MouseInput.Wheel;
				break;

			default:
				// Wheel
				break;
		}		
	}

	// Remember whether each key is down or up
	if (event.EventType == EET_KEY_INPUT_EVENT)
	{
		KeyIsDown[event.KeyInput.Key] = event.KeyInput.PressedDown;
	}

	return false;
}

void EventReceiver::Update()
{
	MouseState.wheelMove = MouseState.wheelCurr - MouseState.wheelPrev;
	MouseState.wheelPrev = MouseState.wheelCurr;
}

const EventReceiver::SMouseState &EventReceiver::GetMouseState(void) const
{
	return MouseState;
}

int EventReceiver::getWheel(bool _mode)
{
	return MouseState.wheelMove;
}

// This is used to check whether a key is being held down
bool EventReceiver::IsKeyDown(EKEY_CODE keyCode) const
{
	return KeyIsDown[keyCode];
}