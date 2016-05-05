#include <tuple>
#include <vector>

#include "Hitcircle.h"

// Gets the index of a hitcircle at a certain time
int getHitcircleAt(std::vector<Hitcircle>& _hitcircles, int _time);

std::vector<std::tuple<int, int, int, int>> getPattern(std::vector<Hitcircle> &_hitcircles, int _time, int _index, double _CS, int _quant = 3);

double PatternReq(std::tuple<int, int, int, int> _p1, std::tuple<int, int, int, int> _p2, std::tuple<int, int, int, int> _p3, double CSpx);

vector2df getParamVel(std::tuple<int, int, int, int> _p1, std::tuple<int, int, int, int> _p2);

std::tuple<int, int, int, int> getPointAt(Hitcircle &_hitcircle, int _index);

int getNumIntersections(std::vector<Hitcircle> &_hitcircles, int _index, double _ARms);
// Gets the number of visible hitcircles at a certain time
int getNumVisibleAt(std::vector<Hitcircle>& _hitcircles, int _time, double _AR, bool _hidden, double _opacity);