// Circle.h: interface for the Circle class.
// Circle class.
// Purpose : Represent the circle object
// Input : 3 different points
// Process : Calcuate the radius and center
// Output : Circle
//           
// This class originally designed for representation of discretized curvature information 
// of sequential pointlist  
// KJIST CAD/CAM     Ryu, Jae Hun ( ryu@geguri.kjist.ac.kr)
// Last update : 1999. 7. 4
#if !defined(AFX_CIRCLE_H__1EC15131_4038_11D3_8404_00C04FCC7989__INCLUDED_)
#define AFX_CIRCLE_H__1EC15131_4038_11D3_8404_00C04FCC7989__INCLUDED_

#include "Point.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class Circle  
{
public:
	double GetRadius();
	Point* GetCenter();
	Circle(Point *p1, Point *p2, Point *p3);	// p1, p2, p3 are co-planar
	Circle();
	virtual ~Circle();

private:
	double CalcCircle(Point *pt1, Point *pt2, Point *pt3);
	bool IsPerpendicular(Point *pt1, Point *pt2, Point *pt3);
	double m_dRadius;
	Point m_Center;
};

#endif // !defined(AFX_CIRCLE_H__1EC15131_4038_11D3_8404_00C04FCC7989__INCLUDED_)
