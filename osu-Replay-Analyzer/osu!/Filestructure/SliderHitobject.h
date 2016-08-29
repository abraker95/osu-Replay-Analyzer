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
		enum SLIDERTYPE
		{
			LINEAR,
			BEZIER,
			CIRCUMSCRIBED
		};

		SliderHitObject(SliderHitObject&&) = default;
		SliderHitObject(SliderHitObject const&) = default;
		SliderHitObject(std::vector<std::string> &_objectData, std::vector<std::string> &_sliderData);

		virtual ~SliderHitObject();

		void GenerateSlider(SLIDERTYPE _sliderType);

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
		std::vector<irr::core::vector2di> readCurvePoints; // Points along the curve read from the .osu file
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

	private:
		void init(std::vector<Bezier> curvesList);
		void PreprocessSliderData(std::vector<std::string> &_objectData, std::vector<std::string> &_sliderData);

		irr::core::vector2di getReadPoint(int _i);

		void MakeCircle();
		void MakeLinear();
		void MakeBezier();

		irr::core::vector2di intersect(irr::core::vector2di a, irr::core::vector2di ta, irr::core::vector2di b, irr::core::vector2di tb);
		irr::core::vector2di pointAt(double t);

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