#include <sstream>
#include "ColorConverter.h"
#include "Train.h"
#include "CubicSpline2D.h"
#include "Car.h"

using namespace std;

void Car::readFile(std::ifstream& input)
{
	setDefaults();

	// reusing approach from ControlSection
	string wholeLineString;
	stringstream wholeLineStream;
	bool continueReading = true;

	// go through file
	while (!input.eof() && continueReading) {

		getline(input, wholeLineString);
		if (wholeLineString.find("Car End") != string::npos) {
			continueReading = false;
		}
		else {
			int colonLocation = wholeLineString.find(":");
			if (colonLocation != string::npos)
				wholeLineStream.str(wholeLineString.substr(colonLocation + 1));

			if (wholeLineString.find("carID") != string::npos) {
				carID = StringPlus::trim(
					wholeLineString.substr(colonLocation + 1));
			}
			else if (wholeLineString.find("frontLatch") != string::npos) {
				wholeLineStream >> frontLatch.x >> frontLatch.y;
			}
			else if (wholeLineString.find("rearLatch") != string::npos) {
				wholeLineStream >> rearLatch.x >> rearLatch.y;
			}
			else if (wholeLineString.find("Path Start") != string::npos) {
				if (geometry != nullptr)
					delete geometry;
				//geometry = new Path2D(input);
				geometry = new CubicSpline2D(input);
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
			wholeLineStream.clear();
			wholeLineStream.str("");
		}
	}
}

void Car::relocateCar()
{
	Path2D* currPath;

	if (itsTrain != nullptr && itsTrain->getTrack() != nullptr
		&& (currPath = itsTrain->getTrack()->getPath()) != nullptr) {

		currFrontPoint = currPath->coordsFromLength(currLoc);
		currBackPoint = currPath->coordsBackPointFromLength(currLoc, params["wheelBaseLength"]);
	}
}

//void Car::moveCar(float timeStep)
//{
//	Track* currTrack;
//	Path2D* currPath;
//	if (itsTrain != nullptr && (currTrack = itsTrain->getTrack()) != nullptr && (currPath = currTrack->getPath()) != nullptr) {
//		// position and velocity (assuming constant acceleration during given timeStep)
//		float prevLoc = currLoc;
//		currLoc += currVel * timeStep + 0.5 * currAccel * timeStep * timeStep;
//		currVel += currAccel * timeStep;
//
//		// acceleration is calculated based on dynamic equilibrium
//		currFrontPoint = currPath->coordsFromLength(currLoc);
//		float wheelBaseLength = params["wheelBaseLength"];
//
//		if (currFrontPoint.x > -INFINITY) { // have not moved past the end of track
//			//Point2D backPoint = currPath->coordsBackPointFromLength(currLoc, wheelBaseLength);
//			currBackPoint = currPath->coordsBackPointFromLength(currLoc, wheelBaseLength);
//			float gravity = 9.81f;
//			float friction = currTrack->getFriction() * (currVel < 0 ? -1 : 1);
//			float thetaRad = Line2D::getAngle(currBackPoint, currFrontPoint) * atan(1.) / 45.f;
//
//			// determine if we should add acceleration from 
//			float controlForce = 0.f;
//			ControlSection* currControl = currTrack->getControlFromLength(currLoc);
//			if (currControl != nullptr)
//				controlForce = currControl->getForceValue(currLoc) * 400.;  // 400 kilograms
//
//			// do force balance in track direction
//			currAccel = -friction * gravity * cos(thetaRad)
//				- gravity * sin(thetaRad) + controlForce / params["mass"];
//		}
//		else {
//			// stop the car
//			currVel = 0;
//			currAccel = 0;
//		}
//	}
//
//}

void Car::paint(Graphics^ g, Color c, bool showPoints, float width,
	std::string textLabel, bool highlight)
{
	// get parameters
	float wheelBaseLength = params["wheelBaseLength"];
	float wheelDiam = params["wheelDiam"];
	//float attachHeight = params["attachHeight"];
	//float frontAttachLength = params["frontAttachLength"];
	//float backAttachLength = params["backAttachLength"];

	Point2D frontPoint, backPoint;

	// if car CAN be placed on a track, get coords of frontPoint and backPoint
	if (currFrontPoint.x > -INFINITY && currBackPoint.x > -INFINITY) {
		frontPoint = currFrontPoint;
		backPoint = currBackPoint;
	}
	else {
		// if cannot place car on track for any reason, just paint car at 0,0
		frontPoint = { 0.f, 0.f };
		backPoint = { -wheelBaseLength, 0.f };
	}

	// use backPoint, frontPoint and car properties to paint the car
	float rotation = Line2D::getAngle(backPoint, frontPoint);
	float wheelRadius = wheelDiam / 2.f;
	float bodyLength = (frontLatch.x - rearLatch.x) * .9;
	float bodyBottom = wheelDiam * 1.2;
	float bodyHeight = max(frontLatch.y, rearLatch.y) * 1.4 - bodyBottom;
	float backX = rearLatch.x * .9;

	// set up transforms
	g->TranslateTransform(frontPoint.x, frontPoint.y);
	g->RotateTransform(rotation);

	//if (highlight) {
	//	auto yellowBrush = gcnew SolidBrush(Color::Yellow);
	//	g->FillRectangle(yellowBrush, rearLatch.x * 1.02, -wheelRadius * 0.4,
	//		bodyLength * 1.2, bodyHeight * 2);
	//}
	// draw cart
	if (geometry == nullptr) {
		Pen^ bodyPen = gcnew Pen(c, width / 8.f);
		float h, s, v;
		ColorNG::colorToHSV(c, h, s, v);

		auto bodyBrush = gcnew SolidBrush(ColorNG::colorFromHSV(h, s / 2, v));
		//float backX = -wheelBaseLength / 2.f - bodyLength / 2.f;

		// plain rectangle
		g->FillRectangle(bodyBrush, backX, bodyBottom, bodyLength, bodyHeight);
		g->DrawRectangle(bodyPen, backX, bodyBottom, bodyLength, bodyHeight);

		// front and back latch
		//float dotSize = wheelRadius * 1.2f;
		////auto dotBrush = gcnew SolidBrush(c);
		//g->FillEllipse(blackBrush, frontLatch.x - dotSize / 2.f, frontLatch.y - dotSize, dotSize, dotSize);
		//g->FillEllipse(blackBrush, rearLatch.x - dotSize / 2.f, rearLatch.y - dotSize, dotSize, dotSize);
	}

	else {
		geometry->paint(g, c, false, width / 0.5f, false, true);
	}

	// draw wheels
	Pen^ greenPen = gcnew Pen(Color::Green, width * 0.5f);
	Pen^ wheelPen = gcnew Pen(Color::Black, 0);
	auto blackBrush = gcnew SolidBrush(Color::Black);
	auto grayBrush = gcnew SolidBrush(Color::Silver);
	//wheels
	g->FillEllipse(blackBrush, -wheelRadius, 0.f, wheelDiam, wheelDiam);
	g->FillEllipse(blackBrush, -wheelBaseLength - wheelRadius, 0.f, wheelDiam, wheelDiam);
	//Rim
	float rimFactor = 2. / 3.;
	g->FillEllipse(grayBrush, -wheelRadius * rimFactor, wheelRadius - wheelDiam * rimFactor / 2.f,
		wheelDiam * rimFactor, wheelDiam * rimFactor);
	g->FillEllipse(grayBrush, -wheelBaseLength - wheelRadius * rimFactor, wheelRadius - wheelDiam * rimFactor / 2.f,
		wheelDiam * rimFactor, wheelDiam * rimFactor);


	// text

	System::Drawing::Font^ drawFont;
	SolidBrush^ drawBrush;
	PointF drawPoint;
	if (highlight) {
		drawFont = gcnew System::Drawing::Font("Arial", bodyHeight / 2.3f, FontStyle::Bold | FontStyle::Italic);
		drawBrush = gcnew SolidBrush(Color::Red);
		drawPoint = PointF(backX / 1.f, -bodyBottom - bodyHeight * 0.7f);
	}
	else {
		drawFont = gcnew System::Drawing::Font("Arial", bodyHeight / 3.f);
		drawBrush = gcnew SolidBrush(Color::Black);
		// Create point for upper-left corner of text.
		drawPoint = PointF(backX / 1.f, -bodyBottom - bodyHeight * 0.6f);
	}
	g->ScaleTransform(1, -1); // turn upside down

	// Draw string to screen.
	System::String^ theLabel;
	if (textLabel == "")
		theLabel = gcnew System::String(carID.c_str());
	else
		theLabel = gcnew System::String(textLabel.c_str());

	g->DrawString(theLabel, drawFont, drawBrush, drawPoint);

	g->ScaleTransform(1, -1); // turn upright

	// reset transforms in reverse order
	g->RotateTransform(-rotation);
	g->TranslateTransform(-frontPoint.x, -frontPoint.y);

}

void Car::performAccelCalc(float& alpha, float& beta, int velSign)
{
	Track* currTrack;
	Path2D* currPath;
	if (itsTrain != nullptr && (currTrack = itsTrain->getTrack()) != nullptr && (currPath = currTrack->getPath()) != nullptr) {
		// note, all angles are in RADIANS
		const float degToRad = atan(1.) / 45.;
		const float PI = acos(-1.);
		float mass = params["mass"];
		float mu = velSign * currTrack->getFriction();

		float thetaI = Line2D::getAngle(currBackPoint, currFrontPoint) * degToRad;
		float thetaB = 0., thetaF = 0.;
		float bFactor = 1.;
		if (behindCar != nullptr) {
			//float temp = Line2D::getAngle(currBackPoint, behindCar->getCurrFrontPoint());
			//thetaB = (180. - Line2D::getAngle(currBackPoint, behindCar->getCurrFrontPoint())) * degToRad;
			//bFactor = mu * sin(thetaB + thetaI) - cos(thetaB + thetaI);
			float thetaB = Line2D::getAngle(currBackPoint, behindCar->getFrontPoint()) * degToRad;
			bFactor = mu * sin(PI - thetaB + thetaI) - cos(PI - thetaB + thetaI);
		}
		float fFactor = 1.;
		if (aheadCar != nullptr) {
			thetaF = Line2D::getAngle(currFrontPoint, aheadCar->getBackPoint()) * degToRad;
			fFactor = mu * sin(thetaF - thetaI) + cos(thetaF - thetaI);
		}

		float gFactor = mass * 9.81 * (mu * cos(thetaI) + sin(thetaI));

		//check if there is a ControlSection and adjust for force applied
		ControlSection* usefulControl = currTrack->getControlFromLength(currLoc);
		if (usefulControl != nullptr) {
			//check if usefulControl has a force
			float controlF = usefulControl->getForceValue(currLoc, currVel, params["mass"]);
			if (controlF > -INFINITY)
				gFactor -= controlF; // since positive means it works against mass * g force when thetaI is positive
		}

		// solve for new values of alpha and beta
		alpha = (mass - alpha * bFactor) / fFactor;
		beta = (gFactor - beta * bFactor) / fFactor;
	}
}

void Car::performAccelCalc(float& alpha, float& beta, int velSign, float givenLength)
{
	float accel = (params["finalVelocity"] * params["finalVelocity"] - params["finalVelocity"] * params["finalVelocity"]) / (2*(params["endLoc"] - params["startLoc"]));
}

bool Car::isAtPoint(Point2D checkPoint)
{
	if (currFrontPoint.x == -INFINITY || currBackPoint.x == -INFINITY)
		return false;
	else {
		// The easiest method is to convert checkPoint to "local" car coordinates
		// where 0,0 is currFrontPoint and the car is perfectly horizontal (like in paint function)

		// first, adjust for location of local axes (reuse checkPoint variable)
		checkPoint.x -= currFrontPoint.x;
		checkPoint.y -= currFrontPoint.y;

		// rotate to match local tilted axis using https://en.wikipedia.org/wiki/Rotation_(mathematics)
		float thetaRad = Line2D::getAngle(currBackPoint, currFrontPoint) * atan(1.) / 45.;
		Point2D transformedPnt;

		// rotating negative angle since I want new coordinate, not new point
		transformedPnt.x = checkPoint.x * cos(-thetaRad) - checkPoint.y * sin(-thetaRad);
		transformedPnt.y = checkPoint.x * sin(-thetaRad) + checkPoint.y * cos(-thetaRad);

		// now check if transformed point is "within" car using some boundaries (rectangle, circle, etc.)
		// my boundaries: back to front in x, a little above the wheels to top of car in y
		Point2D lower = { rearLatch.x, params["wheelDiam"] * 0.2f };
		Point2D upper = { frontLatch.x, max(rearLatch.y, frontLatch.y) * 1.4f };

		return (lower.x < transformedPnt.x && transformedPnt.x < upper.x
			&& lower.y < transformedPnt.y && transformedPnt.y < upper.y);
	}
}

std::ostream& operator<<(std::ostream& os, const Car& aCar)
{
	using namespace std;
	os << "Car Start:" << endl;  // moved to here in case we're saving a single car file

	os << "\tcontrolName: " << aCar.carID << endl;
	for (auto it = aCar.params.begin(); it != aCar.params.end(); it++) {
		os << "\t" << it->first << ": " << it->second << endl;
	}
	os << "\tfrontLatch: " << aCar.frontLatch.x << " " << aCar.frontLatch.y << endl;
	os << "\trearLatch: " << aCar.rearLatch.x << " " << aCar.rearLatch.y << endl;

	os << "Car End:" << endl;
	return os;
}
