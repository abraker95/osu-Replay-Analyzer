#ifndef BUTTON_H
#define BUTTON_H

#include "Window.h"
#include "GuiObj.h"

class Button: public GuiObj
{
	public:
		Button(GuiObj* _parent = nullptr);
		Button(int _xpos, int _ypos, int _width, int _height, std::string _label = "", GuiObj* parent = nullptr);
		virtual ~Button();

		void Resize(int _width, int _height);

		bool isPressed();
		bool isTriggered();

	private:
		bool pressed, triggered;
		std::string label;

		enum State
		{
			IDLE = 0,
			HOLD = 1
		}state;

		virtual void Draw(Window &_win);
		void DrawLabel(Window &_win);

		virtual void UpdateInternal(Window &_win);

		void IdleLogic(Window &_win);
		void HoldLogic(Window &_win);
};

#endif