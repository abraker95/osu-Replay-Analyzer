#ifndef GRAPH_H
#define GRAPH_H

#include <queue>

#include "Window.h"
#include "GuiObj.h"
#include "ColorTable.h"

class Graph: public GuiObj
{
	public:
		Graph(GuiObj* _parent = nullptr);
		Graph(int _xpos, int _ypos, int _width, int _height, GuiObj* _parent = nullptr);
		virtual ~Graph();

		void SetContParam(double* _xVal, double *_yVal, double _maxVals);
		void SetStaticParam(std::vector<double> *_xVal, std::vector<double> *_yVal, double _maxVals);
		
		void SetMouseOverVal(bool _mouseOver);

		void Clear();

		void setWindow(double _xBeg, double _yBeg, double _xEnd, double _yEnd);
		void Resize(int _width, int _height);

	private:
		enum AXIS
		{
			X_AXIS,
			Y_AXIS
		};

		virtual void Draw(Window &_win);
		void DrawGraph(Window &_win);

		virtual void UpdateInternal(Window &_win);
		void UpdateMouseOver(Window &_win);
		void UpdateZoom(Window &_win);

		double Val2Pos(double _val, AXIS _axis);

		stringw mouseOver;
		double *xVal, *yVal;
		double xBeg, xEnd, yBeg, yEnd;
		double maxVals;
		bool cont;

		bool mouseOverEnable;

		std::pair<std::queue<double>, std::queue<double>> vals;
		ColorTable table;
};

#endif