#ifndef DEBUGRENDER_H
#define DEBUGRENDER_H
#define _USE_MATH_DEFINES

#include <vector>
#include <math.h>

#include "Window.h"
#include "Hitcircle.h"

void DrawLine(Window &_win, Hitcircle &_hc1, Hitcircle &_hc2);
void DrawArc(Window &_win, int _xpos, int _ypos, double _radius, SColor _color = SColor(255, 255, 255, 255), double _startAngle = 0.0, double _radAngle = 2 * M_PI);
void DrawAngle(Window &_win, Hitcircle &_hc1, Hitcircle &_hc2, Hitcircle &_hc3);


#endif