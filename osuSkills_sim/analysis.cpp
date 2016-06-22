#include "analysis.h"
#include "mathUtils.h"
#include "Geometry.h"
#include "osuCalc.h"

#include <assert.h>
#include <iostream>
#include <algorithm>

std::vector<std::tuple<int, int, int, int>> getPattern(Beatmap* _beatmap, int _time, int _index, int _quant)
{
	std::vector<std::tuple<int, int, int, int>> points;
	int radius = CS2px(_beatmap->getDiff().cs) / 2.0;
	
	Hitobject* hitObject = _beatmap->hitObjects[_index];
	int currTime = _time;

	// If it is a slider, then iterate through and find reaction points.
	if (hitObject->IsHitObjectType(SLIDER))
	{
		// record the point
		vector2di currSliderPos = hitObject->slider->GetSliderPos(currTime);
		points.push_back(std::tuple<int, int, int, int>(currSliderPos.X, currSliderPos.Y, _time, 0));

		while (points.size() < _quant)
		{
			currTime = hitObject->slider->getTimeDistFrom(_time, radius, 0);
			if (currTime == -1) // reached the beggining of the slider
			{
				Hitobject* prevObj = _beatmap->hitObjects[_index - 1];
				int prevObjStartTime;

				if (prevObj->IsHitObjectType(SLIDER))
					prevObjStartTime = prevObj->slider->GetLastTickTime();
				else
					prevObjStartTime = prevObj->getTime();
				
				std::vector<std::tuple<int, int, int, int>> pattern = getPattern(_beatmap, prevObjStartTime, _index - 1, _quant - points.size());

				points.insert(points.end(),
					std::make_move_iterator(pattern.begin()),
					std::make_move_iterator(pattern.end()));

				assert(points.size() != 0);
				return points;
			}
			else
			{
				currSliderPos = hitObject->slider->GetSliderPos(currTime);
				points.push_back(std::tuple<int, int, int, int>(currSliderPos.X, currSliderPos.Y, _time, 0));
			}
		}
	}
	else
	{
		points.push_back(std::tuple<int, int, int, int>(hitObject->getPos().X, hitObject->getPos().Y, _time, 0));

		Hitobject* prevObj = _beatmap->hitObjects[_index - 1];
		int prevObjStartTime;

		if (prevObj->IsHitObjectType(SLIDER))
			prevObjStartTime = prevObj->slider->GetLastTickTime();
		else
			prevObjStartTime = prevObj->getTime();

		std::vector<std::tuple<int, int, int, int>> pattern = getPattern(_beatmap, prevObjStartTime, _index - 1, _quant - points.size());

		points.insert(points.end(),
			std::make_move_iterator(pattern.begin()),
			std::make_move_iterator(pattern.end()));

		assert(points.size() != 0);
		return points;
	}
}

double JumpPatternReq(std::tuple<int, int, int, int> _p1, std::tuple<int, int, int, int> _p2, std::tuple<int, int, int, int> _p3, double CSpx)
{
	/*double dist_12 = getDist(position2di(std::get<Hitcircle::XPOS>(_p1), std::get<Hitcircle::YPOS>(_p1)), 
							 position2di(std::get<Hitcircle::XPOS>(_p2), std::get<Hitcircle::YPOS>(_p2)));
	double dist_23 = getDist(position2di(std::get<Hitcircle::XPOS>(_p2), std::get<Hitcircle::YPOS>(_p2)),
							 position2di(std::get<Hitcircle::XPOS>(_p3), std::get<Hitcircle::YPOS>(_p3)));
	double dist = dist_12 + dist_23;

	double angle = getAngle(position2di(std::get<Hitcircle::XPOS>(_p1), std::get<Hitcircle::YPOS>(_p1)),
							position2di(std::get<Hitcircle::XPOS>(_p2), std::get<Hitcircle::YPOS>(_p2)),
							position2di(std::get<Hitcircle::XPOS>(_p3), std::get<Hitcircle::YPOS>(_p3)));

	double time = std::get<Hitcircle::TIME>(_p3) - std::get<Hitcircle::TIME>(_p1);
	double distFactor = std::max(1.0, dist / CSpx);
	double angleFactor = getValue(1.0, M_PI, (M_PI - angle) / M_PI);*/

	return 0;//time / (distFactor * angleFactor);
}

double CtrlPatternReq(std::tuple<int, int, int, int> _p1, std::tuple<int, int, int, int> _p2, std::tuple<int, int, int, int> _p3, double CSpx)
{
	/*double dist_12 = getDist(position2di(std::get<Hitcircle::XPOS>(_p1), std::get<Hitcircle::YPOS>(_p1)), 
							 position2di(std::get<Hitcircle::XPOS>(_p2), std::get<Hitcircle::YPOS>(_p2)));
	double dist_23 = getDist(position2di(std::get<Hitcircle::XPOS>(_p2), std::get<Hitcircle::YPOS>(_p2)),
							 position2di(std::get<Hitcircle::XPOS>(_p3), std::get<Hitcircle::YPOS>(_p3)));
	double dist = dist_12 + dist_23;

	double angle = getAngle(position2di(std::get<Hitcircle::XPOS>(_p1), std::get<Hitcircle::YPOS>(_p1)),
							position2di(std::get<Hitcircle::XPOS>(_p2), std::get<Hitcircle::YPOS>(_p2)),
							position2di(std::get<Hitcircle::XPOS>(_p3), std::get<Hitcircle::YPOS>(_p3)));

	double time = std::get<Hitcircle::TIME>(_p3) - std::get<Hitcircle::TIME>(_p1);
	double distFactor = std::max(1.0, dist / CSpx);
	double angleFactor = getValue(1.0, M_PI, angle / M_PI);*/

	return 0;// (time * angleFactor) / (distFactor);
}

/*std::tuple<int, int, int, int> getPointAt(Hitcircle &_hitcircle, int _index)
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
}*/

// Gets the parametric velocity (in individual X, Y components)
vector2df getParamVel(std::tuple<int, int, int, int> _p1, std::tuple<int, int, int, int> _p2)
{
	/*position2di p1 = position2di(std::get<Hitcircle::XPOS>(_p1), std::get<Hitcircle::YPOS>(_p1));
	position2di p2 = position2di(std::get<Hitcircle::XPOS>(_p2), std::get<Hitcircle::YPOS>(_p2));

	float t1 = std::get<Hitcircle::TIME>(_p1);
	float t2 = std::get<Hitcircle::TIME>(_p2) +std::get<3>(_p2);*/

	return vector2df();// vector2df((p2.X - p1.X) / (t2 - t1), (p2.Y - p1.Y) / (t2 - t1));
}

// \TODO: Take account sliders. It currently only does start time of the current note
int getNumIntersectionsAt(Beatmap* _beatmap, int _time, double _AR, bool _hidden, double _opacity)
{
	std::pair<int, int> range = _beatmap->getIndicesVisibleAt(_time, _opacity);

	// \TODO: to be applied when doing slider intersection check
	//getPattern(_hitcircles, timeEnd, _index, _CS, getNumVisibleAt(_hitcircles, _AR, _hidden, 0.1));

	int numIntersections = 0;

	for (int i = range.first + 1; i < range.second; i++)
	{
		std::pair<position2d<double>, position2d<double>> path;
			path.first = _beatmap->hitObjects[i - 1]->getPos();
			path.second = _beatmap->hitObjects[i]->getPos();

		for (int j = i + 1; j < range.second; j++)
		{
			std::pair<position2d<double>, position2d<double>> checkPath;
				checkPath.first = _beatmap->hitObjects[i - 1]->getPos();
				checkPath.second = _beatmap->hitObjects[i]->getPos();

			if (HasIntersectionPoint(path, checkPath))
				numIntersections++;
		}
	}

	return numIntersections;
}

vector2df getParamVelVec(Beatmap* _beatmap, int _time, double _CS)
{
	int index = _beatmap->FindHitobjectAt(_time) + 1;

	if (index > _beatmap->hitObjects.size() - 1)
	{
		return vector2df(0.0, 0.0);
	}
	else if (index < 2)
	{
		return vector2df(0.0, 0.0);
	}
	else
	{
		std::vector<std::tuple<int, int, int, int>> pattern = getPattern(_beatmap, _time, index, 3);
		vector2df velXY;

		if (_beatmap->hitObjects[index]->IsHitObjectType(SLIDER))
			velXY = getParamVel(pattern[2], pattern[1]);
		else
			velXY = getParamVel(pattern[0], pattern[1]);

		//std::cout << velXY.X << ", " << velXY.Y << std::endl;

		return velXY;
	}
}

// future code to simulate cursor movement
vector2df getAimobjPosAt(std::vector<Hitcircle> &_hitcircles, int _time, double _CS, double _maxAcel, double _maxDecel)
{
	// \TODO: factor in the time in which the player will first see the object

	// aim object's position and velocity
/*	position2di AimObjPos = position2di(_hitcircles[0].getPos().X, _hitcircles[0].getPos().Y);
	vector2df AimObjVel = vector2df(0, 0);

	for (int t = 0; t < _time; t++)
	{
		// hitobject specific
		int index = getHitcircleAt(_hitcircles, t) + 1;
		vector2df velXY = getParamVelVec(_hitcircles, _time, _CS);

		// aimobject specific		
		double aimobjVel = AimObjVel.getDistanceFrom(vector2df(0, 0));
		double dist = getDist(AimObjPos, _hitcircles[index].getPos());
		bool AimObjOnHitobject = (CS2px(_CS) >= dist);

		// calculate the time it will take for the aimobject to go out of the hitobject's bounds at
		// current velocity.
		// double eta = 

		// If the aimobject is on the hitobject, then check if it's worth decelerating
		if (AimObjOnHitobject)
		{
			// If the aimobject will be outside of the hitobject's bound by the time the hitobjects needs 
			// to be hit, then the aimobject got to the hitobject early. Wait on the hit object until it's 
			// time to click if so. Otherwise, kill the acceleration/deceleration (do nothing).

			// \TODO: factor in OD, giving leniency
			if (t /*+eta*/ /*< _hitcircles[index].getTime())
			{
				// calculate the deceleration needed for the aimobject not to go out of the hitobject's bounds. 

				// calculate the x-y components of the deceleration

				// apply the calculated required deceleration or the max allow deceleration, which ever is smaller
			}
		}
		else
		{
			// apply acceleration. deceleration, or none as needed

		}
	}*/
	return  vector2df();
}

/*
int getHitcircleAt(std::vector<Hitcircle>& _hitcircles, int _time)
{
	int start = 0;
	int end = _hitcircles.size() - 2;
	int mid;

	while (start <= end)
	{
		mid = (start + end) / 2;
		if (BTWN(_hitcircles[mid].getTime(), _time, _hitcircles[mid + 1].getTime() - 1))
			return mid;
		else if (_time < _hitcircles[mid].getTime())
			end = mid - 1;
		else start = mid + 1;
	}

	return -1;
}*/

/*int getNumVisibleAt(std::vector<Hitcircle>& _hitcircles, int _time, double _AR, bool _hidden, double _opacity)
{
	int index = getHitcircleAt(_hitcircles, _time);
	int count = 1;

	for (int i = index + 1; i < _hitcircles.size(); i++)
	{
		std::pair<int, int> visibilityTimes = _hitcircles[i].getVisiblityTimes(_AR, _hidden, _opacity, _opacity);
		if (!BTWN(visibilityTimes.first, _time, visibilityTimes.second))
			break;

		count++;
	}

	return count;
}*/

/*std::pair<int, int> getIndicesVisibleAt(std::vector<Hitcircle>& _hitcircles, int _time, double _AR, bool _hidden, double _opacity)
{
	int index = getHitcircleAt(_hitcircles, _time);
	std::pair<int, int> range;

	if (index >= 0)
	{
		// Find first note visible
		for (; index < _hitcircles.size(); index++)
		{
			if (_hitcircles[index].isVisible(_time, _AR, _hidden))
			{
				range.first = index;
				break;
			}
		}
		
		// Find last note visible
		for (; index < _hitcircles.size(); index++)
		{
			std::pair<int, int> visibilityTimes = _hitcircles[index].getVisiblityTimes(_AR, _hidden, _opacity, _opacity);
			if (!BTWN(visibilityTimes.first, _time, visibilityTimes.second))
			{
				range.second = index;
				break;
			}
		}
	}

	return range;
}*/

double getOverLapSurfaceArea(Beatmap* _beatmap, int _time, bool _hidden, double _opacity)
{
	std::pair<int, int> range = _beatmap->getIndicesVisibleAt(_time, _opacity);
	double area = 0.0;

	for (int i = range.first; i < range.second; i++)
	{
		for (int j = i + 1; j < range.second; j++)
		{
			area += getCircleOverlapArea(CS2px(_beatmap->getDiff().cs) / 2.0, getDist(_beatmap->hitObjects[i]->getPos(), _beatmap->hitObjects[j]->getPos()));
		}
	}

	return area;
}