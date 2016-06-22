#ifndef OSU_STD_RENDERER
#define OSU_STD_RENDERER

#include "Window.h"
#include "GuiObj.h"
#include "Hitcircle.h"

class OsuStdRenderer: public GuiObj
{
	public:
		OsuStdRenderer(Play* _play, int* _viewTime, GuiObj* _parent);
		virtual ~OsuStdRenderer();

		enum Layer
		{
			VISIBLE = 1 << 0,
			PATHS	= 1 << 1,
			DENSITY = 1 << 2,
			REPLAY  = 1 << 3
		};

		void SetLayers(int _layer);

	private:
		void GenerateHitcircles();
		
		virtual void UpdateInternal(Window& _win);
		virtual void Draw(Window& _win);

		void RenderVisible(Window& _win);
		void RenderPaths(Window& _win);
		void RenderDensities(Window& _win);
		void RenderReplay(Window& _win);

		std::vector<Hitcircle*> hitCircles;

		Play* play;
		int* viewTime;
		Layer layerState;
};

#endif