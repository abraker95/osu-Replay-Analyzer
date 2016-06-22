#include "Event.h"
#include <iostream>

EventReceiver::EventReceiver()
{
	for (u32 i = 0; i < KEY_KEY_CODES_COUNT; ++i)
	{
		keyState[i] = false;
		currKeyState[i] = false;
		prevKeyState[i] = false;
	}
		
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
				break;
		}		
	}

	// Remember whether each key is down or up
	if (event.EventType == EET_KEY_INPUT_EVENT)
	{
		keyState[event.KeyInput.Key] = event.KeyInput.PressedDown;
	}

	return false;
}

void EventReceiver::Update()
{
	MouseState.wheelMove = MouseState.wheelCurr - MouseState.wheelPrev;
	MouseState.wheelPrev = MouseState.wheelCurr;

	MouseState.positionMove = MouseState.positionCurr - MouseState.positionPrev;
	MouseState.positionPrev = MouseState.positionCurr;

	for (u32 i = 0; i < KEY_KEY_CODES_COUNT; ++i)
	{
		 prevKeyState[i] = currKeyState[i];
		 currKeyState[i] = keyState[i];
	}
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
	return currKeyState[keyCode];
}

bool EventReceiver::IsKeyEdgeDown(EKEY_CODE keyCode) const
{
	return (prevKeyState[keyCode] ^ currKeyState[keyCode]) && (currKeyState[keyCode] == true);
}

bool EventReceiver::IsKeyEdgeUp(EKEY_CODE keyCode) const
{
	return (prevKeyState[keyCode] ^ currKeyState[keyCode]) && (currKeyState[keyCode] == false);
}

vector2di EventReceiver::getCursorDelta()
{
	return MouseState.positionMove;
}