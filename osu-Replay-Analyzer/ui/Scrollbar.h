#ifndef SCROLLBAR_H
#define SCROLLBAR_H

#include "Window.h"
#include "Button.h"
#include "GuiObj.h"

class Scrollbar: public GuiObj
{
	public:
		static const int WIDTH = 12;
		enum ORIENTATION
		{
			VERTICAL = 0,
			HORIZANTAL = 1
		};

		Scrollbar(int _xpos, int _ypos, ORIENTATION _scrollBarOrientation, int _pxLength, int _pxAreaLength, GuiObj* _parent = nullptr);
		virtual ~Scrollbar();
		
		void setPxLength(int _length);
		void setPxAreaLength(int _length);

		double getScrollerPercent();

	private:
		int pxLength, pxAreaLength;

		double scrollPos;
		ORIENTATION scrollBarOrientation;

		Button* btnUp;
		Button* btnDown;
		Button* scroller;

		virtual void Draw(Window &_win);
		virtual void UpdateInternal(Window &_win);

		void UpdateScrollerPos(Window &_win);
		void UpdateScrollerLength();

		int getScrollerLength();
		double getScrollerRatio();
};

#endif