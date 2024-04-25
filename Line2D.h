/*
									Nestor Gomez
									Carnegie Mellon University
				_ _ . 	_			C++ for Engrs, 24-280
			  .'	 `./  \			PS03.  Due Mon. Feb. 19, 2024
			 /		  /`   |
			/        |  \._/		Auxiliary class for reasoning 
  ._ _ _ _./          \				about line geometry in 2D
									(Line2D)

*/
#pragma once


// #include <iostream>  // don't need it

struct Point2D {
	float x;
	float y;
};

class Line2D {
public:
	// returns the distance from one point to the other.
	static double getLength(Point2D startPoint, Point2D endPoint) {
		return getLength(startPoint.x, startPoint.y, endPoint.x, endPoint.y);
	};

	// returns the distance from one point to the other.
	static double getLength(float startX, float startY, float endX, float endY);

	// returns a point that is scaled on the line segment defined by startPnt 
	// and endPnt. If fraction is greater than 1.0, the return point is extrapolated 
	// beyond the end point. If fraction is less than zero, the return point is
	// extrapolated “before” start point.
	static Point2D scale(Point2D startPnt, Point2D endPnt, float fraction);

	// returns a point such that a segment from startPnt to the result is perpendicular to the segment 
	// and has a unit length (much easier than you think, no need for sine and cosine)
	static Point2D getPerpendicular(Point2D startPnt, Point2D endPnt);

	// returns the angle of the given line segment, from startPnt to endPnt. The angle returned is in 
	// degrees (0-360), measured CCW from zero (the X-axis). Makes use of atan2() function.
	static float getAngle(Point2D startPnt, Point2D endPnt);

	// returns the intersection of the two lines defined by start and end points given. If there is no intersection 
	// (i.e., the lines are parallel), the coordinates of the results are set to (-INFINITY, -INFINITY). 
	// Will not work if using slope-intercept (y = mx + b) to define lines.
	static Point2D getIntersection(Point2D start1, Point2D end1, Point2D start2, Point2D end2);

	// returns the intersection of the two line segments defined by start and end points given. 
	// The true intersection differs from the mathematical intersection in that the two line segments 
	// (not just their infinite linear extensions) must cross. If there is no true intersection 
	// (i.e., the lines are parallel or the segments don’t cross), the function returns (-INFINITY, -INFINITY).
	static Point2D getTrueIntersection(Point2D start1, Point2D end1, Point2D start2, Point2D end2);

	// returns true if checkpoint is perfectly between startPoint and endpoint. 
	// Uses a tolerance based on distance from startPoint to endPoint.
	static bool isBetween(Point2D startPoint, Point2D checkPoint, Point2D endPoint);

	// returns the angle (in degrees) made by two vectors, sharing a start point
	// and ending at end1 and end2. Note that order of vectors does not matter
	// (i.e., result is always between 0 and 180)
	static float angleBetween(Point2D start, Point2D end1, Point2D end2);
	


};