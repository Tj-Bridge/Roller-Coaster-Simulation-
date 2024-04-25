#include <sstream>
#include <fstream>
#include "StringPlus.h"

#include "Track.h"
#include "CubicSpline2D.h"
#include "ColorConverter.h"
#include "Car.h"

using namespace std;

void Track::readFile(std::ifstream& input)
{
	string wholeLineString;
	stringstream wholeLineStream;
	bool continueReading = true;

	// go through file
	while (!input.eof() && continueReading) {

		getline(input, wholeLineString);
		if (wholeLineString.find("Track End") != string::npos) {
			continueReading = false;
		}
		else {

			int colonLocation = wholeLineString.find(":");
			if (colonLocation != string::npos)
				wholeLineStream.str(wholeLineString.substr(colonLocation + 1));

			if (wholeLineString.find("trackName") != string::npos) {
				trackName = StringPlus::trim(
					wholeLineString.substr(colonLocation + 1));
			}
			else if (wholeLineString.find("displayWidth") != string::npos) {
				wholeLineStream >> displayWidth;
			}
			else if (wholeLineString.find("displayHue") != string::npos) {
				wholeLineStream >> displayHue;
			}
			else if (wholeLineString.find("frictionCoeff") != string::npos) {
				wholeLineStream >> frictionCoeff;
			}
			else if (wholeLineString.find("Path Start") != string::npos) {
				if (geometry != nullptr)
					delete geometry;  // just in case

				// allow for selective spline
				if (wholeLineString.find("Spline") != string::npos) {
					pathIsSpline = true;
					geometry = new CubicSpline2D(input);
				}
				else {
					pathIsSpline = false;
					geometry = new Path2D(input);
				}
			}
			//else if (wholeLineString.find("Car Start") != string::npos) {
			//	if (itsCar != nullptr)
			//		delete itsCar;  // just in case
			//	itsCar = new Car(input);
			//	itsCar->setTrack(this); // inverse relationship
			//}
			else if (wholeLineString.find("Control Start") != string::npos) {
				theControls.push_back(ControlSection(input));
			}

			wholeLineStream.clear();
		}
	}
}

float Track::getTrackHeight()
{
	if (geometry != nullptr) {
		return geometry->upperBoundingBox().y
			- geometry->lowerBoundingBox().y;
	}
	else
		return 0.0f;
}

float Track::getLargestRise()
{
	// note, since I am using Path2D, I don't have to worry about index-1 stuff
	// i.e., Track class is like a "client" of Path2D
	if (geometry == nullptr || geometry->numbPoints() < 2)
		return 0.f;
	else {
		float largestSoFar = 0.f;
		float currRise = 0.f;
		Point2D prevPoint = geometry->getPoint(1);
		Point2D currPoint;
		int pathNumbPoints = geometry->numbPoints(); // so I don't have to call over and over
		for (int i = 2; i <= pathNumbPoints; i++) {
			currPoint = geometry->getPoint(i);
			if (prevPoint.y <= currPoint.y) // current rise continues
				currRise += currPoint.y - prevPoint.y;
			else { // current rise is at an end
				if (currRise > largestSoFar) // remember largest rise
					largestSoFar = currRise;
				currRise = 0;
			}
			prevPoint = currPoint;
		}
		if (currRise > largestSoFar) // in case track ends on largest rise
			largestSoFar = currRise;

		return largestSoFar;
	}

}

float Track::getLargestDrop()
{
	if (geometry == nullptr || geometry->numbPoints() < 2)
		return 0.f;
	else {
		float largestSoFar = 0.f;
		float currDescent = 0.f;
		Point2D prevPoint = geometry->getPoint(1);
		Point2D currPoint;
		int pathNumbPoints = geometry->numbPoints(); // so I don't have to call over and over
		for (int i = 2; i <= pathNumbPoints; i++) { // could have done with while loop, but I'm a bit lazy :-)
			currPoint = geometry->getPoint(i);
			if (prevPoint.y >= currPoint.y) // current descent continues
				currDescent -= currPoint.y - prevPoint.y;  // minus-equal so descent gets "bigger"
			else { // current descent is at an end
				if (currDescent > largestSoFar) // remember largest descent
					largestSoFar = currDescent;
				currDescent = 0;
			}
			prevPoint = currPoint;
		}
		if (currDescent > largestSoFar) // in case track ends on largest descent
			largestSoFar = currDescent;
		return largestSoFar;
	}
}

void Track::paint(Graphics^ g, bool showPoints, bool showLabels)
{
	if (geometry != nullptr) {
		float controlWidth = max(0.1, displayWidth * 1.8);
		for (auto& currControl : theControls) {  // ampersand to avoid copying
			currControl.paint(g, geometry, controlWidth,
				ColorNG::colorFromHSV(displayHue, 0.5f, 0.9f));
		}

		geometry->paint(g, *getColor(), showPoints, displayWidth, showLabels);
	}
	//if (itsCar != nullptr) {
	//	itsCar->paint(g, *getColor(), showPoints, displayWidth);
	//}
}

System::Drawing::Color^ Track::getColor() const
{
	return ColorNG::colorFromHSV(displayHue, 1.f, 1.f);
}

void Track::setColor(Color c)
{
	float sat, val;
	ColorNG::colorToHSV(c, displayHue, sat, val);
}

ControlSection* Track::getControlFromLength(float givenLength)
{
	for (auto& currControl : theControls) {  // ampersand to avoid copying
		if (currControl.inSegment(givenLength))
			return &currControl;    // ampersand since it needs an address for the pointer
	}
	// if no control on given length, just return nullptr
	return nullptr;
}

std::ostream& operator<<(std::ostream& os, const Track& aTrack)
{
	os << "Track Start:" << endl;
	os << "\ttrackName: " << aTrack.trackName << endl;
	os << "\tdisplayWidth: " << aTrack.displayWidth << endl;
	os << "\tdisplayHue: " << aTrack.displayHue << endl;
	os << "\tfrictionCoeff: " << aTrack.frictionCoeff << endl;

	if (aTrack.geometry != nullptr) {
		if (aTrack.pathIsSpline)
			os << "Path Start (Spline):" << endl;
		else
			os << "Path Start:" << endl;
		os << *(aTrack.geometry);
		os << "Path End:" << endl;
	}
	for (auto& currControl : aTrack.theControls) {  // ampersand to avoid copying
		os << "Control Start:" << endl;
		os << currControl;
		os << "Control End:" << endl;
	}

	os << "Track End:" << endl;
	return os;
}
