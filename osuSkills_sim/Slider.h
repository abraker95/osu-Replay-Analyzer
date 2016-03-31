#ifndef SLIDER_H
#define SLIDER_H

#include "Window.h"

#include <iostream>
using namespace std;

class Slider
{
	public:
		enum AUTO_DIR
		{
			FORWARD = 0,
			BACKWARD = 1
		};

		enum AUTO_MODE
		{
			LOOP = 0,
			REVERSE = 1
		};

		Slider(int _x, int _y, double _width, double _val = 0.0);

		void Draw(Window &_win);

		void setRange(double _min, double _max);

		void setWidth(double _width);

		void setAuto(bool _auto, AUTO_MODE _autoMode);

		double getVal();

	private:
		double max, min, val;
		int x, y;
		double width;

		bool automate;
		
		AUTO_DIR autoDir;
		AUTO_MODE autoMode;

		const SColor SLIDER_COLOR = SColor(255, 255, 255, 255);
		const int SLIDER_POINT_RADIUS = 5;

		enum State
		{
			IDLE = 0,
			HOLD = 1
		}state;

		double Pos2percent(int _pos);

		double Pos2val(int _pos);

		int Percent2pos(double _percent);

		int Val2pos(double _val);

		void UpdateAuto();

		void IdleLogic(Window &_win);

		void HoldLogic(Window &_win);

		void Update(Window &_win);
};

#endif