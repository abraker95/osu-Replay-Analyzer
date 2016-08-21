#include "SliderHitObject.h"
#include "filereader.h"

SliderHitObject::SliderHitObject(std::vector<std::string> &_objectData, std::vector<std::string> &_sliderData) : Hitobject(_objectData)
{
	this->toRepeatTime = -1;
	this->endTime = -1;

	PreprocessSliderData(_objectData, _sliderData);
}

void SliderHitObject::GenerateSlider(SLIDERTYPE _sliderType)
{
	genCurve.clear();

	switch (_sliderType)
	{
		case LINEAR:
			this->MakeLinear();
			break;

		case BEZIER:
			this->MakeBezier();
			break;

		case CIRCUMSCRIBED:
			this->MakeCircle();
			break;
		
		default:
			break;
	};
}

void SliderHitObject::MakeLinear()
{
	// Lines: generate a new curve for each sequential pair
	// ab  bc  cd  de  ef  fg

	std::vector<Bezier> beziers;
	for (int i = 0; i < this->readCurvePoints.size(); i++)
		beziers.push_back(Bezier({ getReadPoint(i), getReadPoint(i + 1) }));

	init(beziers);
}

void SliderHitObject::MakeBezier()
{
	// Beziers: splits points into different Beziers if has the same points (red points)
	// a b c - c d - d e f g

	std::vector<Bezier> beziers;
	std::vector<irr::core::vector2di> points;

	for (int i = 0; i <= this->readCurvePoints.size() + 1; i++)
	{
		// Don't record beyond what the list provides
		if(i < this->readCurvePoints.size() + 1)
			points.push_back(getReadPoint(i));

		// If we reached a red point or the end of the point list, then segment the bezier
		if (getReadPoint(i) == getReadPoint(i + 1) || (i > this->readCurvePoints.size()))
		{
			beziers.push_back(Bezier(points));
			points.clear();
		}
	}

	init(beziers);
}

SliderHitObject::~SliderHitObject()
{ 
	ticks.clear();
	genCurve.clear();
	readCurvePoints.clear();

	std::vector<int>().swap(ticks);
	std::vector<irr::core::vector2di>().swap(genCurve);
	std::vector<irr::core::vector2di>().swap(readCurvePoints);
}

irr::core::vector2d<double> SliderHitObject::GetSliderPos(double time)
{
	// convert time to percent
	double percent = getPercent(this->time, time, this->endTime);

	// convert percent to index
	double indexPercent = percent*genCurve.size();
	double indexPos = Triangle(indexPercent*repeat, (2 * genCurve.size()) - 1);

	if (indexPos + 1 >= genCurve.size()) // last point
	{
		irr::core::vector2di poi = this->genCurve[genCurve.size() - 1];
		return irr::core::vector2d<double>(poi.X, poi.Y);
	}
	else // every other point
	{
		irr::core::vector2di poi = this->genCurve[indexPos];
		irr::core::vector2di poi2 = this->genCurve[indexPos + 1];
		
		int indexPosRound = indexPos;
		double percentPoint = indexPos - indexPosRound;  // percent between this point and next

		irr::core::vector2d<double> p = irr::core::vector2d<double>(lerp(poi.X, poi2.X, percentPoint), lerp(poi.Y, poi2.Y, percentPoint));
		return p;
	}
}

int SliderHitObject::getTimeDistFrom(int _time, int _dist, bool _dir)
{
	irr::core::vector2d<double> sliderPos = GetSliderPos(_time);
	int start = 0, end = 0, mid = 0;

	if (_dir == 0)
	{
		int start = getTime();
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

int SliderHitObject::getEndTime()
{
	int hitObjectType = getHitobjectType();

	if (hitObjectType & (HITOBJECTYPE::CIRCLE))
	{
		return time;
	}

	if (hitObjectType & (HITOBJECTYPE::SLIDER))
	{
		return this->time + this->toRepeatTime * this->repeat;
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

int SliderHitObject::GetLastTickTime()
{
	double tickInterval = 0;

	if (!this->ticks.size())
	{
		if (this->repeat > 1)
			return this->endTime - (this->endTime - this->repeatTimes.back()) / 2.0;
		else
			return this->endTime - (this->endTime - this->time) / 2.0;
	}

	else
		return this->endTime - (this->endTime - this->ticks.back()) / 2.0;
}

double SliderHitObject::getLength()
{
	return pixelLength;
}

double SliderHitObject::getVelocity()
{
	double period = this->GetLastTickTime() - this->time;
	double length = this->getLength();
	return length / period;
}

std::vector<int> SliderHitObject::getTickTimes()
{
	return ticks;
}

void SliderHitObject::RecordRepeatTimes()
{
	// Make sure we have the correct data for recording
	if (endTime == -1)	   return;
	if (toRepeatTime <= 1) return;

	// Record the time of the repeats
	for (int i = this->time; i < this->endTime; i += this->toRepeatTime)
	{
		if (i > this->endTime)	break;
		this->repeatTimes.push_back(i);
	}
}

void SliderHitObject::RecordTickIntervals(int _beatmapTickInterval)
{
	const int errInterval = 10;
	int j = 1;

	for (int i = this->time + _beatmapTickInterval; i < (this->endTime - errInterval); i += _beatmapTickInterval)
	{
		if (i > this->endTime)
			break;
		int tickTime = this->time + _beatmapTickInterval * j;
		this->ticks.push_back(tickTime);
		j++;
	}
}

// ----------------- [PRIVATE] ------------------

void SliderHitObject::init(std::vector<Bezier> _curvesList)
{
	for (Bezier& curve : _curvesList)
	{
		for (irr::core::vector2di point : curve.getCurvePoint())
			genCurve.push_back(point);
	}
}

void SliderHitObject::PreprocessSliderData(std::vector<std::string> &_objectData, std::vector<std::string> &_sliderData)
{
	// curve parsing
	curveType = (CurveType)_sliderData[0].c_str()[0];

	for (int i = 1; i < _sliderData.size(); i++)
	{
		std::vector<std::string> curveTokens;
		FileReader::tokenize(_sliderData[i], curveTokens, ":");

		// curve points parsing
		{
			irr::core::vector2di curve;
				curve.X = atoi(curveTokens[0].c_str());
				curve.Y = atoi(curveTokens[1].c_str());

			readCurvePoints.push_back(curve);
		}
	}

	if (this->getHitobjectType() & HITOBJECTYPE::SPINNER)
	{
		endTime = atoi(_objectData[5].c_str());
		return;
	}

	if (this->getHitobjectType() & HITOBJECTYPE::MANIALONG)
	{
		FileReader::tokenize(_objectData[5], _sliderData, ":");
		endTime = atoi(_sliderData[0].c_str());
		return;
	}

	// otherwise this is a osu!std slider and we should get additional data
	repeat = atoi(_objectData[6].c_str());
	pixelLength = atof(_objectData[7].c_str());
}

irr::core::vector2di SliderHitObject::getReadPoint(int _i)
{
	if (_i > readCurvePoints.size()) return irr::core::vector2di(INT_MIN, INT_MIN);

	// The first actual point is the slider's starting position, followed by all other read points
	return (_i == 0) ? irr::core::vector2di(pos.X, pos.Y) : readCurvePoints[_i - 1];
}
