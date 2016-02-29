#pragma once
#include <vector> 

class Node {
private:
	// in walk tiles
	std::pair<int, int> position;
	
	// if there is unit or building
	bool occupied;

	std::vector<Node*> neighbours;

	std::vector<double> terrainCosts;
	std::vector<double> dangerCosts;
	std::vector<bool> dangerUpdate;

public:
	Node(std::pair<int, int>);
	~Node();

	void resetNode();

	std::pair<int, int> getPosition();
	int getX();
	int getY();

	bool isUpdated(int);
	bool isOccupied();
	void setOccupied(bool);

	Node* getNeighbour(int);
	void setNeighbour(int, Node*);

	double getCost(int, double);
	double getTerrainCost(int);
	double getDangerCost(int);

	void setTerrainCost(int, double);
	void setDangerCost(int, double);

	/* AStar vars */
	double f;
	double g;
	Node* prev;
};
