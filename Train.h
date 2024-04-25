/*
									Nestor Gomez
									Carnegie Mellon University
				_ _ . 	_			C++ for Engrs, 24-280
			  .'	 `./  \			PS07.  Due Tues. Apr. 16, 2024
			 /		  /`   |
			/        |  \._/		Class for modeling a roller coaster
  ._ _ _ _./          \				train made up of several cars and 
									associated with a track.

*/

#pragma once
#include <vector>
#include "Track.h"
#include "Car.h"


class Train {
protected:
	std::vector<Car *> itsCars;
	Track* itsTrack = nullptr;
	std::string name;
	//std::vector<Point2D> accelGraph;  // x is time, y is accel
	//std::vector<Point2D> velGraph;  // x is time, y is velocity
	float currTime;

	int selectedCarIndex = -1;   // 

public:
	Train() {}
	Train(std::ifstream& inputStream) {
		readFile(inputStream);
	}

	// destructor that deletes all the cars 
	~Train();

	// get the track that the train is on (needed?)
	Track* getTrack() { return itsTrack; }

	// set the train on a given track
	void setTrack(Track* aTrack) { itsTrack = aTrack; }

	// called by one of the class constructors.
	// reads train properties from file stream.
	void readFile(std::ifstream& inputStream);

	void advanceTrain(float timeStep);
	void initialize();

	bool addCarBack(Car* aCar) {
		if (!itsCars.empty())
			aCar->setAheadCar(itsCars.back());
		itsCars.push_back(aCar);
		relocateFollowingCars();
		return true;
	}
	bool addCarFront(Car* aCar) {
		if (!itsCars.empty())
			aCar->setBehindCar(itsCars.front());
		itsCars.insert(itsCars.begin(), aCar);
		initialize(); // could get fancier by keeping current simulation state
		return true;
	}

	// add the given car either ahead or behind the given index
	bool addCar(Car* aCar, int index, bool addAhead);

	bool deleteCar(int index) {
		if (0 < index && index <= itsCars.size()) {
			itsCars.erase(itsCars.begin() + index - 1);
			resetInterCarPointers();
			return true;
		}
		else
			return false;
	}

	int numbCars() {
		return itsCars.size();
	}

	// output all the data to console or to a filestream. 
	friend std::ostream& operator<<(std::ostream& os, const Train& aTrain);

	// display all cars on screen, at the correct locations
	void paint(Graphics^ g, int highlightCarIndex = -1);

	// returns the total length of the train
	float length();

	float getCurrLoc() {
		if (itsCars.size() > 0)
			return itsCars[0]->getCurrLoc();
		else
			return -1; // error
	}
	float getCurrVel() {
		if (itsCars.size() > 0)
			return itsCars[0]->getCurrVel();
		else
			return -INFINITY;   // error
	}
	void setFirstCarLoc(float loc) {
		if (itsCars.size() > 0) {
			itsCars[0]->setCurrLoc(loc);
			relocateFollowingCars();
		}
	}

	void setName(std::string newName) { name = newName; }
	std::string getName() { return name; }

	// returns a car index (first car is 1) if the given point is on one of the cars
	// returnd -1 in no car is at given point
	// similar to Path2D::getIndex()
	int getCar(Point2D checkPoint);

	//void loadCharts(System::Windows::Forms::DataVisualization::Charting::Series^ velSeries,
	//	System::Windows::Forms::DataVisualization::Charting::Series^ accelSeries);

	//void loadVelocityChart(System::Windows::Forms::DataVisualization::Charting::Series^ aSeries);



protected:
	// sets up the wheel locations of each of the trailing cars
	// using track geometry
	void relocateFollowingCars();
	
	// goes through car list and assures that the aheadCar and
	// behindCar pointers are correctly set
	void resetInterCarPointers();

};