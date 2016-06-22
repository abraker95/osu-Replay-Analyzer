#ifndef DEBUGRENDER_H
#define DEBUGRENDER_H
#define _USE_MATH_DEFINES

#include <vector>
#include <math.h>

#include "Window.h"
#include "Hitobject.h"

void DrawLine(Window &_win, Hitobject& _hc1, Hitobject& _hc2);
void DrawArc(Window &_win, int _xpos, int _ypos, double _radius, SColor _color = SColor(255, 255, 255, 255), double _startAngle = 0.0, double _radAngle = 2 * M_PI);
void DrawAngle(Window &_win, Hitobject& _hc1, Hitobject& _hc2, Hitobject& _hc3);


#endif