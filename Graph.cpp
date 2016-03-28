#pragma once
#include <algorithm>
#include "Graph.h"
#include "Const.h"
#include "DangerFunctions/ComputedDangerFunction.h"
#include "DangerFunctions/ActualDangerFunction.h"
#include "DangerFunctions/RLDangerFunction.h"
#include "FunctionApproximators/FunctionApproximator.h"
#include "FunctionApproximators/MLP.h"
#include "FunctionApproximators/RBF.h"

Graph::Graph(BWAPI::Game* g) {
	Broodwar = g;
	unit = NULL;
	// mapHeight() and mapWidth() in build tiles
	height = Broodwar->mapHeight() * 4 - 4;
	width = Broodwar->mapWidth() * 4;

	terrain = new Terrain(g, width, height);
	map = std::vector<Node*>();
	dangerFunctions = std::map<BWAPI::UnitType, DangerFunction*>();
	lastStates = std::map<BWAPI::UnitType, double>();

	initNodes();
}

Graph::~Graph() {
	deleteNodes();

	// delete danger functions
	for (std::map<BWAPI::UnitType, DangerFunction*>::iterator itr = dangerFunctions.begin(); itr != dangerFunctions.end(); ++itr) {
		itr->second->visualize(std::to_string(Const::MODEL) + "_" + itr->second->getEnemyType().toString() + ".dat", false);
		delete(itr->second);
		dangerFunctions.erase(itr);
	}
}

Terrain* Graph::getTerrain() {
	return terrain;
}

//NODES
std::vector<Node*> Graph::getNodes() {
	return map;
}

void Graph::initNodes() {
	// divide map into walk tiles
	for (int h = 0; h < height; h++) {
		for (int w = 0; w < width; w++) {
			if (terrain->getWalkabilityData(w, h) == 1) map.push_back(new Node(std::pair<int, int>(w, h)));
			else map.push_back(NULL);
		}
	}

	// set neightbours for each node
	for (int h = 0; h < height; h++) {
		for (int w = 0; w < width; w++) {
			if (map[getIndex(w,h)] != NULL) setNodeNeigbours(w, h);
		}
	}
}

void Graph::deleteNodes() {
	for (unsigned int i = 0; i < map.size(); i++) {
		delete(map[i]);
	}
}

void Graph::resetNodes() {
	for (unsigned int i = 0; i < map.size(); i++) {
		if (map[i] != NULL) map[i]->resetNode();
	}
	open_pos = std::map<std::pair<int, int>, Node*>();
	open_f = std::multimap<double, Node*>();
	closed = std::map <std::pair<int, int>, Node*>();
}

void Graph::setNodeNeigbours(int w, int h) {
	int current = getIndex(w, h);
	//top
	if (h > 0) {
		createEdge(current, 1, w, h - 1);
		//left
		if (w > 0) 
			createEdge(current, 0 , w - 1, h - 1);
		//right
		if (w < width - 1) 
			createEdge(current, 2, w + 1 , h - 1); 
	}
	//left
	if (w > 0) 
		createEdge(current, 7, w - 1, h);
	//right
	if (w < width - 1) 
		createEdge(current, 3, w + 1, h);
	//down
	if (h < height - 1) {
		createEdge(current, 5, w, h + 1);
		//left
		if (w > 0) 
			createEdge(current, 6, w - 1, h + 1); 
		// right
		if (w < width - 1) 
			createEdge(current, 4, w + 1, h + 1); 
	}
}

void Graph::createEdge(int from, int index, int w, int h) {
	int to = getIndex(w, h);
	if (map[to] != NULL) { 
		map[from]->setNeighbour(index, map[to]);
		map[from]->setTerrainCost(index, Utility::distance(map[from]->getPosition(), map[to]->getPosition()));
	}
}

//FUNCTIONS

int Graph::getIndex(int w, int h) {
	return (h * width + w);
}

DangerFunction* Graph::getDangerFunction(BWAPI::UnitType unitType) {
	if (dangerFunctions.find(unitType) == dangerFunctions.end()) {
		switch (Const::MODEL) {
			case 1:
				dangerFunctions[unitType] = new ActualDangerFunction(unitType,
					new RBF(1, 1, Const::CENTERS, Const::ALPHA, Const::SIGMA, 0.0, Const::RADIUS, Const::ADF_WRITE_PATH + BWAPI::UnitTypes::Zerg_Hydralisk.toString() + ".txt", true));
				break;
			case 2:
				dangerFunctions[unitType] = new RLDangerFunction(unitType,
					new RBF(1, 1, Const::CENTERS, Const::ALPHA, Const::SIGMA, 0.0, Const::RADIUS, Const::ADF_WRITE_PATH + BWAPI::UnitTypes::Zerg_Hydralisk.toString() + ".txt", true));
				break;
			default:
				dangerFunctions[unitType] = new ComputedDangerFunction(unitType);
		}
		dangerFunctions[unitType]->setUnitPtr(unit);
	}
	return dangerFunctions.at(unitType);
}

void  Graph::setUnitPointer(BWAPI::UnitInterface* u) {
	unit = u;
	unitType = unit->getType();
}

// update e(node, i) cost and return its value
double Graph::getNodeCost(Node* node, int i, double weight) {
	double danger = 0;
	Node* n = node->getNeighbour(i);
	/*if (!n->isUpdated()) {
		
		BWAPI::Unitset units = Broodwar->getUnitsInRadius(n->getX() * Const::WALK_TILE, n->getY() * Const::WALK_TILE, (int) Const::MAX_RANGE);
		// for each enemy compute danger for node

		//if (units.size() == 0) Broodwar->sendText("NOPE");

		for (auto enemy = units.begin(); enemy != units.end(); ++enemy) {
			if ((*enemy)->getPlayer()->isEnemy(Broodwar->self())) {
				BWAPI::Position pos = (*enemy)->getPosition();
				double dist = Utility::distance(n->getX() * Const::WALK_TILE, n->getY() * Const::WALK_TILE, pos.x, pos.y);
				danger += getDangerFunction((*enemy)->getType())->compute(dist);
			}
		}
		//Broodwar->sendText("update %d", (int) danger);
		if (danger < 0) danger = 0;
		n->setDangerCost(danger);
	}*/
	//Utility::printToFile(Const::PATH_DEBUG, std::to_string(node->getTerrainCost(i)) + (string)" " + std::to_string(node->getDangerCost(i)));
	return node->getCost(i, weight);
}

void Graph::updateDangerFunctions() {
	//learn for old states
	for (std::map<BWAPI::UnitType, double>::iterator iterator = lastStates.begin(); iterator != lastStates.end(); iterator++) {
		getDangerFunction(iterator->first)->learn(iterator->second);
		Broodwar->drawTextScreen(10, 80, "last: %f", iterator->second);

	}

	//add new states
	std::map<BWAPI::UnitType, double> newStates = std::map<BWAPI::UnitType, double>();
	BWAPI::Unitset units = Broodwar->getUnitsInRadius(unit->getPosition().x, unit->getPosition().y, (int) Const::MAX_RANGE);
	for (auto enemy = units.begin(); enemy != units.end(); ++enemy) {
		if ((*enemy)->getPlayer()->isEnemy(Broodwar->self())) {
			BWAPI::Position pos = (*enemy)->getPosition();
			double dist = Utility::distance(unit->getPosition().x, unit->getPosition().y, pos.x, pos.y);
			newStates[(*enemy)->getType()] = dist;
			Broodwar->drawTextScreen(10, 100, "new: %f", dist);
		}
	}
	lastStates = newStates;
}

// disable Nodes under all units
void Graph::updateUnits() {
	BWAPI::Unitset units = Broodwar->getAllUnits();

	for (auto u = units.begin(); u != units.end(); ++u) {

		if ((*u) == unit || ((*u)->isFlying() != unit->isFlying())) continue;

		for (int x = (*u)->getLeft() - unitType.dimensionRight(); x < (*u)->getRight() + unitType.dimensionLeft(); x++) {
			for (int y = (*u)->getTop() - unitType.dimensionDown(); y < (*u)->getBottom() + unitType.dimensionUp(); y++) {
				int index = getIndex(Utility::PositionToWalkPosition(x), Utility::PositionToWalkPosition(y));
				
				if (index >= 0 && index < map.size() && map[index] != NULL) {
					map[index]->setOccupied(true);
				}
			}
		}
	}
}

// update all nodes with danger
void Graph::updateDanger() {
	BWAPI::Unitset units = Broodwar->getAllUnits();

	int max_range = Utility::PositionToWalkPosition((int)Const::MAX_RANGE);
	Broodwar->printf("m %d", max_range);

	for (auto enemy = units.begin(); enemy != units.end(); ++enemy) {

		if (!(*enemy)->getPlayer()->isEnemy(Broodwar->self())) continue;

		
		BWAPI::UnitType enemyType = (*enemy)->getType();
		BWAPI::Position position = (*enemy)->getPosition();

		int pos_x = Utility::PositionToWalkPosition(position.x);
		int pos_y = Utility::PositionToWalkPosition(position.y);
		
		int pos_x_start = max(0, pos_x - max_range);
		int pos_y_start = max(0, pos_y - max_range);
		int pos_x_end = min(width - 1, pos_x + max_range + 1);
		int pos_y_end = min(height - 1, pos_y + max_range + 1);

		Broodwar->printf("e: %d", enemyType);
		Broodwar->printf("%d %d %d %d", pos_x, pos_y, pos_x * Const::WALK_TILE, pos_y * Const::WALK_TILE);
		Broodwar->printf("%d %d %d %d", pos_x_start * Const::WALK_TILE, pos_y_start * Const::WALK_TILE, 
			pos_x_end * Const::WALK_TILE, pos_y_end * Const::WALK_TILE);

		// in walktiles
		for (int x = pos_x_start; x < pos_x_end; x++) {
			for (int y = pos_y_start; y < pos_y_end; y++) {
				int index = getIndex(x, y);
				if (map[index] != NULL) {
					double dist = Utility::distance(x, y, pos_x, pos_y) * Const::WALK_TILE;
					map[getIndex(x, y)]->setDangerCost(max(0.0, getDangerFunction(enemyType)->compute(dist)));
				}
			}
		}
	}
}

//A*
std::vector<BWAPI::Position> Graph::getPath(Node* current) {
	std::vector<BWAPI::Position> result = std::vector<BWAPI::Position>();
	while (current != NULL && current->prev != NULL) {
		result.push_back(BWAPI::Position(current->getX() * Const::WALK_TILE, current->getY() * Const::WALK_TILE));
		current = current->prev;
	}
	return result;
}

std::vector<BWAPI::Position> Graph::AStar(BWAPI::Position s, BWAPI::Position e, double weight) {
	resetNodes();
	updateUnits();
	updateDanger();

	std::pair<int, int> start = std::pair<int, int>(Utility::PositionToWalkPosition(s.x), Utility::PositionToWalkPosition(s.y));
	std::pair<int, int> end = std::pair<int, int>(Utility::PositionToWalkPosition(e.x), Utility::PositionToWalkPosition(e.y));
 
	// check if exists a path
	if ((map[getIndex(end.first, end.second)] == NULL) // is walkable
		|| (Broodwar->getUnitsInRadius(e, Const::WALK_TILE).size() > 0) // is not occupied 
		|| !terrain->isReachable(start.first, start.second, end.first, end.second)) { // is reachable
		Broodwar->sendText("no path");
		return std::vector<BWAPI::Position>();
	}

	// init start node
	Node* snode = map[getIndex(start.first, start.second)];
	snode->g = 0;
	snode->f = Utility::distance(start, end);
	open_f.insert(std::pair<double, Node*>(snode->f, snode));
	open_pos.insert(std::pair<std::pair<int, int>, Node*>(start, snode));

	while (open_f.size() > 0) {

		Node* current = open_f.begin()->second;

		// if reached goal
		if (current->getX() == end.first  && current->getY() == end.second) {
			return getPath(current);
		}

		closed.insert(std::pair<std::pair<int, int>, Node*>(current->getPosition(), current));
		open_f.erase(open_f.begin());
		open_pos.erase(open_pos.find(current->getPosition()));

		for (int i = 0; i < 8; i++) {
			Node* neighbour = current->getNeighbour(i);
			if (neighbour == NULL) continue;

			double g_score = current->g + getNodeCost(current, i, weight);

			if (g_score < neighbour->g) {
				//Utility::printToFile(Const::PATH_DEBUG, std::to_string(getNodeCost(current, i, weight)));
				neighbour->prev = current;
				neighbour->g = g_score;
				double f_score = neighbour->g + Utility::distance(neighbour->getPosition(), end);

				std::map<std::pair<int, int>, Node*>::iterator isInOpen = open_pos.find(neighbour->getPosition());

				if (isInOpen == open_pos.end()) {
					open_pos.insert(std::pair<std::pair<int, int>, Node*>(neighbour->getPosition(), neighbour));
					open_f.insert(std::pair<double, Node*>(f_score, neighbour));
				}
				else {
					std::multimap<double, Node*>::iterator it = open_f.find(neighbour->f);
					for (; it != open_f.upper_bound(neighbour->f); it++) {
						if (it->second->getPosition() == neighbour->getPosition()) break;
					}
					open_f.erase(it);
					open_pos.insert(std::pair<std::pair<int, int>, Node*>(neighbour->getPosition(), neighbour));
					open_f.insert(std::pair<double, Node*>(f_score, neighbour));
				}
				neighbour->f = f_score;
			}
		}
	}
	return getPath(snode);
}

