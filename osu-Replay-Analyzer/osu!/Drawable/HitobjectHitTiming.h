#ifndef HITOBJECTHITTIMING_H
#define HITOBJECTHITTIMING_H

#include "../../ui/Window.h"
#include "../../ui/GuiObj.h"

class HitObjectHitTiming : public GuiObj
{
	public:
		HitObjectHitTiming(int _xpos, int _ypos, int _width, int _height, double* _zoom, Window& _win);
		virtual ~HitObjectHitTiming();

	private:
		virtual void UpdateInternal(Window& _win);
		virtual void Draw(Window& _win);

		void IdleLogic(Window &_win);
		void HighlightedLogic(Window &_win);

		//const SColor IDLE_COLOR = SColor(255, 255, 255, 255);
		//const SColor HIGHLIGHTED_COLOR = SColor(255, 255, 255, 0);

		enum STATE
		{
			IDLE = 0,
			HIGHLIGHTED = 1,
			SELECTED = 2
		}state;

		double *zoom;
};

#endif