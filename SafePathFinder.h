#pragma once
#include <BWAPI.h>
#include "Utility.h"
#pragma once
#include "Graph.h"

class SafePathFinder {
private:
	BWAPI::Game* Broodwar;
	Graph* map;
	std::vector<BWAPI::Position> path;

	// learning for Neural Networks
	void update(BWAPI::UnitInterface*);

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


