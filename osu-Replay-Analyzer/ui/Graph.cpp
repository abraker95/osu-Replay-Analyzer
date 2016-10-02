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

	mouseOverEnable = true;
}

Graph::Graph(int _xpos, int _ypos, int _width, int _height, GuiObj* _parent) : GuiObj(_xpos, _ypos, _width, _height, _parent)
{
	xVal = nullptr;
	yVal = nullptr;

	table.AddValueMap(0, SColor(255, 100, 255, 175));
	table.AddValueMap(100, SColor(255, 255, 175, 100));
	table.AddValueMap(1000, SColor(255, 255, 100, 100));

	setWindow(-10, -10, 10, 10);

	mouseOverEnable = true;
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
	mouseOverEnable = _mouseOver;
}

void Graph::Clear()
{
	while (vals.first.size() > 0)	vals.first.pop();
	while (vals.second.size() > 0)	vals.second.pop();
}


double Graph::getXMax()
{
	return vals.first._Get_container()[vals.first.size() - 1];
}

double Graph::getXMin()
{
	return vals.first._Get_container()[0];
}

double Graph::getYMax()
{
	/// \TODO: use cache or something. Going through this everytime is expensive
	return 0;
}

double Graph::getYmin()
{
	/// \TODO: use cache or something. Going through this everytime is expensive
	return 0;
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

	DrawGraph(_win);

	position2di pos = _win.reciever.GetMouseState().positionCurr;
	_win.font->draw(mouseOver, rect<s32>(absXpos + pos.X, absYpos, 100, 10), SColor(255, 255, 255, 255));

	/// \TODO: Handle errors concerning insufficient info to automatically determine graph size

  ///  _win.font->draw(core::stringw(yEnd), rect<s32>(absXpos, Val2Pos(yEnd, Y_AXIS), 100, 10), table.getColor(yEnd));
	//_win.font->draw(core::stringw(yEnd/2), rect<s32>(absXpos, Val2Pos(yEnd/2, Y_AXIS),absXpos + 100, 10), table.getColor(yEnd/2));
	_win.font->draw(core::stringw(0), rect<s32>(absXpos, Val2Pos(0, Y_AXIS) - 10, 100, 10), table.getColor(0));

	_win.driver->draw2DRectangleOutline(recti(absXpos, absYpos, absXpos + width, absYpos + height), SColor(255, 255, 255, 255));
}

void Graph::DrawGraph(Window &_win)
{
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

	UpdateMouseOver(_win);
	UpdatePos(_win);

	if (_win.reciever.IsKeyDown(KEY_KEY_Z))
	{
		UpdateZoom(_win);
	}
}

void Graph::UpdateMouseOver(Window &_win)
{
	if (isMouseOnObj(_win, false) && mouseOverEnable)
	{
		position2di pos = _win.reciever.GetMouseState().positionCurr;
		double percent = (double)(pos.X - absXpos) / (double)getDim().Width;
		int index = (vals.first.size() - 1) * percent;

		mouseOver = core::stringw(vals.second._Get_container()[index]);
	}
	else
	{
		mouseOver = "";
	}
}

void Graph::UpdateZoom(Window &_win)
{
	if (!isMouseOnObj(_win, false)) return;

	position2di pos = _win.reciever.GetMouseState().positionCurr;

	double step = (xEnd-xBeg) / 10.0; // amount of units to zoom in/out by
	double xDelta = step*_win.reciever.getWheel();

	if (xEnd + xDelta <= vals.first._Get_container()[vals.first.size() - 1])
		xEnd += xDelta;

	if (xBeg - xDelta >= vals.first._Get_container()[0])
		xBeg -= xDelta;
}

void Graph::UpdatePos(Window &_win)
{
	if (!isMouseOnObj(_win, false)) return;

	bool leftButton = _win.reciever.GetMouseState().LeftButtonDown;
	if (!leftButton) return;

	/// TODO: positionMove is not accurate, and therefore the positioning kinda shifts over time. 
	///		  Shouldn't be too much of an issue, but do find a more accurate way if there is one
	position2di posMove = _win.reciever.GetMouseState().positionMove;
	int xDelta = -(posMove.X * (xEnd - xBeg) / getDim().Width);

	if(xEnd + xDelta <= vals.first._Get_container()[vals.first.size() - 1])
		xEnd += xDelta;

	if (xBeg + xDelta >= vals.first._Get_container()[0])
		xBeg += xDelta;
}