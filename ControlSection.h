/*
									Nestor Gomez
									Carnegie Mellon University
				_ _ . 	_			C++ for Engrs, 24-280
			  .'	 `./  \			PS05.  Due Thurs. Mar. 21, 2024
			 /		  /`   |
			/        |  \._/		Class for modeling a control section
  ._ _ _ _./          \				of a track. A control section is used
									to apply external forces to car trains
									in addition to gravity and friction.
*/
#pragma once
#include <map>
#include "StringPlus.h"
#include "Path2D.h"

class ControlSection {

protected:
	std::string controlName = "";
	//float startLoc = 0.f, endLoc = 0.f;
	//float startForce = 0.f, endForce = 0.f;

	// alternative method of maintaining multiple parameters
	// makes get() and set() functions very easy/expandable
	// makes readfile and writefile infinitely expandable.
	std::map<std::string, float> params;

public:
	ControlSection() { clearParams(); }
	ControlSection(std::ifstream& input) { readFile(input); }

	void readFile(std::ifstream& input);

	bool inSegment(float givenLength) { return inSection(givenLength); }
	bool inSection(float givenLength) { // better phrasing ? ? ?
		return params["startLoc"] <= givenLength && givenLength <= params["endLoc"];
	}

	std::string getName() { return controlName; }
	void setName(std::string newName) { controlName = StringPlus::trim(newName); }

	float getForceValue(float givenLength);

	//New Implementation
	float getForceValue(float givenLength, float currVel, float mass);

	void paint(Graphics^ g, Path2D* thePath, float width, Color c = Color::Orange, int numbSegments = 30);

	friend std::ostream& operator<<(std::ostream& os, const ControlSection& aSection);

	float get(std::string paramKey) {
		if (params.find(paramKey) != params.end()) // prevents error
			return params[paramKey];
	}
	void set(std::string paramKey, float val) {
		if (params.find(paramKey) != params.end()) // doesn't allow new parameters
			params[paramKey] = val;
	}

protected:
	void clearParams() {
		// assure that only these four are allowed

		params["startLoc"] = 0.f;
		params["endLoc"] = 0.f;
		params["startForce"] = 0.f;
		params["endForce"] = 0.f;
		params["finalVelocity"] = 0.f;
	}
};