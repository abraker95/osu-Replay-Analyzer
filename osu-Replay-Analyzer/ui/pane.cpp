#include "Pane.h"

Pane::Pane(int _xpos, int _ypos, int _width, int _height, int _virtWidth, int _virtHeight, GuiObj* _parent) : GuiObj(_xpos, _ypos, _width, _height, _parent)
{
	virtWidth = _virtWidth;
	virtHeight = _virtHeight;

	verScrollbar = new Scrollbar(0, 0, Scrollbar::VERTICAL, _height, _virtHeight, this);
		verScrollbar->ClipPosTo(TOPRIGHT);
		verScrollbar->addClipDimTo(BTM);
		verScrollbar->setMargin(0, 12);

	horScrollbar = new Scrollbar(0, 0, Scrollbar::HORIZANTAL, _width, _virtWidth, this);
		horScrollbar->ClipPosTo(BTMLEFT);
		horScrollbar->addClipDimTo(BTM);
		horScrollbar->addClipDimTo(RIGHT);
		horScrollbar->setMargin(12, 0);

	SetRelativeGuiLayer(verScrollbar, this);

	guiType = "Pane";
}

Pane::~Pane()
{
	delete verScrollbar;
	delete horScrollbar;
}

void Pane::addObj(GuiObj* _obj)
{
	_obj->setParent(this);
	objs.push_back(_obj);
}

void Pane::setSize(int _width, int _height)
{
	width = _width;
	height = _height;
}

void Pane::setVirtSize(int _width, int _height)
{
	virtWidth = _width;
	virtHeight = _height;
}


// -------- Private ----------


void Pane::Draw(Window& _win)
{
	_win.driver->draw2DRectangle(video::SColor(255, 100, 100, 200),
								 core::rect<s32>(absXpos, absYpos, absXpos + width, absYpos + height));
}

void Pane::UpdateInternal(Window& _win)
{
	if (virtHeight > height) verScrollbar->setVisible(true);
	else					 verScrollbar->setVisible(false);

	if (virtWidth > width)   horScrollbar->setVisible(true);
	else					 horScrollbar->setVisible(false);

	for (int i = 0; i < objs.size(); i++)
	{
		if (objs[i] != nullptr)
			objs[i]->Update(_win);
	}

	verScrollbar->Update(_win);
	horScrollbar->Update(_win);
}