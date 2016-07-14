#ifndef OSUSTATS_H
#define OSUSTATS_H

#include <vector>

#include "../../ui/Window.h"
#include "../../ui/GuiObj.h"
#include "../Filestructure/Play.h"
#include "TimingObject.h"

class TimingGraph : public GuiObj
{
	public:
		TimingGraph(Window& _win, Play* _play); // _res = resolution in px/ms
		virtual ~TimingGraph();

		enum LAYER
		{
			REFRESHRATE_TICKS		= 1 << 0,
			HITOBJECT_VISIBILTITY	= 1 << 1,
			HITOBJECT_HIT_TIMINGS	= 1 << 2

		};

		void EnableLayer(LAYER _layer);
		void DisableLayer(LAYER _layer);
		int getViewTime();
		bool ViewTimeChanged();

		double getZoom();
		std::pair<int, int> getViewTimes();

	private:
		void GenerateVisibilityTimings();

		virtual void UpdateInternal(Window& _win);
		virtual void Draw(Window& _win);

		void IdleLogic(Window &_win);
		void HighlightedLogic(Window &_win);

		int getVirtualxPos();
		int getMarkerPos();

		void UpdateUserControls(Window& _win);
		void UpdateZoom(Window& _win);
		void UpdateViewTime(Window& _win);

		void drawTimingsText(Window& _win);
		void drawViewTimeMarker(Window& _win);
		void drawHitobjectVisibilityTimings(Window &_win);
		void drawHitobjectHitTimings(Window &_win);
		void drawRefreshRateTimings(Window &_win, double _FPS);

		const SColor IDLE_COLOR = SColor(255, 255, 255, 255);
		const SColor HIGHLIGHTED_COLOR = SColor(255, 255, 255, 0);

		enum STATE
		{
			IDLE = 0,
			HIGHLIGHTED = 1,
			SELECTED = 2
		}state;

		SColor edgeCol;
		Play* play;
		int* res;
		
		const int HEIGHT = 25;
		int layerMask;
		double currViewTime, prevViewTime;
		double zoom;
		bool autoPlay;

		std::vector<TimingObject*> timingObjects;
};


#endif