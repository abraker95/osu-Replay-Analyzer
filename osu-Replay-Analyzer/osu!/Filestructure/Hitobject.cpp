#include "Hitobject.h"
#include "../osuCalc.h"
#include "../Filestructure/Beatmap.h"
#include "../Filestructure/filereader.h"

Hitobject::Hitobject() : slider(nullptr){}

Hitobject::Hitobject(std::vector<std::string> &_objectData, std::vector<std::string> &_sliderData) : slider(this)
{
	//slider = nullptr;

	ProcessHitobjectData(_objectData);
	ProcessSliderData(_objectData, _sliderData);
}

Hitobject::~Hitobject()
{
	//if(slider != nullptr) 
		slider.~Slider();
	//slider.reset();
}

bool Hitobject::IsHitObjectType(int _type)
{
	return (_type & this->type) > 0;
}
 
int Hitobject::getHitobjectType()
{
	return this->type;
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

int Hitobject::getEndTime() const
{
	int hitObjectType = this->type;

	if (hitObjectType & (HITOBJECTYPE::CIRCLE))
	{
		return this->time;
	}

	if (hitObjectType & (HITOBJECTYPE::SLIDER))
	{
		return this->time + this->slider.toRepeatTime * this->slider.repeat;
	}

	if (hitObjectType & (HITOBJECTYPE::SPINNER))
	{
		return this->slider.endTime;
	}

	if (hitObjectType & (HITOBJECTYPE::MANIALONG))
	{
		return this->slider.endTime;
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

std::pair<int, int> Hitobject::getVisiblityTimes(double _AR, bool _hidden, double _opacityStart, double _opacityEnd)
{
	double preampTime = this->time - AR2ms(_AR);	// Time when the AR goes into effect
	std::pair<int, int> times;

	if (_hidden)
	{
		double fadeinDuration = 0.4*AR2ms(_AR);				// how long the fadein period is
		double fadeinTimeEnd = preampTime + fadeinDuration; // When it is fully faded in

		times.first = getValue(preampTime, fadeinTimeEnd, _opacityStart);


		// If it's a slider, then the fade out period lasts from when it's fadedin to
		// 70% to the time it the slider ends
		if (this->IsHitObjectType(SLIDER))
		{
			double fadeoutDuration = (this->slider.endTime - fadeinTimeEnd); // how long the fadeout period is
			double fadeoutTimeEnd = fadeinTimeEnd + fadeoutDuration;		   // When it is fully faded out
			times.second = getValue(fadeinTimeEnd, fadeoutTimeEnd, 1.0 - _opacityEnd);

			return times;
		}
		else
		{
			double fadeoutDuration = 0.7*(this->time - fadeinTimeEnd);		// how long the fadeout period is
			double fadeoutTimeEnd = fadeinTimeEnd + fadeoutDuration;	// When it is fully faded out
			times.second = getValue(fadeinTimeEnd, fadeoutTimeEnd, 1.0 - _opacityStart); // <-- no this is not a mistake :D

			return times;
		}
	}
	else
	{
		double fadeinDuration = MIN(AR2ms(_AR), 400);		// how long the fadein period is
		double fadeinTimeEnd = preampTime + fadeinDuration; // When it is fully faded in

		// Fadein period always lasts from preamp time to 400 ms after preamp time or
		// when the object needs to be hit, which ever is smaller
		times.first = getValue(preampTime, fadeinTimeEnd, _opacityStart);

		// If it is during the slider hold period, then it's fully visible.
		// Otherwise, it's not visible anymore.
		if (this->IsHitObjectType(SLIDER))
		{
			times.second = this->slider.endTime;
			return times;
		}
		else
		{
			times.second = this->time;
			return times;
		}
	}
}

bool Hitobject::isVisibleAt(int _time, double _AR, bool _hidden)
{
	double opacity = this->getOpacityAt(_time, _AR, _hidden);
	if (opacity > 0.0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

double Hitobject::getOpacityAt(int _time, double _AR, bool _hidden)
{
	double preampTime = this->time - AR2ms(_AR);	// Time when the AR goes into effect

	if (_hidden)
	{
		double fadeinDuration = 0.4*AR2ms(_AR);				// how long the fadein period is
		double fadeinTimeEnd = preampTime + fadeinDuration; // When it is fully faded in

		// Fadein period always lasts from preamp time to 40% from preamp time to hit time
		double percentFadeIn = getPercent(preampTime, _time, fadeinTimeEnd);

		// If it's not fully faded in, then we haven't gotten up to the later stuff 
		if (percentFadeIn < 1.0)
		{
			return percentFadeIn;
		}
		else // fadeout
		{
			// If it's a slider, then the fade out period lasts from when it's fadedin to
			// 70% to the time it the slider ends
			if (this->IsHitObjectType(SLIDER))
			{
				double fadeoutDuration = (slider.GetLastTickTime() - fadeinTimeEnd); // how long the fadeout period is
				double fadeoutTimeEnd = fadeinTimeEnd + fadeoutDuration;		   // When it is fully faded out
				return (1.0 - getPercent(fadeinTimeEnd, _time, fadeoutTimeEnd));
			}
			else
			{
				double fadeoutDuration = 0.7*(this->time - fadeinTimeEnd);		// how long the fadeout period is
				double fadeoutTimeEnd = fadeinTimeEnd + fadeoutDuration;	// When it is fully faded out
				return (1.0 - getPercent(fadeinTimeEnd, _time, fadeoutTimeEnd));
			}
		}
	}
	else
	{
		double fadeinDuration = MIN(AR2ms(_AR), 400);		// how long the fadein period is
		double fadeinTimeEnd = this->time; // When it is fully faded in

		// Fadein period always lasts from preamp time to 400 ms after preamp time or
		// when the object needs to be hit, which ever is smaller
		double percentFadeIn = getPercent(preampTime, _time, fadeinTimeEnd);

		// If it's not fully faded in, then we haven't gotten up to the later stuff 
		if (percentFadeIn < 1.0)
		{
			return percentFadeIn;
		}
		else // fadeout
		{
			// If it is during the slider hold period, then it's fully visible.
			// Otherwise, it's not visible anymore.
			if (this->IsHitObjectType(SLIDER))
			{
				if (_time > slider.getEndTime())
				{
					// \TODO: Opacity when pressed and when missed
					return 0.0;
				}
				else
				{
					return 1.0;
				}
			}
			else
			{
				if (_time > this->time)
				{
					// \TODO: Opacity when pressed and when missed
					return 0.0;
				}
				else
				{
					return 1.0;
				}
			}
		}
	}
}


bool Hitobject::isHitobjectLong()
{
	return slider.curve.size() != 0;
}

//  ------------ [PRIVATE] ---------------

void Hitobject::ProcessHitobjectData(std::vector<std::string> &_objectData)
{
	this->pos = irr::core::vector2d<double>(atof(_objectData[0].c_str()), atof(_objectData[1].c_str()));
	this->time = atoi(_objectData[2].c_str());
	this->type = atoi(_objectData[3].c_str());
}

void Hitobject::ProcessSliderData(std::vector<std::string> &_objectData, std::vector<std::string> &_sliderData)
{
	
	if (this->getHitobjectType() & HITOBJECTYPE::SLIDER)
	{
		// curve parsing
		slider.curveType = (CurveType)_sliderData[0].c_str()[0];

		for (int i = 1; i < _sliderData.size(); i++)
		{
			std::vector<std::string> curveTokens;
			FileReader::tokenize(_sliderData[i], curveTokens, ":");

			// curve points parsing
			{
				irr::core::vector2di curve;
					curve.X = atoi(curveTokens[0].c_str());
					curve.Y = atoi(curveTokens[1].c_str());

				slider.curve.push_back(curve);
			}
		}

		slider.repeat = atoi(_objectData[6].c_str());
		slider.pixelLength = atof(_objectData[7].c_str());
	}

	if (this->getHitobjectType() & HITOBJECTYPE::SPINNER)
	{
		slider.endTime = atoi(_objectData[5].c_str());
	}

	if (this->getHitobjectType() & HITOBJECTYPE::MANIALONG)
	{
		FileReader::tokenize(_objectData[5], _sliderData, ":");
		slider.endTime = atoi(_sliderData[0].c_str());
	}
}