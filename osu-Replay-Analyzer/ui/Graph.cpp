#include "Graph.h"
#include "../utils/mathUtils.h"

Graph::Graph(GuiObj* _parent) : GuiObj(0, 0, 0, 0, _parent)
{
	xVal = nullptr;
	yVal = nullptr;

	table.AddValueMap(0, SColor(255, 100, 255, 175));
	table.AddValueMap(100, SColor(255, 255, 175, 100));
	table.AddValueMap(1000, SColor(255, 255, 100, 100));

	setWindow(-10, -10, 10, 10);
}

Graph::Graph(int _xpos, int _ypos, int _width, int _height, GuiObj* _parent) : GuiObj(_xpos, _ypos, _width, _height, _parent)
{
	xVal = nullptr;
	yVal = nullptr;

	table.AddValueMap(0, SColor(255, 100, 255, 175));
	table.AddValueMap(100, SColor(255, 255, 175, 100));
	table.AddValueMap(1000, SColor(255, 255, 100, 100));

	setWindow(-10, -10, 10, 10);
}

Graph::~Graph()
{

}

void Graph::SetContParam(double *_xVal, double *_yVal, double _maxVals)
{
	cont = true;

	xVal = _xVal;
	yVal = _yVal;
	maxVals = _maxVals;

	while (vals.first.size() > maxVals)		vals.first.pop();
	while (vals.second.size() > maxVals)	vals.second.pop();
}

void Graph::SetStaticParam(std::vector<double> *_xVal, std::vector<double> *_yVal, double _maxVals)
{
	cont = false;
	maxVals = _maxVals;

	for (double val : *_xVal)	vals.first.push(val);
	for (double val : *_yVal)	vals.second.push(val);
}

void Graph::SetMouseOverVal(bool _mouseOver)
{
	mouseOver = _mouseOver;
}

void Graph::Clear()
{
	while (vals.first.size() > 0)	vals.first.pop();
	while (vals.second.size() > 0)	vals.second.pop();
}

void Graph::setWindow(double _xBeg, double _yBeg, double _xEnd, double _yEnd)
{
	xBeg = _xBeg;	yBeg = _yBeg;
	xEnd = _xEnd;	yEnd = _yEnd;
}

void Graph::Resize(int _width, int _height)
{
	width = _width;
	height = _height;
}

double Graph::Val2Pos(double _val, AXIS _axis)
{
	double pos = 0;

	switch (_axis)
	{
		case X_AXIS:
			pos = absXpos + getPercent(xBeg, _val, xEnd)*width;
			return pos;
			break;

		case Y_AXIS:
			pos = absYpos + (height - getPercent(yBeg, _val, yEnd)*height);
			return pos;
			break;

		default:
			return -1;
			break;
	};
}

void Graph::Draw(Window &_win)
{
	if (vals.second.size() == 0)	return;
	if (vals.first.size() == 0)		return;

	double sampleRate = MAX(1, vals.second.size() / width);
	int i = sampleRate;
	for (; i < vals.second.size(); i += sampleRate)
	{
		double currYVal = vals.second._Get_container()[i];
		double prevYVal = vals.second._Get_container()[MAX(0, i - sampleRate)];

		double currXVal = vals.first._Get_container()[i];
		double prevXVal = vals.first._Get_container()[MAX(0, i - sampleRate)];

		SColor color = table.getColor(currYVal);

		vector2di p1(Val2Pos(prevXVal, X_AXIS), Val2Pos(prevYVal, Y_AXIS));
		vector2di p2(Val2Pos(currXVal, X_AXIS), Val2Pos(currYVal, Y_AXIS));

		_win.driver->draw2DLine(p1, p2, color);
	}

	_win.font->draw(core::stringw(yEnd), rect<s32>(absXpos, Val2Pos(yEnd, Y_AXIS), 100, 10), table.getColor(yEnd));
	//_win.font->draw(core::stringw(yEnd/2), rect<s32>(absXpos, Val2Pos(yEnd/2, Y_AXIS),absXpos + 100, 10), table.getColor(yEnd/2));
	_win.font->draw(core::stringw(0), rect<s32>(absXpos, Val2Pos(0, Y_AXIS) - 10, 100, 10), table.getColor(0));

	_win.driver->draw2DRectangleOutline(recti(absXpos, absYpos, absXpos + width, absYpos + height), SColor(255, 255, 255, 255));
}

void Graph::UpdateInternal(Window &_win)
{
	if (cont)
	{
		if (xVal == nullptr)	return;
		if (yVal == nullptr)	return;

		vals.first.push(MIN(vals.first.size(), maxVals));
		vals.second.push(*yVal);

		while (vals.first.size() > maxVals)		vals.first.pop();
		while (vals.second.size() > maxVals)	vals.second.pop();
	}
}

void Graph::UpdateMouseOver(Window &_win)
{
	/// \TODO

	/*if (timings == nullptr)
		return;

	if (isMouseOnObj(_win, false))
	{
		position2di pos = _win.reciever.GetMouseState().positionCurr;

		double maxTime = (*timings)[timings->size() - 1].time;
		double time = getValue(0, maxTime, (double)pos.X / (double)velGraph.getDim().Width);
		int index = MAX(FindTimingAt(*timings, time) - 1, 0);

		mouseOver.first = pos.X;
		mouseOver.second = core::stringw((*timings)[index].data);
	}
	else
	{
		mouseOver.second = "";
	}*/
}