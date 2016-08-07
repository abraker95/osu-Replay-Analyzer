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
		void Init();

	private:
		virtual void Draw(Window& _win);
		virtual void UpdateInternal(Window& _win);

		void genStdBins();
		void genCtbBins();
		void genTaikoBins();
		void genManiaBins();
		void genDodgeBins();

		std::vector<int> bins;
		std::vector<core::stringw> textBins;
		Play* play;
};

#endif // !HIT_TIMING_GRAPH