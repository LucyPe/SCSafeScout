#pragma once
#include <vector> 

class Node {
private:
	// in walk tiles
	std::pair<int, int> position;

	std::vector<Node*> neighbours;
	std::vector<bool> disabled;

	std::vector<double> terrainCosts;
	std::vector<double> dangerCosts;
	std::vector<bool> dangerUpdate;

public:
	Node(std::pair<int, int>);
	~Node();

	std::pair<int, int> getPosition();
	int getX();
	int getY();

	Node* getNeighbour(int);
	void setNeighbour(int, Node*);
	void disableNeighbour(int);
	void resetNeighbours();

	void resetDangerCost();

	double getCost(int, double);
	double getTerrainCost(int);
	double getDangerCost(int);

	bool isUpdated(int);

	void setTerrainCost(int, double);
	void setDangerCost(int, double);
	//void setDanger(int);

	/* AStar */
	double f;
	double g;
	Node* prev;
};
