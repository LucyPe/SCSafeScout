#pragma once
#include <BWAPI.h>
#include <BWTA.h>
#include "Utility.h"
#include <fstream>

class Terrain {
private:
	BWAPI::Game* Broodwar;
	std::vector<bool> walkability;
	int height;
	int width;

	std::vector<BWTA::Polygon> unwalkablePolygons;
	std::vector<BWTA::Polygon> regionsPolygons;
	
	bool readTerrainData();
	void saveTerrainData();

	bool pointInPolygon(int, int, BWTA::Polygon*);
	bool walkable(int, int);

public:
	Terrain(BWAPI::Game*, int, int);
	~Terrain();

	bool isWalkable(int, int);
	bool isReachable(int, int, int, int);

	void drawPolygons();
};

