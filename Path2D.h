/*
									Nestor Gomez
									Carnegie Mellon University
				_ _ . 	_			C++ for Engrs, 24-280
			  .'	 `./  \			PS06.  Due Tues. Apr. 2, 2024
			 /		  /`   |
			/        |  \._/		Class for maintaining a path in
  ._ _ _ _./          \				2 dimensions, with input/output
									and graphical computation.
									Now with slight changes for inheritance
*/
#pragma once

#include <vector>
#include <iostream>
#include "Line2D.h"

//using namespace std; // better to leave it out of .h if possible
//using namespace System;
using namespace System::Drawing;

class Path2D {
protected:
	std::vector<Point2D> thePoints;
	float currLength;
	float minX, minY, maxX, maxY;


public:
	// default constructor for the class. Initializes member variables only.
	Path2D();

	// additional constructor for the class. Takes a pre-created ifstream and uses it to read coordinate information
	// for the path.Note that the method’s parameter cannot be a string filename because the file may
	// 	already be in the process of being read.The constructor needs to instantiate any constituent objects
	// 	and /or data structures.
	Path2D(std::ifstream& input);

	// adds a coordinate point such that the new point becomes the index-th point . For example, an index value of
	// 3 will insert a point between the existing 2nd and 3rd points such that the new point becomes the new
	// 	3rd point. An index of 1 will insert the new point at the start of the path and an index greater than the
	// 	number of points will insert the new point as the last point.Function returns false only if the new point
	// 	cannot be added for any reason.
	bool addPoint(Point2D newPoint, int index);

	// creates a coordinate point with the given coordinates and inserts it into the path such that the new point
	// becomes the index - th point. Otherwise similar to above.
	bool addPoint(float newX, float newY, int index) {
		return addPoint({ newX, newY }, index);
	};


	// creates a coordinate point between vertices index-1 and index and inserts it into the line segment at the given
	// ratio.The ratio is a number greater than zero and less than 1. Note that, like above, the new point
	// 	becomes the index-th point.Returns false if it cannot insert point (e.g., index is too large or too small,
	// 	ratio value is inappropriate).[A ratio close to zero will insert the new vertex near index - 1; a ratio of
	// 	0.5 will insert the new vertex at the middle of the segment;and a ratio close to 1 will insert the new
	// 	vertex near old index.]

	bool addPoint(int index, float ratio);

	// removes the indicated point from the path. Function returns false only if the point cannot be removed for any
	// reason. First vertex has index of 1
	bool removePoint(int index);

	// removes all points from the path. Function returns false only if the points cannot be removed for any reason.
	bool removeAllPoints() {
		thePoints.clear();
		recalcPath(); // could have just set currLength to zero, but the function may grow later.
		return true;
	}

	// returns the number of points in the path.
	int numbPoints() { return thePoints.size(); };

	// returns the length of the path.
	float length() { return currLength; };

	// outputs all the coordinates of the path to the console
	void print();

	// also used to output all the coordinates of a path, but can do it to console or to a filestream. This can be tricky,
	// so we’ll discuss in lecture.
	friend std::ostream& operator<<(std::ostream& os, const Path2D& aPath);

	//PS03 stuff

	// get coordinates of a point. The first point has index 1. Returns false if index is invalid.
	Point2D getPoint(int index);

	// get index of the first point on the path that is nearer than nearDistance to the given checkPoint
	// The first point has index 1. Returns -1 if no point is near checkPoint.
	int getIndex(Point2D checkPoint, float nearDistance = 0.1);

	// moves a point to new coordinates. The first point has index 1. Returns false if index is invalid.
	bool movePoint(Point2D newCoords, int index);

	// changes the coordinates of a point by the given delta. The first point has index 1.
	// Returns false if index is invalid
	bool movePointDelta(Point2D deltaCoords, int index);

	// returns the coordinates of the point on the path (not necessarily
	// a defining point on the path) that is nearest givenPnt
	Point2D getNearest(Point2D givenPnt);

	// return a point corresponding to the upper right corner of a bounding box for the path
	// (maximum X and maximum Y value of all points).
	Point2D upperBoundingBox() { return { maxX, maxY }; }

	// return a point corresponding to the lower left corner of a bounding box for the path
	// (minimum X andminimum Y value of all points).
	Point2D lowerBoundingBox() { return { minX, minY }; }

	// returns the coordinates of center of the bounding rectangle that encompasses the path.
	// Returns (-INFINITY, -INFINITY) if the path has no points.
	Point2D center() { return { (minX + maxX) / 2.f, (minY + maxY) / 2.f }; }

	// determines the coordinate of a point along the path that is givenLength from the starting point. Returns
	// {-INFINITY, -INFINITY} if givenLength is greater than the length of the path.
	virtual Point2D coordsFromLength(float givenLength);

	// with a given length, returns the coordinates of a back point which lies on the path,
	// such that the distance from back point to forward point (as calculated from givenLength)
	// is exactly spanLength (uses law of sines)
	virtual Point2D coordsBackPointFromLength(float givenLength, float spanLength) {
		return coordsBackPointAux(givenLength, spanLength, thePoints);
	};

	// determines the angle (0-360 deg) of the path at a point along the path that is givenLength from the
	// starting point. Returns{-INFINITY, -INFINITY} if givenLength is invalid.
	virtual float angleFromLength(float givenLength);

	// generates the geometry of the path on the given graphic space, with given color and other parameters
	// (many of which have default values for now). Also has ability to selectively show labels
	virtual void paint(Graphics^ g, Color c = Color::CornflowerBlue,
		bool showPoints = false, float width = 0.f, bool showLabels = false, bool fill = false);

public:
	// returns the length along path that corresponds to the given point
	// returns -1 if givenPoint is not on path
	// optional parameter minLength indicates that we can skip to that length 
	// before searching (possibly saving some processing time)
	virtual float lengthFromCoords(Point2D givenPoint, float minLength = 0) {
		return lengthFromCoordsAux(givenPoint, minLength, thePoints);
	}

	// moves all points by given deltaX and deltaY
	void moveAllPoints(float deltaX, float deltaY);

	// moves all points by given deltaX and deltaY
	void moveAllPoints(Point2D delta) {
		moveAllPoints(delta.x, delta.y);
	};

	// changes coordinates of each of the guidepoints by scaling from reference point
	// by given scales in X and Y
	void scale(Point2D refPnt, float scaleX, float scaleY);

	// changes coordinates of each of the path points by scaling from reference point 
	// by given scale (both x and y)
	void scale(Point2D refPnt, float scaleXY) {
		scale(refPnt, scaleXY, scaleXY);
	};

	// changes coordinates of each of the guidepoints by rotating about reference point
	// by given angle (in degrees). 
	void rotate(Point2D refPnt, float angleDeg);

	// changes coordinates of each of the guidepoints by mirroring each point about the
	// given line segment defined from start point to end point. 
	// Hint: not as difficult as it appears
	void mirror(Point2D startPnt, Point2D endPnt);


protected:
	// calculate the length of the whole path and bounding box
	virtual void recalcPath();

	void paintPoint(Graphics^ g, Color c, bool showLabel = true);

	// with a given length, returns the index (first point's index is 1) 
	// of the point just after given length
	virtual int indexFromLength(float givenLength);

	Point2D coordsBackPointAux(float givenLength, float spanLength, std::vector<Point2D>& pointList);

protected:
	float lengthFromCoordsAux(Point2D givenPoint, float minLength, std::vector<Point2D>& pointList);

	virtual float lengthToPoint(int index);
};
