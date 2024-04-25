#include <iostream> 
#include "Line2D.h"

//using namespace std;

double Line2D::getLength(float startX, float startY, float endX, float endY)
{
	return sqrt((endX - startX) * (endX - startX) + (endY - startY) * (endY - startY));
}

Point2D Line2D::scale(Point2D startPnt, Point2D endPnt, float fraction)
{
	Point2D result;
	result.x = startPnt.x + (endPnt.x - startPnt.x) * fraction;
	result.y = startPnt.y + (endPnt.y - startPnt.y) * fraction;

	return result;
}

Point2D Line2D::getPerpendicular(Point2D startPnt, Point2D endPnt)
{
	float segLength = getLength(startPnt, endPnt);

	if (fabs(segLength) < 1e-8) // points are coincident
		return { -INFINITY, -INFINITY };
	else {
		Point2D result;

		result.x = startPnt.x - (endPnt.y - startPnt.y) / segLength; // note x's and y's
		result.y = startPnt.y + (endPnt.x - startPnt.x) / segLength;

		return result;
	}
}

float Line2D::getAngle(Point2D startPnt, Point2D endPnt)
{	
	// Note: 45 deg = PI/2 = atan(1.)  
	//     >>>  multiply by 45./atan(1.) to convert from radians to degrees
	float answer = atan2(endPnt.y - startPnt.y, endPnt.x - startPnt.x) * 45. / atan(1.); 
	
	return fmodf(answer + 360., 360.);  // fmodf and adding 360, makes answer between 0 and 360
}

Point2D Line2D::getIntersection(Point2D pnt1, Point2D pnt2, Point2D pnt3, Point2D pnt4)
{ // changed parameter names to better match given equation

	float denominator = (pnt1.x - pnt2.x) * (pnt3.y - pnt4.y) - (pnt1.y - pnt2.y) * (pnt3.x - pnt4.x);
	if (fabs(denominator) < 1e-8) // lines are parallel
		return { -INFINITY, -INFINITY };
	else {
		float topX = (pnt1.x * pnt2.y - pnt1.y * pnt2.x) * (pnt3.x - pnt4.x)
			- (pnt1.x - pnt2.x) * (pnt3.x * pnt4.y - pnt3.y * pnt4.x);

		float topY = (pnt1.x * pnt2.y - pnt1.y * pnt2.x) * (pnt3.y - pnt4.y)
			- (pnt1.y - pnt2.y) * (pnt3.x * pnt4.y - pnt3.y * pnt4.x);

		return { topX / denominator, topY / denominator };
	}
}

Point2D Line2D::getTrueIntersection(Point2D start1, Point2D end1, Point2D start2, Point2D end2)
{
	Point2D trialIntersect = getIntersection(start1, end1, start2, end2); // mathematical intersection
	if (trialIntersect.x != -INFINITY) { // lines not parallel
		// for true intersect, intersection point must line on both line segments
		// Here, we are being EXTRA strict. It may be possible to do a simpler check using the knowledge 
		//   trialIntersect is necessarily along the line for both segments. For example, need only check 
		//   that x-coord of trialIntersect is between the x-coords of start and end points of both segments
		if (!(isBetween(start1, trialIntersect, end1) && isBetween(start2, trialIntersect, end2)))
			trialIntersect = { -INFINITY, -INFINITY };
	}
	return trialIntersect;
}

bool Line2D::isBetween(Point2D startPoint, Point2D checkPoint, Point2D endPoint)
{
	float length = getLength(startPoint, endPoint);// *1e-5;
	float delta1 = getLength(startPoint, checkPoint);
	float delta2 = getLength(endPoint, checkPoint);
	return fabs(length - delta1 - delta2) < length * 1e-5;
}

float Line2D::angleBetween(Point2D start, Point2D end1, Point2D end2)
{
	// from dot product formula a . b = |a||b| cos(theta)
	// and a . b = ax*bx + ay*by
	// -> cosTheta = (ax * bx + ay * by ) / len(a) / len(b)
	// be careful that cosTheta not be less than -1 or greater than +1 before trying acos(function)
	float lengthA = getLength(start, end1);
	float lengthB = getLength(start, end2);

	if (fabs(lengthA) < 1e-8 || fabs(lengthB) < 1e-8)
		return 0;
	else {
		float cosTheta = ((end1.x - start.x) * (end2.x - start.x) + (end1.y - start.y) * (end2.y - start.y))
			/ lengthA / lengthB;
		if (cosTheta < -1) cosTheta = -1.f;
		if (cosTheta > 1) cosTheta = 1.f;

		return acos(cosTheta) * 45.f / atan(1.);
	}
}
