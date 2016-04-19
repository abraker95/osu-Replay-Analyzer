#ifndef SKILLSCALC_H
#define SKILLSCALC_H

#include <vector>
#include "Hitcircle.h"

double EyeTime(double _theta);
double getReactionSkill(std::vector<Hitcircle> &_hitcircles, int _time, double _CS, double _AR, bool _hidden);

#endif