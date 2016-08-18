#ifndef SLIDERHITOBJECT_H
#define SLIDERHITOBJECT_H

#include "Hitobject.h"

// \TODO: Make all functions return safe time values (so that non are out of bounds if we were expecting slider)
class SliderHitObject : Hitobject
{
	friend class Hitobject;
	friend class Beatmap;
	friend class Play;

	public:
		SliderHitObject(SliderHitObject&&) = default;
		SliderHitObject(SliderHitObject const&) = default;
		SliderHitObject(std::vector<std::string> &_objectData, std::vector<std::string> &_sliderData);

		virtual ~SliderHitObject();

		void newSlider(bool isline, bool iscircle);

		double getY(int i);
		double getX(int i);

		irr::core::vector2d<double> GetSliderPos(double _time);
		int getTimeDistFrom(int _time, int _dist, bool _dir);  // 0 - back, 1 - forward

		std::vector<int> getTickTimes();

		double getLength();
		double getVelocity();
		int GetLastTickTime();
		int getEndTime();

		void RecordRepeatTimes();
		void RecordTickIntervals(int _beatmapTickInterval);

	protected:
		CurveType curveType;
		std::vector<irr::core::vector2di> curves; // Points along the curve
		std::vector<irr::core::vector2di> genCurve; // Points along the curve

		int ncurve; // number of calculated points
		int repeat; // number of repeats
		std::vector<int> repeatTimes; // the time of repeats
		double pixelLength;
		irr::core::vector2di endPoint; // calculated

		int endTime;	   // also for spinner
		int toRepeatTime;  // time to travel to reverse arrow
		std::vector<int> ticks;

		// Points generated along the curve should be spaced this far apart.
		const int CURVE_POINTS_SEPERATION = 5;

		// The scaled starting x, y coordinates.
		double x, y;

		// The scaled slider x, y coordinate lists.
		std::vector<double> sliderX, sliderY;

	private:
		void init(std::vector<Bezier> curvesList);
		void ProcessSliderData(std::vector<std::string> &_objectData, std::vector<std::string> &_sliderData);

		void MakeCircle();
		irr::core::vector2di intersect(irr::core::vector2di a, irr::core::vector2di ta, irr::core::vector2di b, irr::core::vector2di tb);
		irr::core::vector2di pointAt(double t);

		const double TWO_PI = (double)(M_PI * 2.),
			HALF_PI = (double)(M_PI / 2.);

		// The center of the Circumscribed Circle. 
		irr::core::vector2di circleCenter;

		// The radius of the Circumscribed Circle. 
		double radius;

		// The three points to create the Circumscribed Circle from.
		irr::core::vector2di start, mid, end;

		// The three angles relative to the circle center.
		double startAng, endAng, midAng;
};

#endif // !SLIDER_H