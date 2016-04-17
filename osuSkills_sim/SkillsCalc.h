#ifndef SKILLSCALC_H
#define SKILLSCALC_H

#include <vector>
#include "Hitcircle.h"

double EyeTime(double _theta);
double CalcChaos(std::vector<Hitcircle> &_hitcircles, int _ref, double _CS, double _AR, bool _hidden);

#endif