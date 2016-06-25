#ifndef HIT_TIMING_GRAPH
#define HIT_TIMING_GRAPH

#include <vector>

#include "../../ui/Window.h"
#include "../../ui/GuiObj.h"
#include "../Filestructure/Play.h"

class HitTimingGraph : public GuiObj
{
	public:
		HitTimingGraph(int _xpos, int _ypos, Play* _play);

	private:
		virtual void Draw(Window& _win);
		virtual void UpdateInternal(Window& _win);

		std::vector<int> bins;
};

#endif // !HIT_TIMING_GRAPH