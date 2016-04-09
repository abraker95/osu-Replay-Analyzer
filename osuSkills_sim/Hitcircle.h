#ifndef HITCIRCLE_H
#define HITCIRCLE_H

#include <math.h>
#include <vector>
#include "Object.h"
#include "Window.h"

class Hitcircle: private Object
{
	private:
		int x, y;		// in osu!px
		double radius;   // in osu!px
		double t;		// in ms
		SColor edgeCol;
		
		enum state
		{
			IDLE = 0,
			HIGHLIGHTED = 1,
			SELECTED = 2
		}state;

		const SColor IDLE_COLOR		   = SColor(255, 150, 150, 150);
		const SColor HIGHLIGHTED_COLOR = SColor(255, 255, 255, 255);
		const SColor SELECTED_COLOR    = SColor(255, 255, 100, 100);
		
		void IdleLogic(Window &_win);
		void HighlightedLogic(Window &_win);
		void SelectedLogic(Window &_win);

		void setCS_px(int _CS_px);

		void update(Window &_win);

	public:
		Hitcircle(int _x, int _y, int _t);

		void Draw(Window &_win, int _xOffset, int _yOffset, int _time, double _AR, double _CS, bool _hidden);

		bool isVisible(int _time, double _AR, bool _hidden);

		position2di getPos(bool _absolute = false);

		int getTime();

		double getSizePx();
};

int getHitcircleAt(std::vector<Hitcircle> _hitcircles, int _time);

int getNumVisibleAt(std::vector<Hitcircle> _hitcircles, int _index, double _AR, bool _hidden);

#endif