#include <sstream>
#include <fstream>
#include "StringPlus.h"

#include "Track.h"
#include "CubicSpline2D.h"
#include "ColorConverter.h"
#include "Train.h"

using namespace std;

Train::~Train()
{
	for (auto& currCar : itsCars) {
		delete currCar;
	}
	// itsCars.clear(); // not needed since this is a member variable
}

void Train::readFile(std::ifstream& input)
{
	string wholeLineString;
	stringstream wholeLineStream;  // doesn't get used, but keep for future?
	bool continueReading = true;

	// go through file
	while (!input.eof() && continueReading) {

		getline(input, wholeLineString);
		if (wholeLineString.find("Train End") != string::npos) {
			continueReading = false;
		}
		else {

			int colonLocation = wholeLineString.find(":");
			if (colonLocation != string::npos)
				wholeLineStream.str(wholeLineString.substr(colonLocation + 1));

			if (wholeLineString.find("name") != string::npos) {
				name = StringPlus::trim(
					wholeLineString.substr(colonLocation + 1));
			}
			else if (wholeLineString.find("Car Start") != string::npos) {
				Car* aCar = new Car(input);
				aCar->setTrain(this); // inverse relationship
				addCarBack(aCar);
			}

			wholeLineStream.clear();
			wholeLineStream.str(""); // set to empty string
		}
	}
}

void Train::advanceTrain(float timeStep)
{
	// Are there any cars? Also, is there a path?
	if (itsCars.size() > 0 && itsTrack != nullptr && itsTrack->getPath() != nullptr) {

		// move train to new location using currVel and currAccel of first car
		itsCars[0]->moveCarNoAccel(timeStep);
		for (int i = 1; i < itsCars.size(); i++)
		{
			itsCars[i]->setCurrVelAndAccel(itsCars[0]->getCurrVel(), 0.0f);
		}
		relocateFollowingCars();

		float newVel, newAccel;

		// check if train is still on the track. 
		// set newVel and newAccel to zero if rear car is before start or lead car is after end
		if (itsCars.back()->getCurrLoc() < 0.f
			|| itsCars.front()->getCurrLoc() > itsTrack->getPath()->length()) {
			newVel = newAccel = 0.f;
		}
		else {
			// determine newAccel using tough math
			newVel = itsCars[0]->getCurrVel();
			int velSign;
			if (newVel < 0)
				velSign = -1;
			else
				velSign = 1;

			float alpha = 0, beta = 0;
			for (int i = itsCars.size() - 1; i >= 0; i--) {
				itsCars[i]->performAccelCalc(alpha, beta, velSign);
			}

			// coming out of loop, we have 0 = alpha * newAccel + beta
			// I don't think alpha can be zero unless there are no masses at all.
			if (fabsf(alpha) > 1e-6)
				newAccel = -beta / alpha;
			else
				newAccel = 0.f;
		}

		// set currAccel and currVel of all cars to new values
		// actually, I only really use the values of leading car, but it's nice to 
		// let "everyone" know their vel and accel :-)
		for (auto& currCar : itsCars) {
			currCar->setCurrVelAndAccel(newVel, newAccel);
		}

		currTime += timeStep;
		//accelGraph.push_back({ currTime, newAccel });
		//velGraph.push_back({ currTime, newVel });
	}
}

void Train::initialize()
{
	if (!itsCars.empty()) {
		currTime = 0.f;
		//accelGraph.clear();
		//velGraph.clear();

		// get the Path
		Path2D* thePath = itsTrack->getPath();
		if (thePath != nullptr) {
			// set currLoc of first car as max of initLoc or the length 
			float currTotalLength = length();
			float assignedInitialLoc = itsCars[0]->getProperty("initialLoc");
			itsCars[0]->setProperty("initialLoc", max(currTotalLength, assignedInitialLoc));

			itsCars[0]->initialize();
			relocateFollowingCars();
			for (int i = 1; i < itsCars.size(); i++) {
				itsCars[i]->setProperty("initialLoc", itsCars[i]->getCurrLoc());
				itsCars[i]->initialize();
			}
		}
	}
}

bool Train::addCar(Car* aCar, int index, bool addAhead)
{
	aCar->setTrain(this);

	if (itsCars.empty() || (index == 1 && addAhead))
		addCarFront(aCar);
	else if (index == itsCars.size() && !addAhead)
		addCarBack(aCar);
	else if (1 <= index && index <= itsCars.size()) {
		// now we are inserting "between"
		int frontIndex, backIndex, insertIndex;
		if (addAhead) {
			frontIndex = index + 1;
			backIndex = index;
		}
		else {
			frontIndex = index;
			backIndex = index - 1;
		}
		//aCar->setAheadCar(itsCars[frontIndex - 1]);
		//aCar->setBehindCar(itsCars[backIndex - 1]);
		itsCars.insert(itsCars.begin() + backIndex, aCar);
		resetInterCarPointers();
	}
	else
		return false;

	return true;

}

void Train::paint(Graphics^ g, int highlightCarIndex)
{
	int carNumber = 0;
	stringstream carLabel;
	Car* aheadCar = nullptr;

	for (auto& currCar : itsCars) {  // ampersand to avoid copying
		carNumber++;

		carLabel << "Car " << carNumber;
		if (highlightCarIndex == carNumber)
			int i = 1;
		currCar->paint(g, ColorNG::colorFromHSV((carNumber - 1) * 40.f, 1, 1), false, 0.02f,
			carLabel.str(), highlightCarIndex == carNumber);
		carLabel.str(""); // clears stringstream

		// a not-so-good attempt at a linkage between cars
		if (aheadCar != nullptr) {
			Point2D start = currCar->getFrontPoint();
			Point2D end = aheadCar->getBackPoint();
			g->DrawLine(gcnew Pen(Color::Purple, 0.2), start.x, start.y, end.x, end.y);
		}
		aheadCar = currCar;
	}
}

float Train::length()
{
	// getting total length from front wheel of first car to rear wheel of last car
	float totalLength = 0.f;
	if (itsCars.size() > 0) {
		float backAttach, frontAttach;
		totalLength = itsCars[0]->getProperty("wheelBaseLength");
		for (int i = 1; i < itsCars.size(); i++) {
			backAttach = itsCars[i - 1]->getRearSpacing();
			frontAttach = itsCars[i]->getFrontSpacing();
			totalLength += max(0., backAttach) + max(0., frontAttach)  // use positive only?
				+ itsCars[i]->getProperty("wheelBaseLength");
		}
	}
	return totalLength;
}

int Train::getCar(Point2D checkPoint)
{
	int currIndex = 1;
	for (auto& currCar : itsCars) {  // ampersand to avoid copying
		if (currCar->isAtPoint(checkPoint))
			return currIndex;
		else
			currIndex++;
	}
	return -1;
}

void Train::relocateFollowingCars()
{
	// get the location of first car and use it to set the location of second car
	// then continue down the line 
	// assume that distance from back wheel of one car to the front wheel of the 
	// following car is related to frontSpacing and rearSpacing, respectively
	if (itsCars.size() > 1 && itsTrack != nullptr && itsTrack->getPath() != nullptr) {
		Path2D* thePath = itsTrack->getPath();

		Point2D backPointOfAheadCar, frontPointOfThisCar;
		float locOfBackPointOfAheadCar, locOfFrontPointOfThisCar;
		float carToCarLength, startLookingAfterLoc;

		for (int i = 1; i < itsCars.size(); i++) {

			carToCarLength = max(0., itsCars[i - 1]->getRearSpacing())
				+ max(0., itsCars[i]->getFrontSpacing());

			backPointOfAheadCar = itsCars[i - 1]->getBackPoint();
			startLookingAfterLoc = itsCars[i - 1]->getCurrLoc() - 2 * itsCars[i - 1]->getProperty("wheelBaseLength");
			locOfBackPointOfAheadCar = thePath->lengthFromCoords(backPointOfAheadCar, startLookingAfterLoc);

			startLookingAfterLoc = locOfBackPointOfAheadCar - 2 * itsCars[i]->getProperty("wheelBaseLength");
			frontPointOfThisCar = thePath->coordsBackPointFromLength(locOfBackPointOfAheadCar, carToCarLength);
			locOfFrontPointOfThisCar = thePath->lengthFromCoords(frontPointOfThisCar, startLookingAfterLoc);

			if (locOfFrontPointOfThisCar < 0)
				locOfFrontPointOfThisCar = locOfFrontPointOfThisCar; // for debugging only

			itsCars[i]->setCurrLoc(locOfFrontPointOfThisCar); // will also relocate car
			//itsCars[i]->relocateCar();
		}
	}
}

void Train::resetInterCarPointers()
{
	if (!itsCars.empty()) {
		itsCars.front()->setAheadCar(nullptr);
		itsCars.back()->setBehindCar(nullptr);

		for (int i = 1; i < itsCars.size(); i++) {  // ampersand to avoid copying
			itsCars[i]->setAheadCar(itsCars[i - 1]);
		}
	}
}

std::ostream& operator<<(std::ostream& os, const Train& aTrain)
{

	os << "Train Start:" << endl;
	os << "\tname: " << aTrain.name << endl;
	for (auto& currCar : aTrain.itsCars) {  // ampersand to avoid copying
		//os << "Car Start:" << endl;  // this is now done by Car itself
		os << *currCar;
		//os << "Car End:" << endl;
	}

	os << "Train End:" << endl;
	return os;
}
