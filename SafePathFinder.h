#pragma once
#include <BWAPI.h>
#include "Utility.h"
#include "Graph.h"

class SafePathFinder {
private:
	BWAPI::Game* Broodwar;
	Graph* map;
	std::vector<BWAPI::Position> path;

	BWAPI::UnitInterface* unit;
	double dangerWeight;

	bool existPath();
	bool findPath(BWAPI::Position, BWAPI::Position);
	BWAPI::Position nextPosition();
	
public:
	SafePathFinder(BWAPI::Game*);
	~SafePathFinder();

	void setUnit(BWAPI::UnitInterface*, double);

	void changePosition(BWAPI::Position);
	bool moveUnit(BWAPI::Position, int);

	void showGrid();
	void showPath();
	void showPolygons();
	void drawTerrainData();
	void drawEnemiesAttackRange();
};


