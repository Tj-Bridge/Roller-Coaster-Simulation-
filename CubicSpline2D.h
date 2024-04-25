/*
									Nestor Gomez
									Carnegie Mellon University
				_ _ . 	_			C++ for Engrs, 24-280
			  .'	 `./  \			PS06.  Due Tues. Apr. 2, 2024
			 /		  /`   |
			/        |  \._/		Class for a smooth path
  ._ _ _ _./          \				
									
*/

#include "Path2D.h"

class CubicSpline2D : public Path2D {  // inherit from Path2D
protected: 
	std::vector<Point2D> theSplinePoints; // needed to get specific coords (not just display)

	double splineTension = 0.5;
	std::vector<float> subLengths;  // used to quickly get indexFromLength

public:
	CubicSpline2D() {};

	CubicSpline2D(std::ifstream& input) : Path2D(input) {
		recalcPath();  // run the local one (to get all the spline points)

		// since recalcPath() is virtual, the local one will be the function called
		// anywhere (even within Path2D), except in the constructor of Path2D
	}

	virtual void paint(Graphics^ g, Color c = Color::CornflowerBlue,
		bool showPoints = false, float width = 0.f, bool showLabels = false, bool fill = false);

	// over-ride, now using theSplinePoints instead of thePoints
	Point2D coordsFromLength(float givenLength);

	// over-ride, now using theSplinePoints instead of thePoints
	float angleFromLength(float givenLength);

	// with a given length, returns the coordinates of a back point which lies on the path,
	// such that the distance from back point to forward point (as calculated from givenLength)
	// is exactly spanLength (uses law of sines)
	virtual Point2D coordsBackPointFromLength(float givenLength, float spanLength) {
		return coordsBackPointAux(givenLength, spanLength, theSplinePoints);
	};

	// with a given length, returns the index (first point's index is 1) 
	// of the point just after given length
	virtual	int indexFromLength(float givenLength);

public:
	virtual float lengthFromCoords(Point2D givenPoint, float minLength = 0) {
		return lengthFromCoordsAux(givenPoint, minLength, theSplinePoints);
	}

protected:
	void recalcPath();   // will not work if recalcPath is private!!!
	void generateEdgePoints(int edgeIndex);

protected:
	virtual float lengthToPoint(int index);

};