#include "Hitcircle.h"
#include "../../utils/mathUtils.h"
#include "../../ui/drawUtils.h"

#include "../osuCalc.h"
#include "../osu_standard.h"

#include "../Filestructure/SliderHitObject.h"

#include <iostream>
#include <tuple>

Hitcircle::Hitcircle(Mods* _mods, Hitobject* _hitobject, long* _viewTime)
	: GuiObj(_hitobject->getPos().X, _hitobject->getPos().Y, CS2px(_mods->getCS()), CS2px(_mods->getCS()))
{
	mods = _mods;
	hitobject = _hitobject;
	viewTime = _viewTime;

	state = IDLE;
	edgeCol = IDLE_COLOR;
}

Hitcircle::~Hitcircle()
{

}

void Hitcircle::Draw(Window &_win)
{
	double opacity = OSUSTANDARD::getOpacityAt(*hitobject, *(this->viewTime), mods->getAR(), mods->getModifier().HD);
	SColor fade = SColor(255*opacity, edgeCol.getRed(), edgeCol.getGreen(), edgeCol.getBlue());
	
	vector2df radius(this->width / 2.0, this->height / 2.0);
	DrawArc(_win, this->absXpos, this->absYpos, MIN(radius.X, radius.Y), fade);

	// draw slider
	if (hitobject->IsHitObjectType(SLIDER))
	{
		double velocity = hitobject->getSlider()->getVelocity();
		double itrStep = 1.0 / velocity;
	
		long startTime = hitobject->getTime(),
			 endTime = hitobject->getSlider()->getEndTime();
		 
		if (velocity != 0.0)
		{
			for (double t = startTime; t < endTime - itrStep; t += MAX(1, itrStep))
			{
				vector2d<double> currSliderPoint = hitobject->getSlider()->GetSliderPos(t);
				vector2d<double> nextSliderPoint = hitobject->getSlider()->GetSliderPos(MIN(t + itrStep, endTime - itrStep));

				_win.driver->draw2DLine(vector2di(currSliderPoint.X*getWidthRatio() + parent->getPos().X, currSliderPoint.Y*getHeightRatio() + parent->getPos().Y),
										vector2di(nextSliderPoint.X*getWidthRatio() + parent->getPos().X, nextSliderPoint.Y*getHeightRatio() + parent->getPos().Y),
										fade);

				//_win.driver->drawPixel(sliderPoint.X*getWidthRatio() + parent->getPos().X, sliderPoint.Y*getHeightRatio() + parent->getPos().Y, fade);

				if (BTWN(t, *(this->viewTime), t + itrStep))
					DrawArc(_win, currSliderPoint.X*getWidthRatio() + parent->getPos().X, currSliderPoint.Y*getHeightRatio() + parent->getPos().Y, 5, fade);  // draw slider point
			}
		}
	}
}

/*position2di Hitcircle::getEndPoint()
{
	if (this->isSlider())
	{
		std::tuple<int, int, int> sliderPoint = this->sliders[this->sliders.size() - 1];
		return position2di(std::get<XPOS>(sliderPoint), std::get<YPOS>(sliderPoint));
	}
	else
	{
		return position2di(this->x, this->y);
	}
}*/

/*
int Hitcircle::getHoldPeriod()
{
	return this->getEndTime() - this->getTime();
}

position2di Hitcircle::getSliderPointAt(int _index)
{
	if (_index < sliders.size())
		return position2di(std::get<XPOS>(sliders[_index]), std::get<YPOS>(sliders[_index]));
	else
		return position2di(-1, -1);
}

int Hitcircle::getSliderTimeAt(int _index)
{
	if (_index < sliders.size())
		return std::get<TIME>(sliders[_index]);
	else
		return -1;
}*/

// ------ [PRIVATE] ------

void Hitcircle::IdleLogic(Window &_win)
{
	if (this->isMouseOnObj(_win, true))
		state = HIGHLIGHTED;
	else
		edgeCol = IDLE_COLOR;
}

void Hitcircle::HighlightedLogic(Window &_win)
{
	if (!this->isMouseOnObj(_win, true))
		state = IDLE;
	else
	{
		bool leftEdge = _win.reciever.GetMouseState().LeftButtonEdge;
		if (leftEdge)
		{
			state = SELECTED;
		}

		edgeCol = HIGHLIGHTED_COLOR;
	}
}

void Hitcircle::SelectedLogic(Window &_win)
{
	position2di mouseDelta = _win.reciever.getCursorDelta();
	bool leftButton = _win.reciever.GetMouseState().LeftButtonDown;
	bool leftEdge = _win.reciever.GetMouseState().LeftButtonEdge;
	
	// \TODO: Slider points; See if slider gen and update code can be simplified

	if (!this->isMouseOnObj(_win, true) && leftEdge)
		state = IDLE;
	else if (leftButton)
	{
		// has to be += due the fact that x - xoffset = 0, 
		// resolving the assignment to x = pos.x
		// this->x = pos.X - this->xOffset + this->x  => this->x = pos.X - 0;

		this->xpos += mouseDelta.X;
		this->ypos += mouseDelta.Y;
		
		edgeCol = SELECTED_COLOR;
	}
}

void Hitcircle::UpdateInternal(Window &_win)
{
	UpdateDimentions();

	switch (state)
	{
		case IDLE:
			IdleLogic(_win);
			break;

		case HIGHLIGHTED:
			HighlightedLogic(_win);
			break;

		case SELECTED:
			SelectedLogic(_win);
			break;

		default:
			// ???
			break;
	}
}

void Hitcircle::UpdateDimentions()
{
	xpos = hitobject->getPos().X*getWidthRatio();
	ypos = hitobject->getPos().Y*getHeightRatio();

	int diameter = MIN(CS2px(mods->getCS())*getWidthRatio(),
					   CS2px(mods->getCS())*getHeightRatio());

	width = diameter;
	height = diameter;
}

double Hitcircle::getWidthRatio()
{
	return parent->getDim().Width / 512.0;
}

double Hitcircle::getHeightRatio()
{
	return parent->getDim().Height / 386.0;
}