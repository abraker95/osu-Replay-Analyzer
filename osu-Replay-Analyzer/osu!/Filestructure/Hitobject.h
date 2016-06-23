#ifndef HITOBJECT_H
#define HITOBJECT_H

#include <vector>
#include <memory>

#include "../../irrlicht/include/vector2d.h"

#include "../../utils/mathUtils.h"
#include "../osuEnums.h"

#include "Bezier.h"

class Beatmap;

enum CurveType
{
	PerfectCurve = 'P',
	BezierCurve  = 'B',
	LinearCurve  = 'L',
	CatmullCurve = 'C'
};


class Hitobject
{
	friend class Play;

	public:
		Hitobject(std::vector<std::string> &_objectData, std::vector<std::string> &_sliderData);
		Hitobject();
		virtual ~Hitobject();

		bool IsHitObjectType(int type);
		int getHitobjectType();

		irr::core::vector2d<double> getPos() const;
		void setPos(irr::core::vector2d<double> _pos);

		int getTime() const;
		int getEndTime() const;
		void setTime(Beatmap* _beamap, int _time);

		std::pair<int, int> getVisiblityTimes(double _AR, bool _hidden, double _opacityStart, double _opacityEnd);
		bool isVisibleAt(int _time, double _AR, bool _hidden);
		double getOpacityAt(int _time, double _AR, bool _hidden);

		double GetCircleOverlapPercentage(int CS, Hitobject &c1);
	
		// \TODO: Make all functions return safe time values (so that non are out of bounds if we were expecting slider)
		class Slider
		{
			friend class Hitobject;
			friend class CircumscribedCircle;
			friend class Beatmap;
			friend class Play;

			public:
				Slider(Hitobject* hitobject);
				virtual ~Slider();

				void newSlider(Hitobject* object, bool isline, bool iscircle);

				double getEndAngle();
				double getStartAngle();

				double getY(int i);
				double getX(int i);

				irr::core::vector2di GetSliderPos(int _time);
				int getTimeDistFrom(int _time, int _dist, bool _dir);  // 0 - back, 1 - forward

				double getLength();
				int GetLastTickTime();
				int getEndTime();
				double getVelocity();

				void RecordRepeatTimes();
				void RecordTickIntervals(int _beatmapTickInterval);

				class CircumscribedCircle
				{
						// Constructor.
						// @param hitObject the associated HitObject
						// @param color the color of this curve
					public:
						CircumscribedCircle(Slider* slider);
						virtual ~CircumscribedCircle();

						//void operator = (CircumscribedCircle circle);

						// Checks to see if "b" is between "a" and "c"
						// @return true if b is between a and c
						//
						bool isIn(double a, double b, double c);

						// Finds the point of intersection between the two parametric lines
						// {@code A = a + ta*t} and {@code B = b + tb*u}.
						// http://gamedev.stackexchange.com/questions/44720/
						// @param a  the initial position of the line A
						// @param ta the direction of the line A
						// @param b  the initial position of the line B
						// @param tb the direction of the line B
						// @return the point at which the two lines intersect
						irr::core::vector2di intersect(irr::core::vector2di a, irr::core::vector2di ta, irr::core::vector2di b, irr::core::vector2di tb);

						irr::core::vector2di pointAt(double t);

						Slider* slider;

					private:
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
			
				std::unique_ptr <CircumscribedCircle> circle;

			protected:
				Hitobject* hitobject;

				CurveType curveType;
				std::vector<irr::core::vector2di> curve; // Points along the curve
				std::vector<irr::core::vector2di> genCurve; // Points along the curve
				
				int ncurve; // number of calculated points
				int repeat; // number of repeats
				std::vector<int> repeatTimes; // the time of repeats
				double pixelLength;
				irr::core::vector2di endPoint; // calculated

				int endTime;	   // also for spinner
				int toRepeatTime;  // time to travel to reverse arrow
				std::vector<int> ticks;

				/** Points generated along the curve should be spaced this far apart. */
				const int CURVE_POINTS_SEPERATION = 5;

				/** The angles of the first and last control points for drawing. */
				double startAngle, endAngle;

				/** The scaled starting x, y coordinates. */
				double x, y;

				/** The scaled slider x, y coordinate lists. */
				std::vector<double> sliderX, sliderY;

			private:
				void init(std::vector<Bezier> curvesList);
		};
		
		std::unique_ptr<Slider> slider;


	private:
		int time, index;
		irr::core::vector2d<double> pos;
		int type;
		Beatmap* beatmap;

		void ProcessHitobjectData(std::vector<std::string> &_objectData);
		void ProcessSliderData(std::vector<std::string> &_objectData, std::vector<std::string> &_sliderData);
};

#endif