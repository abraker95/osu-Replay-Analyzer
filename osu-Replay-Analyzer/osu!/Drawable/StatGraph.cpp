#include "StatGraph.h"
#include "../../utils/mathUtils.h"
#include "../Analysis/AnalysisStruct.h"

StatGraph::StatGraph(int _xpos, int _ypos, int *_viewtime) : GuiObj(_xpos, _ypos, 0, 40, nullptr)
{
	viewtime = _viewtime;
	timings = nullptr;
	selection = 0;

	graph = Graph(this);
		graph.Resize(0, this->height);
		graph.addClipDimTo(GuiObj::RIGHT);
		graph.setMargin(20, 0);

	btnUp = Button(-1, 0, 15, 10, this);
		btnUp.ClipPosTo(GuiObj::TOPRIGHT);

	btnDwn = Button(-1, 1, 15, 10, this);
		btnDwn.ClipPosTo(GuiObj::BTMRIGHT);	
}

StatGraph::~StatGraph(){}

void StatGraph::Init()
{
	timings = getSelection();

	// Make sure we have timings
	if (timings == nullptr)	return;
	if (timings->size() == 0) return;

	std::vector<double> xvals;
	std::vector<double> yvals;

	double max = INT_MIN, min = INT_MAX;
	for (osu::TIMING timing : *timings)
	{
		xvals.push_back(timing.time);
		yvals.push_back(timing.data);

		max = MAX(timing.data, max);
		min = MIN(timing.data, min);
	}

	graph.Clear();
	graph.SetStaticParam(&xvals, &yvals, 1000);
	graph.setWindow(0, min, xvals[xvals.size() - 1], max);
}


void StatGraph::Draw(Window &_win) 
{
	// Make sure we have timings
	if (timings == nullptr)	return;
	if (timings->size() == 0) return;

	double maxTime = (*timings)[timings->size() - 1].time;
	double pos = getValue(0, graph.getDim().Width, (*viewtime) / maxTime);

	std::string dataName = AnalysisStruct::beatmapAnalysis.getValidAnalyzers()[selection];

	_win.driver->draw2DRectangleOutline(recti(absXpos + pos, absYpos, absXpos + pos + 1, absYpos + height), SColor(255, 255, 255, 255));
	_win.font->draw(core::stringw(dataName.data()), rect<s32>(absXpos + this->width/2, absYpos, 100, 10), SColor(255, 255, 255, 255));

	graph.Update(_win);
	btnUp.Update(_win);
	btnDwn.Update(_win);
}

void StatGraph::UpdateInternal(Window &_win) 
{
	UpdateDataSelect();

	if (timings == nullptr) return;
	if (timings->size() == 0) return;
}

void StatGraph::UpdateDataSelect()
{
	if (btnUp.isTriggered())	CycleSelectUp();
	if (btnDwn.isTriggered())	CycleSelectDwn();
}

void StatGraph::CycleSelectUp()
{
	int numSelections = AnalysisStruct::beatmapAnalysis.getValidAnalyzers().size();

	if (selection + 1 >= numSelections)
		selection = 0;
	else
		selection++;

	Init();
}

void StatGraph::CycleSelectDwn()
{
	int numSelections = AnalysisStruct::beatmapAnalysis.getValidAnalyzers().size();

	if (selection - 1 <= -1)
		selection = numSelections - 1;
	else
		selection--;

	Init();
}

std::vector<osu::TIMING>* StatGraph::getSelection()
{
	std::vector<std::string> analyzers = AnalysisStruct::beatmapAnalysis.getValidAnalyzers();
	if (analyzers.size() == 0) return nullptr;

	return AnalysisStruct::beatmapAnalysis.getAnalyzer(analyzers[selection])->getData();
}