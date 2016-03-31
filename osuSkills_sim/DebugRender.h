#ifndef DEBUGRENDER_H
#define DEBUGRENDER_H

#include <vector>
#include "Window.h"
#include "Hitcircle.h"

void DrawLine(Window &_win, Hitcircle &_hc1, Hitcircle &_hc2);
void DrawAngle(Window &_win, Hitcircle &_hc1, Hitcircle &_hc2, Hitcircle &_hc3);

#endif