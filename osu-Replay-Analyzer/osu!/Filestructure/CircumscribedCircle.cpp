#include "Hitobject.h"
#include <iostream>

Hitobject::Slider::CircumscribedCircle::CircumscribedCircle(Slider* _slider) : slider(_slider)
{
	if (_slider != nullptr)
	{
		for (int i = 0; i < slider->curve.size(); i++)
		{
			slider->sliderX.push_back(slider->curve[i].X);
			slider->sliderY.push_back(slider->curve[i].Y);
		}

		slider->x = slider->hitobject->pos.X;
		slider->y = slider->hitobject->pos.Y;

		// construct the three points
		this->start = irr::core::vector2di(slider->getX(0), slider->getY(0));
		this->mid = irr::core::vector2di(slider->getX(1), slider->getY(1));
		this->end = irr::core::vector2di(slider->getX(2), slider->getY(2));

		// find the circle center
		irr::core::vector2di mida = start.midPoint(mid);
		irr::core::vector2di midb = end.midPoint(mid);
		irr::core::vector2di nora = (mid - start).nor();
		irr::core::vector2di norb = (mid - end).nor();

		this->circleCenter = intersect(mida, nora, midb, norb);
		if (circleCenter == irr::core::vector2di(-1, -1))
		{
			// Temporary fallback to bezier slider
			//Slider slider(slider, true);
			return;
		}

		// find the angles relative to the circle center
		irr::core::vector2di startAngPoint = start - circleCenter;
		irr::core::vector2di midAngPoint = mid - circleCenter;
		irr::core::vector2di endAngPoint = end - circleCenter;

		this->startAng = atan2(startAngPoint.Y, startAngPoint.X);
		this->midAng = atan2(midAngPoint.Y, midAngPoint.X);
		this->endAng = atan2(endAngPoint.Y, endAngPoint.X);

		// find the angles that pass through midAng
		if (!isIn(startAng, midAng, endAng))
		{
			if (abs(startAng + TWO_PI - endAng) < TWO_PI && isIn(startAng + (TWO_PI), midAng, endAng))
				startAng += TWO_PI;
			else if (abs(startAng - (endAng + TWO_PI)) < TWO_PI && isIn(startAng, midAng, endAng + (TWO_PI)))
				endAng += TWO_PI;
			else if (abs(startAng - TWO_PI - endAng) < TWO_PI && isIn(startAng - (TWO_PI), midAng, endAng))
				startAng -= TWO_PI;
			else if (abs(startAng - (endAng - TWO_PI)) < TWO_PI && isIn(startAng, midAng, endAng - (TWO_PI)))
				endAng -= TWO_PI;
			else
			{
				std::cout << "Cannot find angles between midAng (%.3f %.3f %.3f)." << std::endl;
				//startAng, midAng, endAng), null, true
				return;
			}
		}

		// find an angle with an arc length of pixelLength along this circle
		this->radius = startAngPoint.getLength();
		double pixelLength = slider->pixelLength;
		double arcAng = pixelLength / radius;  // len = theta * r / theta = len / r

		// now use it for our new end angle
		this->endAng = (endAng > startAng) ? startAng + arcAng : startAng - arcAng;

		// calculate points
		double step = slider->pixelLength / slider->CURVE_POINTS_SEPERATION;
		slider->ncurve = (int)step;
		int len = (int)step + 1;

		slider->genCurve.clear();
		for (int i = 0; i < len; i++)
		{
			irr::core::vector2di xy = pointAt(i / step);
			slider->genCurve.push_back(irr::core::vector2di(xy.X, xy.Y));
		}
	}
}

Hitobject::Slider::CircumscribedCircle::~CircumscribedCircle(){}

/*void Hitobject::Slider::CircumscribedCircle::operator =(CircumscribedCircle circle)
{
	*this = circle;
}*/

bool Hitobject::Slider::CircumscribedCircle::isIn(double a, double b, double c)
{
	return (b > a && b < c) || (b < a && b > c);
}

irr::core::vector2di Hitobject::Slider::CircumscribedCircle::intersect(irr::core::vector2di a, irr::core::vector2di ta, irr::core::vector2di b, irr::core::vector2di tb)
{
	double des = tb.X * ta.Y - tb.Y * ta.X;
	if (abs(des) < 0.00001)
	{
		std::cout << "Vectors are parallel." << std::endl;
		return irr::core::vector2di(-1, -1);
	}
	double u = ((b.Y - a.Y) * ta.X + (a.X - b.X) * ta.Y) / des;
	b.X += tb.X * u;
	b.Y += tb.Y * u;
	return b;
}

irr::core::vector2di Hitobject::Slider::CircumscribedCircle::pointAt(double t)
{
	double ang = lerp(this->startAng, this->endAng, t);
	return irr::core::vector2di((cos(ang) * this->radius + this->circleCenter.X),
								(sin(ang) * this->radius + this->circleCenter.Y));
}