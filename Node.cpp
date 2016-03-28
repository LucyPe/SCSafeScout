#pragma once
#include "Node.h"

Node::Node(std::pair<int, int> p) {
	neighbours = std::vector<Node*>(8, NULL);
	terrainCosts = std::vector<double>(8, INT_MAX);
	walkable = true;
	updated = false;
	position = p;
	resetNode();
};

Node::~Node() {}

void Node::resetNode() {
	f = 0;
	g = INT_MAX;
	prev = NULL;
	dangerCost = 0;
	occupied = false;
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

//NEIGHBOURS
Node* Node::getNeighbour(int i) {
	return neighbours[i];
}

void Node::setNeighbour(int i, Node* n) {
	neighbours[i] = n;
}

//COST
double Node::getCost(int i, double weight) {
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

void Node::addDangerCost(double d) {
	dangerCost += d;
}