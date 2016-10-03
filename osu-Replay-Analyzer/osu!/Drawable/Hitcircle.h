#ifndef HITCIRCLE_H
#define HITCIRCLE_H

#include <math.h>
#include <vector>
#include <tuple>

#include "../../ui/GuiObj.h"
#include "../../ui/Window.h"
#include "../Filestructure/Hitobject.h"
#include "../Filestructure/Beatmap.h"

class Hitcircle: public GuiObj
{
	public:
		Hitcircle(Mods* _mods, Hitobject* _hitobject, long* _viewTime);
		virtual ~Hitcircle();

	private:
		Mods *mods;
		Hitobject *hitobject;
		long *viewTime;

		SColor edgeCol;

		enum STATE
		{
			IDLE = 0,
			HIGHLIGHTED = 1,
			SELECTED = 2
		}state;

		const SColor IDLE_COLOR = SColor(255, 150, 150, 150);
		const SColor HIGHLIGHTED_COLOR = SColor(255, 255, 255, 255);
		const SColor SELECTED_COLOR = SColor(255, 255, 100, 100);

		void IdleLogic(Window &_win);
		void HighlightedLogic(Window &_win);
		void SelectedLogic(Window &_win);

		virtual void UpdateInternal(Window &_win);
		void UpdateDimentions();

		virtual void Draw(Window &_win);

		inline double getWidthRatio();
		inline double getHeightRatio();
};

#endif