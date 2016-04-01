#pragma once
#include <BWAPI.h>
#include <BWTA.h>
#include "Node.h"
#include "Terrain.h"
#include "Utility.h"
#include "Const.h"
#include "DangerFunctions/DangerFunction.h"

class Graph {
private:
	BWAPI::Game* Broodwar;
	Terrain* terrain;
	BWAPI::UnitInterface* unit;
	BWAPI::UnitType unitType;

	std::map<BWAPI::UnitType, DangerFunction*> dangerFunctions;
	std::vector<Node*> map; // positions in walktiles
	int width;	// in walktiles x
	int height; // in walktiles y

	int const WALK_MAX_RANGE = Utility::PositionToWalkPosition((int)Const::MAX_RANGE);

	std::map<std::pair<int, int>, Node*> open_pos;
	std::multimap<double, Node*> open_f;
	std::map<std::pair<int, int>, Node*> closed;
	
	std::map<BWAPI::UnitInterface*, double> lastStates;

	void initNodes();
	void deleteNodes();
	void resetNodes();
	void setNodeNeigbours(int, int);
	void createEdge(int, int, int, int);

	void resetWalkability();

	int getIndex(int, int);

	DangerFunction* getDangerFunction(BWAPI::UnitType);

	void updateUnits();
	void updateDanger(BWAPI::UnitInterface*);
	void updateWalkability(BWAPI::UnitInterface*);

	std::vector<BWAPI::Position> getPath(Node*);

public:
	Graph(BWAPI::Game*);
	~Graph();

	bool existsNode(Node*);
	Terrain* getTerrain();

	std::vector<BWAPI::Position> AStar(BWAPI::Position, BWAPI::Position, double);
	std::vector<Node*> getNodes();

	void setUnitPointer(BWAPI::UnitInterface*);
	void updateDangerFunctions();
};
