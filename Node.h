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
	double dangerCost;
	bool dangerUpdate;

	//std::vector<double> dangerCosts;
	//std::vector<bool> dangerUpdate;

public:
	Node(std::pair<int, int>);
	~Node();

	void resetNode();

	std::pair<int, int> getPosition();
	int getX();
	int getY();

	bool isUpdated();
	bool isOccupied();
	void setOccupied(bool);

	Node* getNeighbour(int);
	void setNeighbour(int, Node*);

	double getCost(int, double);
	double getTerrainCost(int);
	double getDangerCost();

	void setTerrainCost(int, double);
	void setDangerCost(double);

	/* AStar vars */
	double f;
	double g;
	Node* prev;
};
