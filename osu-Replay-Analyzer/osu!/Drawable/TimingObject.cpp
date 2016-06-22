#include "TimingObject.h"
#include "TimingGraph.h"

TimingObject::TimingObject(int _xpos, int _ypos, int _width, int _height, Hitobject* _hitobject, Beatmap::Modifier* _modifier, Beatmap::Diff* _diff, TimingGraph* _timingGraph) : GuiObj(_xpos, _ypos, _width, _height, _timingGraph)
{
	state = IDLE;

	timingGraph = _timingGraph;
	hitobject = _hitobject;
	modifier = _modifier;
	diff = _diff;

	origXpos = _xpos;
	origWidth = _width;

}

void TimingObject::UpdateInternal(Window& _win)
{
	switch (state)
	{
		case IDLE:
			IdleLogic(_win);
			UpdatePosDim();
			break;

		case HIGHLIGHTED:
			HighlightedLogic(_win);
			UpdatePosDim();
			break;

		case SELECTED:
			SelectedLogic(_win);
			UpdateHitObjectTiming();
			break;

		default:
			// ???
			break;
	}
}

void TimingObject::Draw(Window& _win)
{
	_win.driver->draw2DRectangle(color, rect<s32>(absXpos, absYpos, absXpos + width, absYpos + height));
}

void TimingObject::IdleLogic(Window &_win)
{
	if (this->isMouseOnObj(_win, true))
		state = HIGHLIGHTED;
	else
		color = IDLE_COLOR;
}

void TimingObject::HighlightedLogic(Window &_win)
{
	if (!this->isMouseOnObj(_win, true))
		state = IDLE;
	else
	{
		bool leftEdge = _win.reciever.GetMouseState().LeftButtonEdge;
		if (leftEdge)
		{
			state = SELECTED;
		}

		color = HIGHLIGHTED_COLOR;
	}
}

void TimingObject::SelectedLogic(Window &_win)
{
	position2di pos = _win.reciever.GetMouseState().positionCurr;
	position2di mouseDelta = _win.reciever.getCursorDelta();
	bool leftButton = _win.reciever.GetMouseState().LeftButtonDown;
	bool leftEdge = _win.reciever.GetMouseState().LeftButtonEdge;

	if (!this->isMouseOnObj(_win, true) && leftEdge)
		state = IDLE;
	else if (leftButton)
	{
		// has to be += due the fact that x - xoffset = 0, 
		// resolving the assignment to x = pos.x
		// this->x = pos.X - this->xOffset + this->x  => this->x = pos.X - 0;
		this->xpos += mouseDelta.X;
		color = SELECTED_COLOR;
	}
}

void TimingObject::UpdateHitObjectTiming()
{
	
}

void TimingObject::UpdatePosDim()
{
	std::pair<int, int> viewTimes = timingGraph->getViewTimes();
	std::pair<int, int> visibilityTimes = hitobject->getVisiblityTimes(diff->ar, modifier->hidden, 0.1, 0.1);

	int startPos = (visibilityTimes.first - viewTimes.first)*timingGraph->getZoom();
	int endPos = (visibilityTimes.second - viewTimes.first)*timingGraph->getZoom();

	xpos = startPos;
	width = endPos - startPos;
}