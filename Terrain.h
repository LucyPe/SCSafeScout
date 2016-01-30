#pragma once
#include <BWAPI.h>
#include <BWTA.h>
#include "Utility.h"
#include <fstream>

class Terrain {
private:
	BWAPI::Game* Broodwar;
	std::vector<int> walkability;
	int height;
	int width;

	std::vector<BWTA::Polygon> unwalkablePolygons;
	std::vector<BWTA::Polygon> regionsPolygons;

	void setWalkabilityData();
	bool readTerrainData();

public:
	Terrain(BWAPI::Game*, int, int);
	~Terrain();
	bool pointInPolygon(int, int, BWTA::Polygon*);

	int getWalkabilityData(int, int);
	bool isWalkable(int, int);
	bool isReachable(int, int, int, int);

	void drawPolygons();

};

