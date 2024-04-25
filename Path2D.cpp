#include <fstream>
#include <sstream>
#include "ColorConverter.h"
#include "Path2D.h"

using namespace std;

void Path2D::paint(Graphics^ g, Color c, bool showPoints, float width, 
	bool showLabels, bool fill)
{
	if (thePoints.size() > 1) {

		float pntSize = width * 3.;
		if (width == 0)
			pntSize = 0.15 * currLength / thePoints.size();  // 15% of average segment length

		auto aPen = gcnew Pen(c, width);

		// Transparency can be used by adding an "alpha" value to the RGB value 
		auto aBrush = gcnew SolidBrush(Color::FromArgb(100, c));

		auto aFont = gcnew System::Drawing::Font(L"Courier", pntSize);

		Point2D prevPnt = thePoints[0];

		//for (int i = 0; i < thePoints.size(); i++) { // starting at i=0 draws extra zero-length line
		//	g->DrawLine(aPen, prevPnt.x, prevPnt.y, thePoints[i].x, thePoints[i].y);
		//	prevPnt = thePoints[i];

		//	if (showPoints)
		//		g->FillEllipse(aBrush, thePoints[i].x - pntSize / 2.f, thePoints[i].y - pntSize / 2.f, pntSize, pntSize);
		//}


		// for "smooth" line
		auto aGPath = gcnew System::Drawing::Drawing2D::GraphicsPath();
		aGPath->StartFigure();
		prevPnt = thePoints[0];
		for (int i = 1; i < thePoints.size(); i++) {
			aGPath->AddLine(prevPnt.x, prevPnt.y, thePoints[i].x, thePoints[i].y);
			prevPnt = thePoints[i];
		}

		aPen->LineJoin = System::Drawing::Drawing2D::LineJoin::Round; // there are other join types
		g->DrawPath(aPen, aGPath);

		// paint points and labels so they appear "on top of" path lines (no need for loop here)
		if (showPoints || showLabels) {
			paintPoint(g, c, showLabels);
		}
	}

}

void Path2D::paintPoint(Graphics^ g, Color c, bool showLabel)
{
	Pen^ pointPen = gcnew Pen(Color::Black, 0);

	// based on zoom scale (point gets bigger when you zoom in)
	//float pointDiam = max((maxX - minX) / 40.f, (maxY - minY) / 40.f);
	//pointDiam = max(pointDiam, 0.5f);  // don't want it too small ?

	// always same number of pixels (point stays the same size when you zoom in)
	float currScale = g->Transform->Elements[0]; // gets the scale from the transform itself
	int pointPixels = 15; // change as needed
	float pointDiam = pointPixels / currScale;

	for (int i = 0; i < thePoints.size(); i++) {
		// translate to point itself 
		// (this will make it easier later if I want a triangle or tilted square or whatever)
		g->TranslateTransform(thePoints[i].x, thePoints[i].y);

		// draw circle
		//g->DrawEllipse(pointPen, -pointDiam / 2., -pointDiam / 2., pointDiam, pointDiam);

		// draw tilted square
		g->RotateTransform(45.);
		g->DrawRectangle(pointPen, -pointDiam / 2., -pointDiam / 2., pointDiam, pointDiam);
		g->RotateTransform(-45.);

		if (showLabel) {
			// drawing the index number on the point
			System::Drawing::Font^ drawFont = gcnew System::Drawing::Font("Arial", pointDiam);
			SolidBrush^ drawBrush = gcnew SolidBrush(Color::Black);

			// need to scale so that y-axis is down (otherwise text will be upside down)
			g->ScaleTransform(1., -1.);

			// Create point for upper-left corner of text.
			PointF drawPoint = PointF(pointDiam * 0.5, pointDiam * 0.5);

			// Draw string to screen.
			g->DrawString((i + 1).ToString(), drawFont, drawBrush, drawPoint);

			// scale back to our normal y-axis is up
			g->ScaleTransform(1., -1.);
		}
		// translate back to our normal drawing plane
		g->TranslateTransform(-thePoints[i].x, -thePoints[i].y);

	}

}

int Path2D::indexFromLength(float givenLength)
{
	if (thePoints.size() <= 1 || givenLength < 0 || givenLength >= currLength)
		return  -1;
	else if (givenLength == 0) // requesting start of path
		return 1;
	else {
		// actually look for point along path
		int currPointIndex;

		// serial search: O(N)
		// once loop exits, currPointIndex is the point where lengthToForwardPoint is 
		// just bigger than givenLength. 

		currPointIndex = 0;
		float lengthToForwardPoint = 0.f;
		while (currPointIndex < thePoints.size() && lengthToForwardPoint < givenLength) {
			currPointIndex++;
			lengthToForwardPoint += Line2D::getLength(thePoints[currPointIndex],
				thePoints[currPointIndex - 1]);
		}

		return currPointIndex + 1;
	}

}

Point2D Path2D::coordsBackPointAux(float givenLength, float spanLength,
	std::vector<Point2D>& pointList)
{
	// note: pointList is a reference to the vector that holds the points that
	// define the actual path (sometimes a basic Path2D, sometimes a CubicSpine2D)

	// quick check to see that the request is valid
	if (givenLength < 0 || givenLength > currLength || pointList.size() < 2)
		return { -INFINITY, -INFINITY };
	else if (givenLength == 0) {
		// frontPoint is right at the start of path so no need for any more information
		// return a point as back extension of first path segment
		float ratio = -spanLength / Line2D::getLength(pointList[0], pointList[1]);
		return Line2D::scale(pointList[0], pointList[1], ratio);
	}
	else {
		auto forwardPoint = coordsFromLength(givenLength);

		// just something so I can test the paint function
		// doesn't even rotate the car
		//return { forwardPoint.x - spanLength, forwardPoint.y };

		// a little better approximation but not quite what we need
		// rotates car based on slope of track at front wheel
		//float frontAngle = angleFromLength(givenLength);
		//if (frontAngle > -INFINITY)
		//	return { forwardPoint.x - spanLength * (float)cos(frontAngle / 45. * atan(1.)),
		//	 forwardPoint.y - spanLength * (float)sin(frontAngle / 45. * atan(1.)) };
		//else
		//	return { -INFINITY, -INFINITY };

		// ===============================================
		// now for the real way
		// note that since I am using the vector directly, indexing is from zero (not 1)

		// -1 to account for indexing from 0 in this function
		// and then -1 again to go back one point
		int prevPointIndex = indexFromLength(givenLength) - 2;

		// if we are close to beginning and backPoint is off the track
		// extend first segment backwards to get location of backPoint
		if (prevPointIndex == 0 && spanLength >= givenLength) {
			float ratio = -(spanLength - givenLength) / Line2D::getLength(pointList[0], pointList[1]);
			return Line2D::scale(pointList[0], pointList[1], ratio);
		}
		// for all other cases, find coordinates of backPoint
		else {
			Point2D prevPathPoint = pointList[prevPointIndex];
			float lastLength = Line2D::getLength(prevPathPoint, forwardPoint);

			// if backPoint is on same edge as forwardPoint, interpolate and done
			if (lastLength >= spanLength)
				return Line2D::scale(forwardPoint, prevPathPoint, spanLength / lastLength);
			else {
				// need to search backwards along path points until I find a path point
				// that is outside the "circle"
				Point2D currPathPoint = prevPathPoint;
				prevPointIndex--;
				prevPathPoint = pointList[prevPointIndex];
				while (prevPointIndex > 0 && Line2D::getLength(prevPathPoint, forwardPoint) < spanLength) {
					currPathPoint = prevPathPoint;
					prevPointIndex--;
					prevPathPoint = pointList[prevPointIndex];
				}
				// when I come out of loop, I have the 3 points I need 
				// unless I came out of loop due to first condition
				if (prevPointIndex == 0)
					prevPathPoint = Line2D::scale(pointList[0], pointList[1], -1000);

				// get the angle between two vectors
				// vector1 goes from currPathPoint to forwardPoint  (currToForward)
				// vector2 goes from currPathPoint to prevPathPoint (currToBack)
				float angleOppSpan = Line2D::angleBetween(currPathPoint, forwardPoint, prevPathPoint);
				float currToForwardLength = Line2D::getLength(currPathPoint, forwardPoint);
				float currToBackLength;

				if (fabsf(angleOppSpan - 180) < 1e-8)
					// will not able able to divide by sine of angle
					// segments are "in-line" so it's trivial to get currToBackLength
					currToBackLength = spanLength - currToForwardLength;
				else {
					// find currToBackLength using law of sines
					float sinOfAngleOppSpan = sin(angleOppSpan * atan(1.) / 45.);
					float sinOfAngleOppCurrToForward = sinOfAngleOppSpan
						/ spanLength * currToForwardLength;
					float angleOppCurrToForward = asin(sinOfAngleOppCurrToForward) * 45. / atan(1.);
					float angleOppCurrToBack = 180. - angleOppSpan - angleOppCurrToForward;
					float sinOfAngleOppCurrToBack = sin(angleOppCurrToBack * atan(1.) / 45.);

					// actually use law of sines again.
					currToBackLength = spanLength * sinOfAngleOppCurrToBack / sinOfAngleOppSpan;;
				}

				float backRatio = currToBackLength / Line2D::getLength(currPathPoint, prevPathPoint);
				return Line2D::scale(currPathPoint, prevPathPoint, backRatio);

			}
		}
	}
}

float Path2D::lengthFromCoordsAux(Point2D givenPoint, float minLength, std::vector<Point2D>& pointList)
{
	// now that I can easily have hundreds of points, it makes sense to make this run
	// more efficiently
	int currPointIndex;
	if (pointList.size() > 1 && minX <= givenPoint.x && givenPoint.x <= maxX
		&& minY <= givenPoint.y && givenPoint.y <= maxY) {

		// find the two path points such that givenPoint is between them
		minLength = max(0.f, minLength);

		// length to path point that is definitely behind givenPoint
		float lengthToBackPoint;

		// currPointIndex is the start-from-zero index of the point just past minlength
		currPointIndex = indexFromLength(minLength) - 1;

		if (currPointIndex < 0)
			return -1;
		else if (currPointIndex == 0) { // handle special case of minLength putting us at start of path
			currPointIndex++;
			lengthToBackPoint = 0.f;
		}
		else
			lengthToBackPoint = lengthToPoint(currPointIndex); // this is what we want

		// walk through path until we find givenPoint
		for (/*currPointIndex++*/; currPointIndex < pointList.size(); currPointIndex++) {

			if (Line2D::isBetween(pointList[currPointIndex], givenPoint,
				pointList[currPointIndex - 1])) {
				// found a good path edge
				return lengthToBackPoint + Line2D::getLength(pointList[currPointIndex - 1], givenPoint);
			}
			else
				lengthToBackPoint += Line2D::getLength(pointList[currPointIndex],
					pointList[currPointIndex - 1]);
		}
	}

	return -1; // this means givenPoint was NOT along path

}

float Path2D::lengthToPoint(int index)
{
	if (0 < index && index <= thePoints.size()) {
		// since we did not precalculate subLengths, we must run loop
		float subLength = 0.f;

		for (int i = 1; i < index; i++) {
			subLength += Line2D::getLength(thePoints[i - 1], thePoints[i]);
		}
		return subLength;
	}
	else
		return -1;
}

void Path2D::recalcPath()
{
	// go through all points in the path
	// sum up the lengths of each "segment" and check for max and min
	currLength = 0.f;
	if (thePoints.size() > 0) {
		minX = maxX = thePoints[0].x;
		minY = maxY = thePoints[0].y;

		for (int i = 1; i < thePoints.size(); i++) {
			currLength += Line2D::getLength(thePoints[i - 1], thePoints[i]);

			if (thePoints[i].x < minX)
				minX = thePoints[i].x;
			if (thePoints[i].x > maxX)
				maxX = thePoints[i].x;
			if (thePoints[i].y < minY)
				minY = thePoints[i].y;
			if (thePoints[i].y > maxY)
				maxY = thePoints[i].y;
		}
	}
}

Point2D Path2D::coordsFromLength(float givenLength)
{
	// if there are no points or givenLength is bad, return error signal
	if (thePoints.size() == 0 || givenLength < 0 || givenLength > currLength)
		return { -INFINITY, -INFINITY };
	else if (givenLength == 0) // requesting start of path, so just give it
		return thePoints[0];
	else if (givenLength == currLength) // requesting end of path, so just give it
		return thePoints[thePoints.size() - 1];
	else {
		// actually look for point along path
		int currPoint = 0;
		float lengthToBackPoint = 0.f, lengthToForwardPoint = 0.f;

		while (currPoint < thePoints.size() && lengthToForwardPoint < givenLength) {
			lengthToBackPoint = lengthToForwardPoint; // keep track of prev calculation instead of redoing it
			currPoint++;
			lengthToForwardPoint += Line2D::getLength(thePoints[currPoint],
				thePoints[currPoint - 1]);
		}

		// once loop exits, currPoint is the point where lengthToForwardPoint is 
		// just bigger than givenLength. 
		//    i.e., givenLegth is between lengthToBackPoint and lengthToForwardPoint  
		// time to interpolate using Line2D::scale
		float interpolationRatio = (givenLength - lengthToBackPoint)
			/ (lengthToForwardPoint - lengthToBackPoint);
		return Line2D::scale(thePoints[currPoint - 1], thePoints[currPoint], interpolationRatio);
	}
}

float Path2D::angleFromLength(float givenLength)
{
	// if there are not enough points or givenLength is bad, return error signal
	if (thePoints.size() < 2 || givenLength < 0 || givenLength > currLength)
		return -INFINITY;
	else if (givenLength == currLength) // requesting end of path, 
		// reference previous point for angle (unlike others)
		return Line2D::getAngle(thePoints[thePoints.size() - 2], thePoints[thePoints.size() - 1]);
	else {
		// actually walk along path
		int currPoint = 0;
		float lengthToBackPoint = 0.f, lengthToForwardPoint = 0.f;

		while (currPoint < thePoints.size() && lengthToForwardPoint <= givenLength) {
			lengthToBackPoint = lengthToForwardPoint; // keep track of prev calculation instead of redoing it
			currPoint++;
			lengthToForwardPoint += Line2D::getLength(thePoints[currPoint],
				thePoints[currPoint - 1]);
		}

		// once loop exits, currPoint is the point where lengthToForwardPoint is 
		// just bigger than givenLength. 
		//    i.e., givenLegth is between lengthToBackPoint and lengthToForwardPoint  
		// time to get angle using Line2D::getAngle
		return Line2D::getAngle(thePoints[currPoint - 1], thePoints[currPoint]);
	}
}


void Path2D::print()
{
	//for (int i = 0; i < thePoints.size(); i++) {
	//	cout << thePoints[i].x << "\t" << thePoints[i].y << endl;
	//}

	// since we now have a stream output function, just use it to print to console
	std::cout << *this;  // "*this" is a reference of the current object

}

Point2D Path2D::getPoint(int index)
{
	if (index <= 0 || index > thePoints.size())
		return { -INFINITY, -INFINITY };
	else {
		return thePoints[index - 1];
	}
}

int Path2D::getIndex(Point2D checkPoint, float nearDistance)
{
	// first do a quick check to see if checkPoint is in ballpark
	if (checkPoint.x < minX - nearDistance || checkPoint.x > maxX + nearDistance
		|| checkPoint.y < minY - nearDistance || checkPoint.y > maxY + nearDistance)
		return -1;
	else {
		// now I really have to search for something
		bool foundIt = false;
		int currIndex = 0;
		while (!foundIt && currIndex < thePoints.size()) {
			foundIt = (checkPoint.x > thePoints[currIndex].x - nearDistance && checkPoint.x < thePoints[currIndex].x + nearDistance
				&& checkPoint.y > thePoints[currIndex].y - nearDistance && checkPoint.y < thePoints[currIndex].y + nearDistance);
			currIndex++;
		}

		if (foundIt)
			return currIndex; // note that the "+1" is included above
		else
			return -1;
	}

}

bool Path2D::movePoint(Point2D newCoords, int index)
{
	if (index <= 0 || index > thePoints.size())
		return false;
	else {
		thePoints[index - 1] = newCoords;

		recalcPath();
		return true;
	}
}

bool Path2D::movePointDelta(Point2D deltaCoords, int index)
{
	if (index <= 0 || index > thePoints.size())
		return false;
	else {
		thePoints[index - 1].x += deltaCoords.x;
		thePoints[index - 1].y += deltaCoords.y;
		recalcPath();
		return true;
	}
}

Path2D::Path2D()
{
	currLength = minX = minY = maxX = maxY = 0.f;
}

Path2D::Path2D(std::ifstream& input)
{
	float xInput, yInput;
	float prevX = -INFINITY, prevY = -INFINITY;  // used to assure I don't add the same point twice

	// extra things to be able to end file reading when finding keyword
	string wholeLineString;
	stringstream wholeLineStream;  // at top of .cpp, need >>  #include <sstream>
	bool continueReading = true;

	// go through file
	while (!input.eof() && continueReading) {

		getline(input, wholeLineString);
		if (wholeLineString.find("Path End") != string::npos) {
			continueReading = false;
		}
		else {
			wholeLineStream.str(wholeLineString);

			// read x and y from each line
			//input >> xInput >> yInput;
			wholeLineStream >> xInput >> yInput;

			// add a point to my path if it is not the same as the last point added
			if (xInput != prevX || yInput != prevY)
				thePoints.push_back({ xInput, yInput });

			// record previous input to eliminate duplication
			// ok to put these on same line since "they go together"
			prevX = xInput; prevY = yInput;

			wholeLineStream.clear();
		}
	}

	// recalculate dependent path parameters
	recalcPath();
}


bool Path2D::addPoint(Point2D newPoint, int index)
{
	if (index < 1)
		return false;   // POSSIBLE EXIT FROM FUNCTION
	else if (index > thePoints.size())
		thePoints.push_back(newPoint);
	else
		thePoints.insert(thePoints.begin() + index - 1, newPoint);

	// since a point was added, need to recalc
	recalcPath();
	return true;
}

bool Path2D::addPoint(int index, float ratio)
{
	// minimum allowed value for index is 2
	// maximum allowed value for index is the number of points
	if (index <= 1 || index > thePoints.size()
		|| ratio <= 0. || ratio >= 1.0)
		return false;   // POSSIBLE EXIT FROM FUNCTION

	Point2D backPoint = thePoints[index - 2];
	Point2D frontPoint = thePoints[index - 1];

	thePoints.insert(thePoints.begin() + index - 1, Line2D::scale(backPoint, frontPoint, ratio));

	// don't need this since this new point will not change the derived parameters
	// recalcPath();

	return true;
}

bool Path2D::removePoint(int index)
{
	if (0 < index && index <= thePoints.size()) {
		thePoints.erase(thePoints.begin() + index - 1);
		recalcPath();
		return true;
	}
	else
		return false;
}

std::ostream& operator<<(std::ostream& os, const Path2D& aPath)
{
	// output all points
	for (int i = 0; i < aPath.thePoints.size(); i++) {
		os << aPath.thePoints[i].x << "\t" << aPath.thePoints[i].y << endl;
	}

	return os;
}

void Path2D::moveAllPoints(float deltaX, float deltaY)
{
	for (auto& currPnt : thePoints) {
		currPnt.x += deltaX;
		currPnt.y += deltaY;
	}
	recalcPath();
}

void Path2D::scale(Point2D refPnt, float scaleX, float scaleY)
{
	for (auto& currPnt : thePoints) {
		currPnt.x = (currPnt.x - refPnt.x) * scaleX + refPnt.x;
		currPnt.y = (currPnt.y - refPnt.y) * scaleY + refPnt.y;
	}
	recalcPath();

}

void Path2D::rotate(Point2D refPnt, float angleDeg)
{
	float angleRad = angleDeg * atan(1.) / 45.;
	Point2D oldPnt;  // needed since old values of both X and Y are used for new values
	for (auto& currPnt : thePoints) {
		oldPnt = currPnt;
		currPnt.x = (oldPnt.x - refPnt.x) * cos(angleRad) - (oldPnt.y - refPnt.y) * sin(angleRad) + refPnt.x;
		currPnt.y = (oldPnt.x - refPnt.x) * sin(angleRad) + (oldPnt.y - refPnt.y) * cos(angleRad) + refPnt.y;
	}
	recalcPath();
}

void Path2D::mirror(Point2D startPnt, Point2D endPnt)
{
	// rotate the path so that the given line "becomes" the x-axis
	float lineAngle = Line2D::getAngle(startPnt, endPnt);
	rotate(startPnt, -lineAngle);

	// flip the path about x axis by scaling -1 for y scale
	scale(startPnt, 1, -1);

	// rotate to original position
	rotate(startPnt, lineAngle);
	recalcPath();
}
