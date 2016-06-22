#include "Scrollbar.h"
#include "mathUtils.h"

#include <algorithm>
#include <iostream>

// \TODO: GuiObj needs actual dimensions of the object
// \TODO: Support for a horizantal scrollbar
Scrollbar::Scrollbar(int _xpos, int _ypos, ORIENTATION _scrollBarOrientation, int _pxLength, int _pxAreaLength, GuiObj* _parent) : GuiObj(_xpos, _ypos, WIDTH, 0, _parent)
{
	pxLength = _pxLength;
	pxAreaLength = _pxAreaLength;
	scrollBarOrientation = _scrollBarOrientation;

	switch (_scrollBarOrientation)
	{
		case HORIZANTAL:
			height = WIDTH;
			width = _pxLength;

			btnUp = new Button(0, 0 , WIDTH, WIDTH, this);
				btnUp->ClipPosTo(TOPLEFT);
			btnDown = new Button(0, 0, WIDTH, WIDTH, this);
				btnDown->ClipPosTo(TOPRIGHT);
			scroller = new Button(0, 0, this->getScrollerLength(), WIDTH, this);
			break;

		case VERTICAL:
			height = _pxLength;
			width = WIDTH;
			
			btnUp = new Button(0, 0, WIDTH, WIDTH, this);
				btnUp->ClipPosTo(TOPLEFT);
			btnDown = new Button(0, 0, WIDTH, WIDTH, this);
				btnDown->ClipPosTo(BTMLEFT);
			scroller = new Button(0, 0, WIDTH, this->getScrollerLength(), this);
			break;

		default:
			btnUp = nullptr;
			btnDown = nullptr;
			scroller = nullptr;
			break;
	};

	
	scrollPos = _ypos + WIDTH + this->getScrollerLength()/2;

	guiType = "Scrollbar";
}

Scrollbar::~Scrollbar()
{
	delete this->btnUp;
	delete this->btnDown;
	delete this->scroller;
}

void Scrollbar::setPxLength(int _length)
{
	pxLength = _length;
}

void Scrollbar::setPxAreaLength(int _length)
{
	pxAreaLength = _length;
}

double Scrollbar::getScrollerPercent()
{
	return getPercent(ypos + getScrollerLength() / 2 + WIDTH, scrollPos, ypos - getScrollerLength() / 2 + pxLength - WIDTH);
}



// --------- Private -------------

int Scrollbar::getScrollerLength()
{
	double length = this->getScrollerRatio()*(pxLength - (WIDTH * 2));
		length = BOUND(12.0, length, pxLength - (WIDTH * 2));	
	return length;
}

// \TODO: style customization
void Scrollbar::Draw(Window &_win)
{
	switch (scrollBarOrientation)
	{
		case HORIZANTAL:
			_win.driver->draw2DRectangle(
				video::SColor(255, 200, 200, 200),
				core::rect<s32>(
					 this->absXpos /*+ WIDTH / 4*/, 
					 this->absYpos, 
					 this->absXpos + this->pxLength - WIDTH, 
					 this->absYpos + WIDTH /*- WIDTH / 4*/));
			break;

		case VERTICAL:
			_win.driver->draw2DRectangle(
				video::SColor(255, 200, 200, 200),
				core::rect<s32>(
					this->absXpos /*+ WIDTH / 4*/, 
					this->absYpos + WIDTH, 
					this->absXpos + WIDTH /*- WIDTH / 4*/, 
					this->absYpos + this->pxLength));
			break;

		default:
			// ;-;
			break;
	};
}


void Scrollbar::UpdateInternal(Window &_win)
{
	//xpos = _win.reciever.GetMouseState().positionCurr.X;
	//ypos = _win.reciever.GetMouseState().positionCurr.Y;

	this->UpdateScrollerPos(_win);
	this->UpdateScrollerLength();

	btnUp->Update(_win);
	btnDown->Update(_win);
	scroller->Update(_win);
	
}

void Scrollbar::UpdateScrollerPos(Window &_win)
{
	double DELTA = this->getScrollerRatio();
	
	// determine new scroller position
	if (btnUp->isPressed())
	{
		scrollPos -= DELTA;
	}
	else if (btnDown->isPressed())
	{
		scrollPos += DELTA;
	}
	else if(scroller->isPressed()) // we are dragging scroller
	{
		position2di pos = _win.reciever.GetMouseState().positionCurr - position2di(absXpos, absYpos);

		switch (scrollBarOrientation)
		{
			case HORIZANTAL:
				scrollPos = pos.X;
				break;
			case VERTICAL:
				scrollPos = pos.Y;
				break;
			default:
				// ;-;
				break;
		};
	}

	// Update scroller position
	switch (scrollBarOrientation)
	{
		case HORIZANTAL:
			this->scrollPos = BOUND(this->getScrollerLength() / 2 + WIDTH, scrollPos, -this->getScrollerLength() / 2 + pxLength - WIDTH);
			scroller->setPos(scrollPos - this->getScrollerLength() / 2, 0);
			break;
		case VERTICAL:
			this->scrollPos = BOUND(this->getScrollerLength() / 2 + WIDTH, scrollPos, -this->getScrollerLength() / 2 + pxLength - WIDTH);
			scroller->setPos(0, scrollPos - getScrollerLength() / 2);
			break;
		default:
			// ;-;
			break;
	};
}

void Scrollbar::UpdateScrollerLength()
{
	// update pxLength
	switch (scrollBarOrientation)
	{
		case HORIZANTAL:
			pxLength = width;
			break;

		case VERTICAL:
			pxLength = height;
			break;

		default:
			break;
	};

	// Update scroller size
	int length = getScrollerLength();
	switch (scrollBarOrientation)
	{
		case HORIZANTAL:
			scroller->Resize(length, WIDTH);
			break;

		case VERTICAL:
			scroller->Resize(WIDTH, length);
			break;

		default:
			break;
	};
}

double Scrollbar::getScrollerRatio()
{
	return (double)pxLength/ (double)pxAreaLength;
}