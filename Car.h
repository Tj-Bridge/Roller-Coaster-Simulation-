/*

									Carnegie Mellon University
	   __..-------------...__		C++ for Engrs, 24-280
	 .".--.```|    - /.--.  =:		PS7.  Due Tues. Apr. 16, 2024
	(.: {} :__L______: {} :__;
	   *--*           *--*      	Class for modeling a rollercoaster
									car. Implements kinetics to simulate
									motion along a track.

*/

#pragma once
#include <fstream>
#include <unordered_map>
//#include "Track.h"  // needed??
//#include "Train.h"
#include "Path2D.h"

class Train;

using namespace System::Drawing;

class Car {
protected:
	// car properties (saved to file)
	std::string carID;

	// using unordered_map instead (for easier expansion)
	std::unordered_map<std::string, float> params;
	Point2D frontLatch, rearLatch;
	Path2D* geometry = nullptr;

	// simulation parameters (not saved to file)
	Train* itsTrain = nullptr;
	float currLoc = -1;
	float currVel = -1;
	float currAccel = -1;
	float currTimeStamp = -1; // seconds

	int currPassengerCount = 0;

	Point2D currFrontPoint = { -INFINITY, -INFINITY };  // these will make it easier to paint
	Point2D currBackPoint = { -INFINITY, -INFINITY };

public:
	// constructors
	Car() { setDefaults(); }
	Car(std::ifstream& inputStream) {
		readFile(inputStream);
	}

	// called by one of the class constructors.
	// reads car properties from file stream.
	void readFile(std::ifstream& inputStream);

	// creates link to track that this car rides on
	//void setTrack(Track* aTrack) {
	//	itsTrack = aTrack;
	//}

	// creates link to track that this car rides on
	void setTrain(Train* aTrain) {
		itsTrain = aTrain;
	}

	// creates bi-directional relationship
	void setAheadCar(Car* other) {
		if (other != behindCar) { // avoid making a loop
			aheadCar = other;
			if (other != nullptr)
				other->behindCar = this;
		}
	}

	// creates bi-directional relationship
	void setBehindCar(Car* other) {
		if (other != aheadCar) { // avoid making a loop
			behindCar = other;
			if (other != nullptr)
				other->aheadCar = this;
		}
	}

	// sets all simulation parameters to initial conditions
	void initialize() {
		setCurrLoc(params["initialLoc"]);
		currVel = 0.f;
		currAccel = 0.f;
		currTimeStamp = 0.f;
	}

	// allows non-simulated motion of car
	void setCurrLoc(float loc) {
		currLoc = loc;
		relocateCar();
	}

	// gets the appropriate coords for front and back wheel of car at currLoc
	void relocateCar();

	// returns the current location of the car along the track
	float getCurrLoc() { return currLoc; }

	// returns the current velocity of the car along the track
	float getCurrVel() { return currVel; }

	// obsolete now that Train class is implemented
	//// updates the car's current position, velocity, and acceleration using kinetics
	//void moveCar(float timeStep);

	// graphically paints the car on the screen at the current location and rotation
	void paint(Graphics^ g, Color c = Color::CornflowerBlue, bool showPoints = false,
		float width = 0.f, std::string textLabel = "", bool highlight = false);

	// used to output all the data of a car, but can do it to console or to a filestream. 
	friend std::ostream& operator<<(std::ostream& os, const Car& aCar);

	// determines if givenPoint world coordinates are within geometry of the car
	// to be implemented in PS07
	// bool pointInCar(Point2D givenPoint);

	float getProperty(std::string prop) {
		if (params.find(prop) != params.end())
			return params[prop];
		else
			return -1;
	}
	void setProperty(std::string prop, float value) {
		if (params.find(prop) != params.end())
			params[prop] = value;
	}

	// for PS7
public:
	Point2D getFrontPoint() { return currFrontPoint; }
	Point2D getBackPoint() { return currBackPoint; }

	// performs needed calculations to determine how the car participates in the train 
	// movement. We'll discuss in lecture since this function is the key to train kinetics
	void performAccelCalc(float& alpha, float& beta, int velSign);
	void performAccelCalc(float& alpha, float& beta, int velSign, float givenLength);
	// sets the velocity and acceleration of the car (although only lead car’s vel and 
	// accel are actually used in calculations)
	void setCurrVelAndAccel(float newVel, float newAccel) {
		currVel = newVel; currAccel = newAccel;
	}

	// get front attachment length in front of the front wheel
	float getFrontSpacing() { return frontLatch.x; };

	// get rear attachment length beyond the back wheel
	float getRearSpacing() { return -rearLatch.x - params["wheelBaseLength"]; }

	// apply velocity and acceleration, but don't calculate anything else
	// we'll use this only for the lead car
	void moveCarNoAccel(float timeStep) {
		// not sure we need t^2 term (for constant accel), but just to be more accurate?
		currLoc += currVel * timeStep + 0.5 * currAccel * timeStep * timeStep;
		currVel += currAccel * timeStep;
		relocateCar();
	}

	// returns true if the given checkpoint is within bounds of the current location of the car
	bool isAtPoint(Point2D checkPoint);

private:
	Car* aheadCar = nullptr;
	Car* behindCar = nullptr;

protected:
	void setDefaults() {
		params["initialLoc"] = 0.;
		params["mass"] = 400;
		params["wheelBaseLength"] = 2.1;
		params["wheelDiam"] = 0.3;
		params["seatHeight"] = 1.3;
		params["passengerCapacity"] = 4;

		rearLatch = { -params["wheelBaseLength"] - params["wheelDiam"] * 2.f, params["wheelDiam"] * 2.f };
		frontLatch = { params["wheelDiam"] * 2.f, params["wheelDiam"] * 2.f };

		geometry = nullptr;
	}
};