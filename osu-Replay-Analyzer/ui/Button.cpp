#include "Button.h"
#include "../utils/mathUtils.h"

Button::Button(GuiObj* _parent) : GuiObj(0, 0, 0, 0, _parent){}

Button::Button(int _xpos, int _ypos, int _width, int _height, GuiObj* _parent) : GuiObj(_xpos, _ypos, _width, _height, _parent)
{
	pressed = false;
	triggered = false;

	state = IDLE;
	guiType = "Button";
}

Button::~Button()
{

}

void Button::Resize(int _width, int _height)
{
	width = _width;
	height = _height;
}

bool Button::isPressed()
{
	return pressed;
}

bool Button::isTriggered()
{
	return triggered;
}


// -------- Private --------


void Button::Draw(Window &_win)
{
	switch (state)
	{
		case IDLE:
			_win.driver->draw2DRectangle(video::SColor(255, 150, 150, 150),
										 core::rect<s32>(absXpos, absYpos, absXpos + width, absYpos + height));
			break;

		case HOLD:
			_win.driver->draw2DRectangle(video::SColor(255, 100, 100, 100),
										 core::rect<s32>(absXpos, absYpos, absXpos + width, absYpos + height));
			break;

		default:
			// ???
			break;
	}
}

void Button::UpdateInternal(Window &_win)
{
	switch (state)
	{
		case IDLE:
			IdleLogic(_win);
			break;

		case HOLD:
			HoldLogic(_win);
			break;

		default:
			// ???
			break;
	}
}

void Button::IdleLogic(Window &_win)
{
	bool leftEdge = _win.reciever.GetMouseState().LeftButtonEdge;
	if (this->isMouseOnObj(_win, true) && leftEdge)
	{
		state = HOLD;
		pressed = true;
		triggered = true;
	}
}

void Button::HoldLogic(Window &_win)
{
	bool leftButton = _win.reciever.GetMouseState().LeftButtonDown;
	
	triggered = false;
	
	if (!leftButton)
	{
		state = IDLE;
		pressed = false;
	}
}