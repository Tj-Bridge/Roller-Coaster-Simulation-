#include <sstream>
#include <fstream>
#include "StringPlus.h"

#include "ControlSection.h"
using namespace std;

void ControlSection::readFile(std::ifstream& input)
{
	string wholeLineString;
	stringstream wholeLineStream;
	bool continueReading = true;

	// reset everything to zero
	clearParams();

	// go through file
	while (!input.eof() && continueReading) {

		getline(input, wholeLineString);
		if (wholeLineString.find("Control End") != string::npos) {
			continueReading = false;
		}
		else {

			int colonLocation = wholeLineString.find(":");
			if (colonLocation != string::npos)
				wholeLineStream.str(wholeLineString.substr(colonLocation + 1));

			if (wholeLineString.find("controlName") != string::npos) {
				controlName = StringPlus::trim(
					wholeLineString.substr(colonLocation + 1));
			}
			else {
				// get keyword from before colon
				string key = StringPlus::trim(
					wholeLineString.substr(0, colonLocation));
				
				// get value and assign to parameter
				// not clear if I should allow new parameters
				if (params.find(key) != params.end()) 
					wholeLineStream >> params[key];
			}

			// "Old way" of doing it
			//else if (wholeLineString.find("startLoc") != string::npos) {
			//	wholeLineStream >> startLoc;
			//}
			//else if (wholeLineString.find("endLoc") != string::npos) {
			//	wholeLineStream >> endLoc;
			//}
			//else if (wholeLineString.find("startForce") != string::npos) {
			//	wholeLineStream >> startForce;
			//}
			//else if (wholeLineString.find("endForce") != string::npos) {
			//	wholeLineStream >> endForce;
			//}

			wholeLineStream.clear();
		}
	}

}

float ControlSection::getForceValue(float givenLength)
{
	if (inSection(givenLength)) {
		// using extra variables for easier debugging
		float distFromStart = givenLength - params["startLoc"];

		// interpolate 
		float value = params["startForce"] + distFromStart / (params["endLoc"] - params["startLoc"]) * (params["endForce"] - params["startForce"]);
		return value;
	}
	else
		return 0.0f;
}

// New Implementation 
float ControlSection::getForceValue(float givenLength, float currVel, float mass)
{
	if (inSection(givenLength)) {
		float distFromStart = givenLength - params["startLoc"];
		float disttoEnd = params["endLoc"] - givenLength;
		// a = (vf^2 - vi^2)/2(dx)
		float accel = (params["finalVelocity"] * params["finalVelocity"] - currVel * currVel) / (2 * (distFromStart) + 0.1);
		// Derive from velocity. F=ma -> F=m(vf-vi)/tf-ti ; vi = currVelocity 
		float force = mass * accel;
		return force;
	}
	else
		return 0.0f;
}
void ControlSection::paint(Graphics^ g, Path2D* thePath, float width, Color c, int numbSegments)
{
	if (thePath != nullptr && numbSegments > 0) {
		auto startLoc = params["startLoc"];
		auto endLoc = params["endLoc"];

		Point2D prevPnt = thePath->coordsFromLength(startLoc);
		Point2D currPnt;

		auto aPen = gcnew Pen(c, width);

		// for "smooth" line
		auto aGPath = gcnew System::Drawing::Drawing2D::GraphicsPath();
		aGPath->StartFigure();
		for (int i = 0; i < numbSegments; i++) {
			currPnt = thePath->coordsFromLength(startLoc + (i * (endLoc - startLoc)) / numbSegments);
			aGPath->AddLine(prevPnt.x, prevPnt.y, currPnt.x, currPnt.y);
			prevPnt = currPnt;
		}
		aPen->LineJoin = System::Drawing::Drawing2D::LineJoin::Round; // there are other join types
		g->DrawPath(aPen, aGPath);

	}
}

std::ostream& operator<<(std::ostream& os, const ControlSection& aSection)
{
	os << "\tcontrolName: " << aSection.controlName << endl;
	for (auto it = aSection.params.begin(); it != aSection.params.end(); it++) {
		os << "\t" << it->first << ": " << it->second << endl;
	}

	//os << "\tstartLoc: " << aSection.startLoc <<endl;
	//os << "\tendLoc: " << aSection.endLoc<< endl;
	//os << "\tstartForce: " << aSection.startForce << endl;
	//os << "\tendForce: " << aSection.endForce << endl;

	return os;
}
