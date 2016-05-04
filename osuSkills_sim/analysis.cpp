#include "analysis.h"
#include "mathUtils.h"
#include "Geometry.h"
#include "osuCalc.h"

#include <assert.h>
#include <iostream>

std::vector<std::tuple<int, int, int, int>> getPattern(std::vector<Hitcircle> &_hitcircles, int _time, int _index, double _CS, int _quant)
{
	Hitcircle &circle = _hitcircles[_index];
	std::vector<std::tuple<int, int, int, int>> points;

	// If it is a slider, then iterate through and find reaction points.
	if (circle.isSlider())
	{
		int i = 0, prev_i = i;
		int timeDelta = circle.getSliderTimeAt(1) - circle.getSliderTimeAt(0) + 1;
		int pointTime = 0;

		// find the last point before current time
		for (; pointTime < _time && pointTime + timeDelta < circle.getEndTime(); i++)
			pointTime = circle.getSliderTimeAt(i);

		// record the point
		points.push_back(getPointAt(circle, i--));
		prev_i = i;

		// get 2 points before the current point which are at least a circle size apart
		while (i >= 0 && points.size() < _quant)
		{
			int dist = 0;

			// find the next point that is at least a circle size apart
			while (dist < CS2px(_CS) && i >= 0 && points.size() < 3)
			{
				// \TODO: make this more accurate
				dist = getDist(circle.getSliderPointAt(prev_i), circle.getSliderPointAt(i--));
			}

			// record the point if the position is different
			position2di prevPos = position2di(std::get<Hitcircle::XPOS>(points[points.size() - 1]), std::get<Hitcircle::YPOS>(points[points.size() - 1]));
			position2di newPos = circle.getSliderPointAt(1 + i);
			if (getDist(prevPos, newPos) > CS2px(_CS))
			{
				prev_i = i;
				points.push_back(getPointAt(circle, 1 + i--));
			}
		}
	}
	else
	{
		position2di pos = circle.getPos();
		points.push_back(std::tuple<int, int, int, int>(pos.X, pos.Y, circle.getTime(), 0));
	}

	// check if we have the required amount of points
	if (points.size() < _quant && _index > 0)
	{
		// if not, go to the previous hitobject
		std::vector<std::tuple<int, int, int, int>> pattern = getPattern(_hitcircles, _hitcircles[_index - 1].getEndTime(), _index - 1, _CS, _quant - points.size());

		points.insert(points.end(),
			std::make_move_iterator(pattern.begin()),
			std::make_move_iterator(pattern.end()));

		assert(points.size() != 0);
		return points;
	}
	else
	{
		assert(points.size() != 0);
		return points;
	}
}

double PatternReq(std::tuple<int, int, int, int> _p1, std::tuple<int, int, int, int> _p2, std::tuple<int, int, int, int> _p3, double CSpx)
{
	double dist_12 = getDist(position2di(std::get<Hitcircle::XPOS>(_p1), std::get<Hitcircle::YPOS>(_p1)), 
							 position2di(std::get<Hitcircle::XPOS>(_p2), std::get<Hitcircle::YPOS>(_p2)));
	double dist_23 = getDist(position2di(std::get<Hitcircle::XPOS>(_p2), std::get<Hitcircle::YPOS>(_p2)),
							 position2di(std::get<Hitcircle::XPOS>(_p3), std::get<Hitcircle::YPOS>(_p3)));
	double dist = dist_12 + dist_23;

	double angle = getAngle(position2di(std::get<Hitcircle::XPOS>(_p1), std::get<Hitcircle::YPOS>(_p1)),
							position2di(std::get<Hitcircle::XPOS>(_p2), std::get<Hitcircle::YPOS>(_p2)),
							position2di(std::get<Hitcircle::XPOS>(_p3), std::get<Hitcircle::YPOS>(_p3)));

	double time = std::get<Hitcircle::TIME>(_p3) - std::get<Hitcircle::TIME>(_p1);

	return time / ((dist / (3 * CSpx))*((M_PI - angle)/M_PI));
}

std::tuple<int, int, int, int> getPointAt(Hitcircle &_hitcircle, int _index)
{
	position2di pos = _hitcircle.getPos();
	int time = _hitcircle.getTime();
	int timeSinceStart = 0;

	if (_hitcircle.isSlider())
	{
		pos = _hitcircle.getSliderPointAt(_index);
		time = _hitcircle.getSliderTimeAt(_index);
		timeSinceStart = time - _hitcircle.getTime();
	}

	return std::tuple<int, int, int, int>(pos.X, pos.Y, time, timeSinceStart);
}

// Gets the parametric velocity (in individual X, Y components)
vector2df getParamVel(std::tuple<int, int, int, int> _p1, std::tuple<int, int, int, int> _p2)
{
	position2di p1 = position2di(std::get<Hitcircle::XPOS>(_p1), std::get<Hitcircle::YPOS>(_p1));
	position2di p2 = position2di(std::get<Hitcircle::XPOS>(_p2), std::get<Hitcircle::YPOS>(_p2));

	float t1 = std::get<Hitcircle::TIME>(_p1);
	float t2 = std::get<Hitcircle::TIME>(_p2) +std::get<3>(_p2);

	return vector2df((p2.X - p1.X) / (t2 - t1), (p2.Y - p1.Y) / (t2 - t1));
}

// \TODO: Take account sliders. It currently only does start time of the current note
int getNumIntersections(std::vector<Hitcircle> &_hitcircles, int _index, double _ARms)
{
	int timeEnd = _hitcircles[_index].getTime();
	int timeStart = timeEnd - _ARms;
	int indexStart = getHitcircleAt(_hitcircles, timeStart);
	
	// \TODO: to be applied when doing slider intersection check
	//getPattern(_hitcircles, timeEnd, _index, _CS, getNumVisibleAt(_hitcircles, _AR, _hidden, 0.1));

	// out of bounds check
	if (indexStart == -1)
		return 0;

	int numIntersections = 0;
	std::pair<position2di, position2di> path;
		path.first = _hitcircles[_index - 1].getPos();
		path.second = _hitcircles[_index].getPos();

	for (int i = indexStart; i < _index; i++)
	{
		// out of bounds check
		if (i > 2)
		{
			std::pair<position2di, position2di> checkPath;
			checkPath.first = _hitcircles[i - 1].getPos();
			checkPath.second = _hitcircles[i].getPos();

			if (HasIntersectionPoint(path, checkPath))
				numIntersections++;
		}
	}

	return numIntersections;
}