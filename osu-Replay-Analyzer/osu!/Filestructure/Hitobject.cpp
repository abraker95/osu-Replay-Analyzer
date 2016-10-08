#include "Hitobject.h"
#include "../osuCalc.h"
#include "../Filestructure/Beatmap.h"
#include "../Filestructure/filereader.h"
#include "../Filestructure/SliderHitObject.h"

Hitobject::Hitobject(std::vector<std::string> &_objectData)
{
	ProcessHitobjectData(_objectData);
}

Hitobject::~Hitobject(){}

bool Hitobject::IsHitObjectType(int _type)
{
	return (_type & this->type) > 0;
}
 
int Hitobject::getHitobjectType()
{
	return this->type;
}

SliderHitObject* Hitobject::getSlider()
{
	return reinterpret_cast<SliderHitObject*>(this);
}

irr::core::vector2d<double> Hitobject::getPos() const
{
	return this->pos;
}

void Hitobject::setPos(irr::core::vector2d<double> _pos)
{
	this->pos = _pos;
}

int Hitobject::getTime() const
{
	return this->time;
}

int Hitobject::getEndTime()
{
	int hitObjectType = this->type;

	if (hitObjectType & (HITOBJECTYPE::CIRCLE))
	{
		return this->time;
	}

	if (hitObjectType & (HITOBJECTYPE::SLIDER))
	{
		return this->getSlider()->endTime;
	}

	if (hitObjectType & (HITOBJECTYPE::SPINNER))
	{
		return this->getSlider()->endTime;
	}

	if (hitObjectType & (HITOBJECTYPE::MANIALONG))
	{
		return this->getSlider()->endTime;
	}
}


void Hitobject::setTime(Beatmap* _beatmap, int _time)
{
	/*int newIndex = _beatmap->FindHitobjectAt(_time);
	int currIndex = this->index;

	/// \TODO: IDK if this works or not... expecting crash :c
	if (newIndex != currIndex)
	{
		for (int i = currIndex; i >= newIndex; i--)
			_beatmap->hitObjects[i + 1] = _beatmap->hitObjects[i];
		_beatmap->hitObjects[newIndex] = this;
	}
	
	this->index = newIndex;*/
	this->time = _time;
}

double Hitobject::GetCircleOverlapPercentage(int CS, Hitobject &c1)
{
	// \TODO: account for sliders
	double distance = c1.pos.getDistanceFrom(this->pos);
	double radius = CS2px(CS);
	double result = 1 - distance / (radius * 2);
	return (result < 0) ? 0 : result;
}

// Useful for sliders. Returns the timing of a slider end to which a point in time is closer to
long Hitobject::getCloserTime(long _time)
{
	long sliderEndTime = this->getEndTime();
	long sliderSrtTime = this->getTime();
	long sliderMidTime = (sliderEndTime + sliderSrtTime) / 2;

	if (sliderMidTime < _time) _time = sliderEndTime;
	else					   _time = sliderSrtTime;

	return _time;
}

bool Hitobject::isHitobjectLong()
{
	return !IsHitObjectType(HITOBJECTYPE::CIRCLE);
}

//  ------------ [PRIVATE] ---------------

void Hitobject::ProcessHitobjectData(std::vector<std::string> &_objectData)
{
	this->pos = irr::core::vector2d<double>(atof(_objectData[0].c_str()), atof(_objectData[1].c_str()));
	this->time = atoi(_objectData[2].c_str());
	this->type = atoi(_objectData[3].c_str());
}