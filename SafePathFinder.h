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

	void loadParams();
	void saveParams();

	bool existPath();
	bool findPath(BWAPI::Position, BWAPI::Position);
	BWAPI::Position nextPosition();

	void showGrid();
	void showPath();
	void drawTerrainData();
	void drawEnemiesAttackRange();
	
public:
	SafePathFinder(BWAPI::Game*);
	~SafePathFinder();

	/* params */
	double dangerWeight = -1;
	int POSITIONS = 1;

	/* Gui Params */
	bool GRID = false;
	bool PATH = true;
	bool MOVE = true;
	bool NO_GUI = false;
	bool TERRAIN_DATA = false;
	bool ENEMY_RANGE = true;

	/* Model Params */
	bool LEARNING = true;

	/**/

	void setUnit(BWAPI::UnitInterface*);
	int getUnitHP();
	BWAPI::Position getUnitPosition();

	int pathLenght();
	void changePosition(BWAPI::Position);
	bool moveUnit(BWAPI::Position);

	void visualizeData();
};


