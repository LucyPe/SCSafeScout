#pragma once
#include <BWAPI.h>
#include <BWTA.h>
#include "Node.h"
#include "Utility.h"
#include "Terrain.h"
#include "DangerFunction.h"
#include "ComputedDangerFunction.h"
#include "ActualDangerFunction.h"

#include "FunctionApproximator.h"
#include "MLP.h"

class Graph {
private:
	BWAPI::Game* Broodwar;
	Terrain* terrain;
	std::map<BWAPI::UnitType, DangerFunction*> dangerFunctions;
	std::vector<Node*> map; // positions in walktiles
	int width;	// in walktiles x
	int height; // in walktiles y

	std::map<std::pair<int, int>, Node*> open_pos;
	std::multimap<double, Node*> open_f;
	std::map<std::pair<int, int>, Node*> closed;
	
	void initNodes();
	void deleteNodes();
	void resetNodes();
	void setNodeNeigbours(int, int);
	void createEdge(int, int, int, int);

	int getIndex(int, int);
	//double distance(std::pair<int, int>, std::pair<int, int>);
	//double distance(int, int, int, int);
	DangerFunction* getDangerFunction(BWAPI::UnitType);
	void updateDanger();
	double getNodeCost(Node*, int, BWAPI::UnitInterface*);
	//double danger(int);

	//double euklid(std::pair<int, int>, std::pair<int, int>);

	std::vector<BWAPI::Position> getPath(Node*);

public:
	Graph(BWAPI::Game*);
	~Graph();

	Terrain* getTerrain();

	std::vector<BWAPI::Position> AStar(BWAPI::Position, BWAPI::Position, BWAPI::UnitInterface*);
	std::vector<Node*> getNodes();

	void setUnit(BWAPI::UnitInterface*);
	void updateDangerFunctions(BWAPI::UnitInterface*);
	//void resetDanger();
	//void updateDanger(int, int, double);
};
