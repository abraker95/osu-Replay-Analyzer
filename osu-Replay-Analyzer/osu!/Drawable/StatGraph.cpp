#include "StatGraph.h"
#include "../Skills/osu-std/reading.h"
#include "../../utils/mathUtils.h"

StatGraph::StatGraph(int _xpos, int _ypos, int *_viewtime) : GuiObj(_xpos, _ypos, 0, 40, nullptr)
{
	viewtime = _viewtime;
	timings = nullptr;
	selection = 0;

	velGraph = Graph(this);
		velGraph.Resize(0, 0);

	btnUp = Button(-1, 0, 15, 10, this);
		btnUp.ClipPosTo(GuiObj::TOPRIGHT);

	btnDwn = Button(-1, 1, 15, 10, this);
		btnDwn.ClipPosTo(GuiObj::BTMRIGHT);
}

StatGraph::~StatGraph(){}

void StatGraph::InitGraph()
{
	timings = getSelection();

	// Make sure we have timings
	if (timings == nullptr)	return;
	if (timings->size() == 0) return;

	std::vector<double> velX;
	std::vector<double> velY;

	double max = INT_MIN;
	for (osu::TIMING timing : *timings)
	{
		velX.push_back(timing.time);
		velY.push_back(timing.data);

		max = MAX(timing.data, max);
	}

	velGraph.Clear();
	velGraph.SetStaticParam(&velX, &velY, 1000);
	velGraph.setWindow(0, 0, velX[velX.size() - 1], max);
	velGraph.Resize(0, this->height);
	velGraph.addClipDimTo(GuiObj::RIGHT);
	velGraph.setMargin(20, 0);
}


void StatGraph::Draw(Window &_win) 
{
	// Make sure we have timings
	if (timings == nullptr)	return;
	if (timings->size() == 0) return;

	double maxTime = (*timings)[timings->size() - 1].time;
	double pos = getValue(0, velGraph.getDim().Width, (*viewtime) / maxTime);

	_win.driver->draw2DRectangleOutline(recti(absXpos + pos, absYpos, absXpos + pos + 1, absYpos + height), SColor(255, 255, 255, 255));
	_win.font->draw(mouseOver.second, rect<s32>(absXpos + mouseOver.first, absYpos, 100, 10), SColor(255, 255, 255, 255));

	velGraph.Update(_win);
	btnUp.Update(_win);
	btnDwn.Update(_win);
}

void StatGraph::UpdateInternal(Window &_win) 
{
	UpdateDataSelect();

	if (timings == nullptr) return;
	if (timings->size() == 0) return;

	/// \TODO: This belongs in the Graph object
	if (velGraph.isMouseOnObj(_win, false))
	{
		position2di pos = _win.reciever.GetMouseState().positionCurr;

		double maxTime = (*timings)[timings->size() - 1].time;
		double time = getValue(0, maxTime, (double)pos.X/(double)velGraph.getDim().Width);
		int index = MAX(FindTimingAt(*timings, time) - 1, 0);

		mouseOver.first = pos.X;
		mouseOver.second = core::stringw((*timings)[index].data);
	}
	else
	{
		mouseOver.second = "";
	}
}

void StatGraph::UpdateDataSelect()
{
	if (btnUp.isTriggered())	CycleSelectUp();
	if (btnDwn.isTriggered())	CycleSelectDwn();
}

void StatGraph::CycleSelectUp()
{
	if (selection + 1 >= NUM_SELECTIONS)
		selection = 0;
	else
		selection++;

	InitGraph();
}

void StatGraph::CycleSelectDwn()
{
	if (selection - 1 <= -1)
		selection = NUM_SELECTIONS - 1;
	else
		selection--;

	InitGraph();
}

std::vector<osu::TIMING>* StatGraph::getSelection()
{
	switch (selection)
	{
		case VEL:
			return &OSUSTANDARD::READING::velocities;

		case NPS:
			return &OSUSTANDARD::READING::notesRate;

		case DIFF:
			return &OSUSTANDARD::READING::diffs;

		case OVRLP:
			return &OSUSTANDARD::READING::overlaps;

		default:
			return nullptr;
	}
}