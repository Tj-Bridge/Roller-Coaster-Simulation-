/*
									Nestor Gomez
									Carnegie Mellon University
				_ _ . 	_			C++ for Engrs, 24-280
			  .'	 `./  \			PS07.  Due Tues. Apr. 16, 2024
			 /		  /`   |
			/        |  \._/		Class for modeling a roller coaster
  ._ _ _ _./          \				track, making use of a Path2D

*/

#pragma once
#include <string>
#include <vector>
#include "StringPlus.h"
#include "Path2D.h"
#include "ControlSection.h"

class Train;

using namespace System::Drawing;

class Track {
	Path2D* geometry = nullptr;

	std::vector<ControlSection> theControls;
	std::string trackName = "";
	float displayWidth = 0.f;
	float displayHue = 0.f;
	float frictionCoeff = 0.f;
	float maxObservedSpeed = 0.f;

	Train* itsTrain = nullptr;
	//Car* itsCar = nullptr;
	bool pathIsSpline = false;

public:
	Track() {};
	Track(std::ifstream& input) { readFile(input); }

	void readFile(std::ifstream& input);

	//void writeFile(std::ofstream& outputFile);

	friend std::ostream& operator<<(std::ostream& os, const Track& aTrack);

	float getTrackHeight();
	float getLargestRise();
	float getLargestDrop();

	Path2D* getPath() { return geometry; }
	void setPath(Path2D* aPath) { // old geometry should be deleted
		if (geometry != nullptr)
			delete geometry;
		geometry = aPath; 
	} 

	Train* getTrain() { return itsTrain; }
	void setTrain(Train* aTrain) { itsTrain = aTrain; } // old train should NOT be deleted

	float getWidth() { return displayWidth; }
	void setWidth(float newWidth) { displayWidth = newWidth; }

	float getFriction() { return frictionCoeff; }
	void setFriction(float newFriction) { frictionCoeff = newFriction; }

	std::string getName() { return trackName; }
	void setName(std::string newName) { trackName = StringPlus::trim(newName); }

	void paint(Graphics^ g, bool showPoints = false, bool showLabels = false);

	System::Drawing::Color^ getColor() const;
	void setColor(Color c);

	ControlSection* getControlSection(int index) {
		if (0 < index && index <= theControls.size())
			return &(theControls.at(index - 1));
		else
			return nullptr;
	}
	ControlSection* getControlFromLength(float givenLength);

	void addControlSection() {
		theControls.push_back(ControlSection()); // will add empty control sectoin
	}

	void deleteControlSection(int index) {
		if (0 < index && index <= theControls.size())
			theControls.erase(theControls.begin() + index - 1);
	}

};
