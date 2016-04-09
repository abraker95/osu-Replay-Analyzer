#ifndef OSUSTATS_H
#define OSUSTATS_H

#include <vector>
#include "Window.h"
#include "Hitcircle.h"

// _res = resolution in px/ms
void drawTimingGraphs(Window &_win, int xoffset, int yoffset, std::vector<Hitcircle> &_hitcircles, bool _relative, int _time, double _res, double _CS, double _AR, bool _hidden);

#endif