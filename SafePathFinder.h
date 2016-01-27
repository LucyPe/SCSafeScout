#pragma once
#include <BWAPI.h>
#include "Utility.h"
#include "Graph.h"

class SafePathFinder {
private:
	BWAPI::Game* Broodwar;
	Graph* map;
	std::vector<BWAPI::Position> path;

	bool existPath();
	bool findPath(BWAPI::Position, BWAPI::Position, BWAPI::UnitInterface*);
	BWAPI::Position nextPosition();
	
public:
	SafePathFinder(BWAPI::Game*);
	~SafePathFinder();

	void changePosition(BWAPI::Position, BWAPI::UnitInterface*);
	void moveUnit(BWAPI::UnitInterface*, BWAPI::Position, int frame);

	void showGrid();
	void showPath();
	void showPolygons();
	void drawTerrainData();
	void drawEnemiesAttackRange();
};


