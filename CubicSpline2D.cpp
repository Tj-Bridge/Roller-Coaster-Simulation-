#include "ColorConverter.h"
#include "CubicSpline2D.h"

void CubicSpline2D::paint(Graphics^ g, Color c, bool showPoints, float width,
	bool showLabels, bool fill)
{
	//Path2D::paint(g, c, showPoints, width, showLabels); 

	cli::array<PointF>^ pointArray = gcnew cli::array<PointF>(thePoints.size());
	int i = 0;
	for (auto& currPnt : thePoints)
		pointArray[i++] = PointF(currPnt.x, currPnt.y);

	auto aPen = gcnew Pen(c, width);

	if (fill) {
		float h, s, v;
		ColorNG::colorToHSV(c, h, s, v);
		auto fillBrush = gcnew SolidBrush(ColorNG::colorFromHSV(h, s / 2, v));
		g->FillClosedCurve(fillBrush, pointArray);
		g->DrawClosedCurve(aPen, pointArray);
	}
	else {
		g->DrawCurve(aPen, pointArray, splineTension);
		//g->DrawLines(linePen, pointArray);

	}



	// for hard way to generate path graphics
	// uncomment this code to confirm that generated spline points actually follow
	// graphical spline curve

	//Pen^ linePen = gcnew Pen(Color::Black, 0.);
	//for (int i = 1; i < theSplinePoints.size(); i++) {
	//	g->DrawLine(linePen, theSplinePoints[i - 1].x, theSplinePoints[i - 1].y,
	//		theSplinePoints[i].x, theSplinePoints[i].y);
	//}

	if (showPoints || showLabels)
		paintPoint(g, c, showLabels);

}

Point2D CubicSpline2D::coordsFromLength(float givenLength)
{
	// if there are no points or givenLength is bad, return error signal
	if (thePoints.size() == 0 || givenLength < 0 || givenLength > currLength)
		return { -INFINITY, -INFINITY };
	else if (givenLength == 0) // requesting start of path
		return thePoints[0];
	else if (givenLength == currLength) // requesting end of path
		return thePoints.back();
	else {
		// actually look for point along path
		int currPoint = 0;
		float lengthToBackPoint = 0.f, lengthToForwardPoint = 0.f;

		// serial: O(N)
		//while (currPoint < thePoints.size() && lengthToForwardPoint < givenLength) {
		//	lengthToBackPoint = lengthToForwardPoint; // keep track of prev calculation
		//	currPoint++;
		//	lengthToForwardPoint += Line2D::getLength(thePoints[currPoint],
		//		thePoints[currPoint - 1]);
		//}

		// once loop exits, currPoint is the point where lengthToForwardPoint is 
		// just bigger than givenLength. 
		//    i.e., givenLegth is between lengthToBackPoint and lengthToForwardPoint  
		// time to interpolate

		// using indexFromLength (binary search)
		currPoint = indexFromLength(givenLength) - 1; // -1 since indexFromLength uses 1 as first point
		lengthToBackPoint = subLengths[currPoint - 1];
		lengthToForwardPoint = subLengths[currPoint];

		float interpolationRatio = (givenLength - lengthToBackPoint)
			/ (lengthToForwardPoint - lengthToBackPoint);

		return Line2D::scale(theSplinePoints[currPoint - 1], theSplinePoints[currPoint], interpolationRatio);
	}

}

float CubicSpline2D::angleFromLength(float givenLength)
{
	int currPoint = indexFromLength(givenLength);
	if (currPoint <= 0)
		return -INFINITY;
	else
		return Line2D::getAngle(theSplinePoints[currPoint - 1], theSplinePoints[currPoint]);

}

void CubicSpline2D::recalcPath()
{
	// first, use thePoints to generate theSplinePoints
	// then, go through all spline points in the path and make good decisions
	currLength = 0.f;
	minX = minY = maxX = maxY = -INFINITY;  // this means "error"
	theSplinePoints.clear();
	subLengths.clear();

	if (thePoints.size() >= 2) {

		// generate cubic spline points
		for (int i = 0; i < thePoints.size() - 1; i++) {
			generateEdgePoints(i);
		}

		if (theSplinePoints.size() > 0) {
			minX = maxX = theSplinePoints[0].x;
			minY = maxY = theSplinePoints[0].y;
			subLengths.push_back(0.f);

			for (int i = 1; i < theSplinePoints.size(); i++) {
				currLength += Line2D::getLength(theSplinePoints[i - 1], theSplinePoints[i]);
				subLengths.push_back(currLength);  // store the length from start to curr point
				if (theSplinePoints[i].x < minX)
					minX = theSplinePoints[i].x;
				else if (theSplinePoints[i].x > maxX)  // a tiny bit better performance using else if
					maxX = theSplinePoints[i].x;

				if (theSplinePoints[i].y < minY)
					minY = theSplinePoints[i].y;
				else if (theSplinePoints[i].y > maxY)
					maxY = theSplinePoints[i].y;
			}
		}
	}
}

void CubicSpline2D::generateEdgePoints(int edgeIndex)
{
	int splineSubdivisions = 6; // maybe this should be based on length of edge
	float c = splineTension; // spline tension (0.0 means no spline at all)

	Point2D prev, curr, next, nextNext;
	curr = thePoints[edgeIndex];
	next = thePoints[edgeIndex + 1];

	// if edgeIndex == 0, develop prev point as extension before start of path
	if (edgeIndex == 0)
		prev = Line2D::scale(thePoints[0], thePoints[1], -0.2);
	else
		prev = thePoints[edgeIndex - 1];

	// if edgeIndex == numbpoints - 2, develop nextNext as extension past end of path
	if (edgeIndex == thePoints.size() - 2)
		nextNext = Line2D::scale(thePoints[thePoints.size() - 2],
			thePoints[thePoints.size() - 1], 1.2);
	else
		nextNext = thePoints[edgeIndex + 2];

	float xa = -c * prev.x + (2. - c) * curr.x
		+ (c - 2.) * next.x + c * nextNext.x;
	float xb = 2. * c * prev.x + (c - 3.) * curr.x
		+ (3. - 2. * c) * next.x - c * nextNext.x;
	float xc = -c * prev.x + c * next.x;
	float xd = curr.x;

	float ya = -c * prev.y + (2. - c) * curr.y
		+ (c - 2.) * next.y + c * nextNext.y;
	float yb = 2. * c * prev.y + (c - 3.) * curr.y
		+ (3. - 2. * c) * next.y - c * nextNext.y;
	float yc = -c * prev.y + c * next.y;
	float yd = curr.y;

	// add the spline points
	if (edgeIndex == 0) // add the first point if at start of process
		theSplinePoints.push_back(curr);

	float t, x, y;
	for (int k = 1; k <= splineSubdivisions; k++) {
		t = float(k) / splineSubdivisions;  // parameter
		x = xa * t * t * t + xb * t * t + xc * t + xd;
		y = ya * t * t * t + yb * t * t + yc * t + yd;

		theSplinePoints.push_back({ x,y });
	}
}

float CubicSpline2D::lengthToPoint(int index)
{
	// trivial since we already have all the subLengths
	if (0 < index && index <= subLengths.size())
		return subLengths[index - 1];
	else
		return -1;
}

int CubicSpline2D::indexFromLength(float givenLength)
{
	if (theSplinePoints.size() <= 1 || givenLength < 0 || givenLength >= currLength)
		return  -1;
	else if (givenLength == 0) // requesting start of path
		return 1;
	else {
		// actually look for point along path
		int currPointIndex;

		// serial search: O(N)

		// once loop exits, currPointIndex is the point where lengthToForwardPoint is 
		// just bigger than givenLength. 

		//currPointIndex = 0;
		//float lengthToForwardPoint = 0.f;
		//while (currPointIndex < theSplinePoints.size() 
		// && lengthToForwardPoint < givenLength) {
		//	currPointIndex++;
		//	lengthToForwardPoint += Line2D::getLength(theSplinePoints[currPointIndex],
		//		theSplinePoints[currPointIndex - 1]);
		//}

		// binary search: O(logN) (1024 points using only 10 runs through loop)

		int lower = 0, upper = theSplinePoints.size() - 1; // there are least 2 points
		currPointIndex = (lower + upper) / 2; // start at middle point
		// loop until givenLength is between two adjacent sublengths
		while (!(subLengths[currPointIndex] <= givenLength
			&& givenLength < subLengths[currPointIndex + 1])) {
			if (givenLength < subLengths[currPointIndex])  // look to lower half
				upper = currPointIndex;
			else // look to upper half
				lower = currPointIndex;
			currPointIndex = (lower + upper) / 2;
		}
		currPointIndex++;  // change to point AFTER givenLength
		currPointIndex++;  // change to first point index is 1 

		return currPointIndex;
	}
}

