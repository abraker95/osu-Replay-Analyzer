#include <tuple>
#include <vector>

#include "Hitcircle.h"

std::vector<std::tuple<int, int, int, int>> getPattern(std::vector<Hitcircle> &_hitcircles, int _time, int _index, double _CS, int _quant = 3);

double PatternReq(std::tuple<int, int, int, int> _p1, std::tuple<int, int, int, int> _p2, std::tuple<int, int, int, int> _p3, double CSpx);

vector2df getParamVel(std::tuple<int, int, int, int> _p1, std::tuple<int, int, int, int> _p2);

std::tuple<int, int, int, int> getPointAt(Hitcircle &_hitcircle, int _index);

int getNumIntersections(std::vector<Hitcircle> &_hitcircles, int _index, double _ARms);