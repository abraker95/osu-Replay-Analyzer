#ifndef TIMINGOBJECT
#define TIMINGOBJECT

#include "GuiObj.h"
#include "Window.h"
#include "Beatmap.h"
#include "Hitobject.h"

class TimingGraph;

class TimingObject : public GuiObj
{
	public:
		TimingObject(int _xpos, int ypos, int _width, int _height, Hitobject* _hitobject, Beatmap::Modifier* _modifier, Beatmap::Diff* _diff, TimingGraph* _timingGraph);

	private:
		virtual void UpdateInternal(Window& _win);
		void UpdatePosDim();
		
		virtual void Draw(Window& _win);

		void IdleLogic(Window &_win);
		void HighlightedLogic(Window &_win);
		void SelectedLogic(Window &_win);

		void UpdateHitObjectTiming();

		const SColor IDLE_COLOR = SColor(255, 255, 150, 255);
		const SColor HIGHLIGHTED_COLOR = SColor(255, 255, 200, 255);
		const SColor SELECTED_COLOR = SColor(255, 255, 100, 255);
		SColor color;

		TimingGraph* timingGraph;
		Beatmap::Diff* diff;
		Beatmap::Modifier* modifier;
		Hitobject* hitobject;

		int origXpos, origWidth;

		enum STATE
		{
			IDLE = 0,
			HIGHLIGHTED = 1,
			SELECTED = 2
		}state;
};

#endif