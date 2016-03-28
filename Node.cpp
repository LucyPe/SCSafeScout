#pragma once
#include "Node.h"

Node::Node(std::pair<int, int> p) {
	neighbours = std::vector<Node*>(8, NULL);
	occupied = false;

	terrainCosts = std::vector<double>(8, INT_MAX);

	position = p;
	resetNode();
};

Node::~Node() {}

void Node::resetNode() {
	f = 0;
	g = INT_MAX;
	prev = NULL;
	dangerCost = 0;
	dangerUpdate = false;
	//dangerCosts = std::vector<double>(8, 0.0);
	//dangerUpdate = std::vector<bool>(8, false);
	setOccupied(false);
}

//POSITION
std::pair<int, int> Node::getPosition() {
	return position;
}

int Node::getX() {
	return position.first;
}

int Node::getY() {
	return position.second;
}

//STATUS
bool Node::isUpdated() {
	return  dangerUpdate;
}

bool Node::isOccupied() {
	return occupied;
}

void Node::setOccupied(bool b) {
	occupied = b;
}

//NEIGHBOURS
Node* Node::getNeighbour(int i) {
	Node* n = neighbours[i];
	if (n != NULL && n->isOccupied()) return NULL;
	return n;
}

void Node::setNeighbour(int i, Node* n) {
	neighbours[i] = n;
}

//COST
double Node::getCost(int i, double weight) {
	//return terrainCosts[i] + weight * dangerCosts[i];
	return terrainCosts[i] + weight * 20 * neighbours[i]->getDangerCost();
}

double Node::getDangerCost() {
	return dangerCost;
}

double Node::getTerrainCost(int i) {
	return terrainCosts[i];
}

void Node::setTerrainCost(int i, double d) {
	terrainCosts[i] = d;
}

void Node::setDangerCost(double d) {
	dangerCost += d;
	dangerUpdate = true;
}