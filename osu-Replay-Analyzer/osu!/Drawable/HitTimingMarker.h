#ifndef HITTIMINGMARKER_H
#define HITTIMINGMARKER_H

#include <vector>

#include "../../ui/Window.h"
#include "../../ui/GuiObj.h"
#include "../Filestructure/Play.h"

class HitTimingMarker : public GuiObj
{
	public:
		HitTimingMarker(int _xpos, int _ypos, Play* _play, int* _viewTime);

	private:
		virtual void Draw(Window& _win);
		virtual void UpdateInternal(Window& _win);

		void DrawStdTimings(Window& _win);
		void DrawCtBTimings(Window& _win);
		void DrawTaikoTimings(Window& _win);
		void DrawManiaTimings(Window& _win);
		void DrawDodgeTimings(Window& _win);

		void UpdateUserControls(Window& _win);
		void UpdateZoomTime(Window& _win);
		void UpdateZoomView(Window& _win);

		void IdleLogic(Window &_win);
		void HighlightedLogic(Window &_win);

		enum STATE
		{
			IDLE = 0,
			HIGHLIGHTED = 1,
			SELECTED = 2
		}state;

		const SColor IDLE_COLOR = SColor(255, 0, 0, 0);
		const SColor HIGHLIGHTED_COLOR = SColor(255, 255, 255, 0);

		SColor edgeCol;
		double timeZoom = 1000;
		double viewZoom = 250.0;

		int* viewTime;
		Play* play;
};

#endif