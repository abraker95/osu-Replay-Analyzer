#ifndef DIALOG_H
#define DIALOG_H

#include "Window.h"
#include "GuiObj.h"
#include "pane.h"
#include "Button.h"

class Dialog: public GuiObj
{
	public:
		Dialog(int _xpos, int _ypos, int _width, int _height, GuiObj* _parent = nullptr);
		virtual ~Dialog();

	private:
		const int TITLEBAR_SIZE = 12;

		Pane* pane;

		Button* btnToggleMinimize;
		Button* btnClose;
		Button* titleBar;
		Button* resize;

		enum State
		{
			MINIMIZED = 0,
			MAXIMIZED = 1
		}state;


		virtual void Draw(Window& _win);
		virtual void UpdateInternal(Window& _win);
};

#endif