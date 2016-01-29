#pragma once
#include "Node.h"

Node::Node(std::pair<int, int> p) {
	neighbours = std::vector<Node*>(8, NULL);
	disabled = std::vector<bool>(8, false);

	terrainCosts = std::vector<double>(8, INT_MAX);
	resetDangerCost();

	position = p;
	f = 0;
	g = INT_MAX;
	prev = NULL;
};

Node::~Node() {}

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
	if (disabled[i]) return NULL;
	return neighbours[i];
}

void Node::setNeighbour(int i, Node* n) {
	neighbours[i] = n;
}

void Node::disableNeighbour(int i) {
	disabled[i] = true;
}

void Node::resetNeighbours() {
	disabled = std::vector<bool>(8, false);
}

//COST
void Node::resetDangerCost() {
	dangerCosts = std::vector<double>(8, 0.0);
	dangerUpdate = std::vector<bool>(8, false);
};

double Node::getCost(int i, double weight) {
	return terrainCosts[i] + weight * dangerCosts[i];
}

double Node::getDangerCost(int i) {
	return dangerCosts[i];
}

double Node::getTerrainCost(int i) {
	return terrainCosts[i];
}

void Node::setTerrainCost(int i, double d) {
	terrainCosts[i] = d;
}

void Node::setDangerCost(int i, double d) {
	dangerCosts[i] = d;
	dangerUpdate[i] = true;
}

bool Node::isUpdated(int i) {
	return dangerUpdate[i];
}

/*
void Node::setDanger(int danger) {
	for (int i = 0; i < 8; i++) {
		Node* neighbour = getNeighbour(i);
		if (neighbour != NULL) {
			neighbour->updateDangerCost((i + 4) % 8, danger);
		}
	}
}*/


