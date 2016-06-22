#ifndef _bezier_h_
#define _bezier_h_

#include <vector>
#include <math.h>

#include "../../irrlicht\include\vector2d.h"
#include "../../utils/mathUtils.h"


/**
* Representation of a Bezier curve with the distance between each point calculated.
*
* @author fluddokt (https://github.com/fluddokt)
  @porter abraker
*/

class Bezier
{
	public:
		// Constructor
		// @param points the control points*/
		Bezier(std::vector<irr::core::vector2di> _points);

		void init(double approxlength);

		irr::core::vector2di pointAt(double _t);

		// Returns the points along the curve of the Bezier curve.
		std::vector<irr::core::vector2di> getCurvePoint();

		// Returns the distances between a point of the curve and the last point.
		std::vector<double> getCurveDistances();

		// Returns the number of points along the curve.
		int getCurvesCount();

		// Returns the total distances of this Bezier curve.
		double TotalDistance();


	private:
		// The control points of the Bezier curve
		std::vector<irr::core::vector2di> points;

		// Points along the curve of the Bezier curve
		std::vector<irr::core::vector2di> curvePoints;

		// Distances between a point of the curve and the last point
		std::vector<double> curveDis;

		// The number of points along the curve
		int ncurve;

		// The total distances of this Bezier
		double totalDistance;
};

#endif