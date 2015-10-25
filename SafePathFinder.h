#pragma once
#include <BWAPI.h>
#include "Utility.h"
#include "Graph.h"


class SafePathFinder {
private:
	BWAPI::Game* Broodwar;
	Graph* map;
	std::vector<BWAPI::Position> path;
	
public:
	SafePathFinder(BWAPI::Game*);
	~SafePathFinder();
	
	void update(BWAPI::UnitInterface*);
	bool existPath(); 
	int pathSize();
	bool findPath(BWAPI::Position, BWAPI::Position, BWAPI::UnitInterface*);

	BWAPI::Position getNextPosition();
	BWAPI::Position getLastPosition();
	BWAPI::Position nextPosition();
	BWAPI::Position randomPosition();

	void showGrid();
	void showPath();
	void showPolygons();
	void drawTerrainData();
	void drawEnemiesAttackRange();
};


