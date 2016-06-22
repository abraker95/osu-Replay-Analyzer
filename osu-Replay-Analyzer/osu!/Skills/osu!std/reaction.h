#ifndef SKILLSCALC_H
#define SKILLSCALC_H

#include <vector>
#include "../../Filestructure/Beatmap.h"

double EyeTime(double _theta);
double getReactionSkill(Beatmap* _beatmap, int _time);

#endif