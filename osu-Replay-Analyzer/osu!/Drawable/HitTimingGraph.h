#ifndef HIT_TIMING_GRAPH
#define HIT_TIMING_GRAPH

#include <vector>

#include "../../ui/Window.h"
#include "../../ui/GuiObj.h"
#include "../Filestructure/Play.h"
#include "../../ui/ColorTable.h"

class HitTimingGraph : public GuiObj
{
	public:
		HitTimingGraph(int _xpos, int _ypos, Play* _play);
		virtual ~HitTimingGraph();

		void setBinTable(std::vector<double> _binTable);
		void Init();

	private:
		virtual void Draw(Window& _win);
		virtual void UpdateInternal(Window& _win);

		void genStdBins();
		void genCtbBins();
		void genTaikoBins();
		void genManiaBins();
		void genDodgeBins();

		void Clear();

		std::vector<int> bins;
		std::vector<double> binTable;
		std::vector<core::stringw> textBins;

		ColorTable colorTable;
		Play* play;
};

#endif // !HIT_TIMING_GRAPH