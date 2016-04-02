#pragma once
#include <vector> 

class Node {
private:
	// in walk tiles
	std::pair<int, int> position;
	std::vector<Node*> neighbours;

	std::vector<double> terrainCosts;
	double dangerCost;

public:
	Node(std::pair<int, int>);
	~Node();

	// if there is unit or building
	bool occupied;
	bool isWalkable;
	bool updated;

	void resetNode();

	std::pair<int, int> getPosition();
	int getX();
	int getY();

	Node* getNeighbour(int);
	void setNeighbour(int, Node*);

	double getCost(int, double);
	double getTerrainCost(int);
	double getDangerCost();

	void setTerrainCost(int, double);
	void addDangerCost(double);

	/* AStar vars */
	double f;
	double g;
	Node* prev;
};
