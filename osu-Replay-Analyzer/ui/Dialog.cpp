#include "Dialog.h"

#include <iostream>

Dialog::Dialog(int _xpos, int _ypos, int _width, int _height, GuiObj* _parent) : GuiObj(_xpos, _ypos, _width, _height, _parent)
{
	pane = new Pane(0, TITLEBAR_SIZE, 0, 0, _width, _height, this);
		pane->ClipPosTo(TOPLEFT);
		pane->addClipDimTo(BTM);
		pane->addClipDimTo(RIGHT);

	btnToggleMinimize = new Button(-24 - 4, 0, 12, 12, this);
		btnToggleMinimize->ClipPosTo(TOPRIGHT);

	btnClose = new Button(-12 - 2, 0, 12, 12, this);
		btnClose->ClipPosTo(TOPRIGHT);

	titleBar = new Button(0, 0, _width, TITLEBAR_SIZE, this);
		titleBar->ClipPosTo(TOPLEFT);
		titleBar->addClipDimTo(RIGHT);

	resize = new Button(0, 0, 12, 12, this);
		resize->ClipPosTo(BTMRIGHT);

	SetRelativeGuiLayer(resize, pane);
	SetRelativeGuiLayer(btnClose, titleBar);
	SetRelativeGuiLayer(btnToggleMinimize, titleBar);

	state = MAXIMIZED;
}

Dialog::~Dialog()
{
	delete pane;
	delete btnToggleMinimize;
	delete btnClose;
	delete titleBar;
	delete resize;
}

// -------- Private ----------

void Dialog::Draw(Window& _win)
{
	pane->Update(_win);
	titleBar->Update(_win);

	btnClose->Update(_win);
	btnToggleMinimize->Update(_win);
	resize->Update(_win);
}

void Dialog::UpdateInternal(Window& _win)
{
	if (titleBar->isPressed())
	{
		position2di mouseDelta = _win.reciever.getCursorDelta();
		this->xpos += mouseDelta.X;
		this->ypos += mouseDelta.Y;
	}
	else if (btnClose->isTriggered())
	{
		// \TODO: Close dialog
	}
	else if (btnToggleMinimize->isTriggered())
	{
		switch (this->state)
		{
			case MINIMIZED:
				this->state = MAXIMIZED;
				resize->setVisible(true);
				pane->setVisible(true);
				break;

			case MAXIMIZED:
				this->state = MINIMIZED;
				resize->setVisible(false);
				pane->setVisible(false);
				break;

			default:
				break;
		}
	}
	else if (resize->isPressed())
	{
		position2di mouseDelta = _win.reciever.getCursorDelta();
		this->width += mouseDelta.X;
		this->height += mouseDelta.Y;
	}
}