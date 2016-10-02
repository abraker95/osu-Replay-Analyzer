#ifndef STATGRAPH_H
#define STATGRAPH_H

#include "../../ui/Graph.h"
#include "../osuStructs.h"
#include "../../ui/Button.h"

class StatGraph : public GuiObj
{
	public:
		StatGraph(int _xpos, int _ypos, int *_viewtime);
		virtual ~StatGraph();

		void Init();

	private:
		enum SELECTIONS
		{
			VEL,
			NPS,
			OVRLP,
			DIFF,
			NUM_SELECTIONS
		};

		Graph graph;
		Button btnUp, btnDwn;

		std::vector<osu::TIMING>* timings;
		int *viewtime;
		int selection;

		virtual void Draw(Window &_win);
		virtual void UpdateInternal(Window &_win);
		
		void UpdateDataSelect();
		std::vector<osu::TIMING>* getSelection();

		void CycleSelectUp();
		void CycleSelectDwn();
};

#endif // !STATGRAPH_H