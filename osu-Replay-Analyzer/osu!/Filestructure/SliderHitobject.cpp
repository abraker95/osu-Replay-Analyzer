#include "Hitobject.h"

Hitobject::Slider::Slider(Hitobject* object) : hitobject(object), circle(nullptr) 
{
	this->toRepeatTime = -1;
}

void Hitobject::Slider::newSlider(Hitobject* object, bool isline, bool iscircle)
{
	if (object != nullptr)
	{
		std::vector<Bezier> beziers;

		// Beziers: splits points into different Beziers if has the same points (red points)
		// a b c - c d - d e f g
		// Lines: generate a new curve for each sequential pair
		// ab  bc  cd  de  ef  fg
		int controlPoints = this->curve.size() + 1;
		std::vector<irr::core::vector2di> points;  // temporary list of points to separate different Bezier curves
		irr::core::vector2di lastPoi(-1, -1);

		for (int i = 0; i < this->curve.size(); i++)
		{
			sliderX.push_back(this->curve[i].X);
			sliderY.push_back(this->curve[i].Y);
		}

		x = hitobject->pos.X;
		y = hitobject->pos.Y;

		for (int i = 0; i < controlPoints; i++)
		{
			irr::core::vector2di tpoi = irr::core::vector2di(getX(i), getY(i));
			if (isline)
			{
				if (lastPoi != irr::core::vector2di(-1, -1))
				{
					points.push_back(tpoi);
					beziers.push_back(Bezier(points));
					points.clear();
				}
			}
			else if ((lastPoi != irr::core::vector2di(-1, -1)) && (tpoi == lastPoi))
			{
				if (points.size() >= 2)
					beziers.push_back(Bezier(points));
				points.clear();
			}
			points.push_back(tpoi);
			lastPoi = tpoi;
		}

		if (isline || points.size() < 2)
		{
			// trying to continue Bezier with less than 2 points
			// probably ending on a red point, just ignore it
		}
		else
		{
			beziers.push_back(Bezier(points));
			points.clear();
		}

		init(beziers);

		if (iscircle)
		{
			this->circle = std::make_unique<CircumscribedCircle>(this);
		}
			
	}
}

Hitobject::Slider::~Slider()
{ 
	//if (circle != nullptr)
	//	delete circle; 
}

/*void Hitobject::Slider::operator=(Slider slider)
{
	*this = slider;
}*/

irr::core::vector2di Hitobject::Slider::GetSliderPos(int time)
{
	// convert time to percent
	double percent;
	if (time <= hitobject->time)
		percent = 0;
	else if (time > this->endTime)
		percent = 1;
	else
	{
		int timeLength = (time - hitobject->time);
		int repeatsDone = (int)(timeLength / this->toRepeatTime);
		percent = (timeLength - this->toRepeatTime * repeatsDone) / (double)this->toRepeatTime;
		if (repeatsDone % 2)
			percent = 1 - percent; // it's going back
	}
		
	// get the points
	int ncurve = this->ncurve;
	double indexF = percent * ncurve;
	int index = (int)indexF;
	
	if (index >= this->ncurve)
	{
		irr::core::vector2di poi = this->genCurve[ncurve];
		return irr::core::vector2di(poi.X, poi.Y);
	}
	else
	{
		irr::core::vector2di poi = this->genCurve[index];
		irr::core::vector2di poi2 = this->genCurve[index + 1];
		double t2 = indexF - index;
		return irr::core::vector2di(lerp(poi.X, poi2.X, t2), lerp(poi.Y, poi2.Y, t2));
	}
}

int Hitobject::Slider::getTimeDistFrom(int _time, int _dist, bool _dir)
{
	irr::core::vector2di sliderPos = GetSliderPos(_time);
	int start = 0, end = 0, mid = 0;

	if (_dir == 0)
	{
		int start = hitobject->getTime();
		int end = _time;
		int mid;
	}
	else
	{
		int start = _time;
		int end = this->GetLastTickTime();
		int mid;
	}

	while (start <= end)
	{
		double currDist = sliderPos.getDistanceFrom(GetSliderPos(mid));

		mid = (start + end) / 2;
		if (BTWN(_dist, currDist, _dist + 1.0))
			return mid;
		else if (_dist < currDist)
			end = mid - 1;
		else start = mid + 1;
	}

	return -1;
}

int Hitobject::Slider::getEndTime()
{
	int hitObjectType = this->hitobject->getHitobjectType();

	if (hitObjectType & (HITOBJECTYPE::CIRCLE))
	{
		return this->hitobject->time;
	}

	if (hitObjectType & (HITOBJECTYPE::SLIDER))
	{
		return this->hitobject->time + this->toRepeatTime * this->repeat;
	}
	
	if (hitObjectType & (HITOBJECTYPE::SPINNER))
	{
		return this->endTime;
	}

	if (hitObjectType & (HITOBJECTYPE::MANIALONG))
	{
		return this->endTime;
	}
}

int Hitobject::Slider::GetLastTickTime()
{
	double tickInterval = 0;

	if (!this->ticks.size())
	{
		if (this->repeat > 1)
			return this->endTime - (this->endTime - this->repeatTimes.back()) / 2.0;
		else
			return this->endTime - (this->endTime - this->hitobject->time) / 2.0;
	}

	else
		return this->endTime - (this->endTime - this->ticks.back()) / 2.0;
}

double Hitobject::Slider::getLength()
{
	return pixelLength;
}

double Hitobject::Slider::getVelocity()
{
	double period = this->GetLastTickTime() - hitobject->time;
	double length = this->getLength();
	return length / period;
}

void Hitobject::Slider::RecordRepeatTimes()
{
	// Saving the time of repeats
	if (this->repeat > 1)
	{
		for (int i = this->hitobject->time; i < this->endTime; i += this->toRepeatTime)
		{
			if (i > this->endTime)
				break;
			this->repeatTimes.push_back(i);
		}
	}
}

void Hitobject::Slider::RecordTickIntervals(int _beatmapTickInterval)
{
	const int errInterval = 10;
	int j = 1;

	for (int i = this->hitobject->time + _beatmapTickInterval; i < (this->endTime - errInterval); i += _beatmapTickInterval)
	{
		if (i > this->endTime)
			break;
		int tickTime = this->hitobject->time + _beatmapTickInterval * j;
		this->ticks.push_back(tickTime);
		j++;
	}
}

// ----------------- [PRIVATE] ------------------

void Hitobject::Slider::init(std::vector<Bezier> curvesList)
{
	// now try to creates points the are equidistant to each other
	ncurve = (int)(this->pixelLength / CURVE_POINTS_SEPERATION);
	genCurve.resize(ncurve + 1);

	double distanceAt = 0;
	int curveCounter = 0;
	int curPoint = 0;
	Bezier curCurve = curvesList[curveCounter++];
	irr::core::vector2di lastCurve = curCurve.getCurvePoint()[0];
	double lastDistanceAt = 0;

	// length of Curve should equal pixel length (in 640x480)
	double pixelLength = this->pixelLength;

	// for each distance, try to get in between the two points that are between it
	for (int i = 0; i < ncurve + 1; i++) 
	{
		int prefDistance = (int)(i * pixelLength / ncurve);
		while (distanceAt < prefDistance)
		{
			lastDistanceAt = distanceAt;
			lastCurve = curCurve.getCurvePoint()[curPoint];
			curPoint++;

			if (curPoint >= curCurve.getCurvesCount())
			{
				if (curveCounter < curvesList.size())
				{
					curCurve = curvesList[curveCounter++];
					curPoint = 0;
				}
				else 
				{
					curPoint = curCurve.getCurvesCount() - 1;
					
					// out of points even though the preferred distance hasn't been reached
					if (lastDistanceAt == distanceAt) break;	
				}
			}
			distanceAt += curCurve.getCurveDistances()[curPoint];
		}
		irr::core::vector2di thisCurve = curCurve.getCurvePoint()[curPoint];

		// interpolate the point between the two closest distances
		if (distanceAt - lastDistanceAt > 1) 
		{
			double t = (prefDistance - lastDistanceAt) / (distanceAt - lastDistanceAt);
			genCurve[i] = irr::core::vector2di(lerp(lastCurve.X, thisCurve.X, t), lerp(lastCurve.Y, thisCurve.Y, t));
		}
		else
			genCurve[i] = thisCurve;
	}

	//	if (hitObject.getRepeatCount() > 1) {
	
	// start angle
	irr::core::vector2di c1 = genCurve[0];
	int cnt = 1;
	irr::core::vector2di c2 = genCurve[cnt++];

	while (cnt <= ncurve && c2.getDistanceFrom(c1) < 1)
		c2 = genCurve[cnt++];

	startAngle = (double)(atan2(c2.Y - c1.Y, c2.X - c1.X) * 180 / M_PI);

	// end angle
	c1 = genCurve[ncurve];
	cnt = ncurve - 1;
	c2 = genCurve[cnt--];
	
	while (cnt >= 0 && c2.getDistanceFrom(c1) < 1)
		c2 = genCurve[cnt--];
	
	endAngle = (double)(atan2(c2.Y - c1.Y, c2.X - c1.X) * 180 / M_PI);
	//	}
}

double Hitobject::Slider::getEndAngle() { return endAngle; }
double Hitobject::Slider::getStartAngle() { return startAngle; }


double Hitobject::Slider::getX(int i) { return (i == 0) ? x : sliderX[i - 1]; }
double Hitobject::Slider::getY(int i) { return (i == 0) ? y : sliderY[i - 1]; }
