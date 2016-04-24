#pragma once
#include <algorithm>
#include "Graph.h"
#include "DangerFunctions/ComputedDangerFunction.h"
#include "DangerFunctions/ActualDangerFunction.h"
#include "DangerFunctions/RLDangerFunction.h"
#include "DangerFunctions/RBF.h"

Graph::Graph(BWAPI::Game* g) {
	Broodwar = g;
	unit = NULL;
	// mapHeight() and mapWidth() in build tiles
	height = Broodwar->mapHeight() * 4 - 4;
	width = Broodwar->mapWidth() * 4 - 2;

	terrain = new Terrain(g, width, height);
	map = std::vector<Node*>();
	dangerFunctions = std::map<BWAPI::UnitType, DangerFunction*>();
	enemy = NULL;

	initNodes();
}

Graph::~Graph() {
	deleteNodes();

	// delete danger functions
	for (std::map<BWAPI::UnitType, DangerFunction*>::iterator itr = dangerFunctions.begin(); itr != dangerFunctions.end(); ++itr) {
		// save visualization to file
		itr->second->visualize(Const::PATH_DF + itr->first.toString() + ".dat", false);
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
			Node* node = new Node(std::pair<int, int>(w, h));
			if (terrain->isWalkable(w, h) == 0) {
				node->isWalkable= false;
			}
			map.push_back(node);
		}
	}

	// set neightbours for each node
	for (int h = 0; h < height; h++) {
		for (int w = 0; w < width; w++) {
			setNodeNeigbours(w, h);
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
		map[i]->resetNode();
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
	map[from]->setNeighbour(index, map[to]);
	map[from]->setTerrainCost(index, Utility::distance(map[from]->getPosition(), map[to]->getPosition()));
}

//FUNCTIONS

int Graph::getIndex(int w, int h) {
	return (h * width + w);
}

void  Graph::setUnitPointer(BWAPI::UnitInterface* u) {
	unit = u;
	unitType = unit->getType();
	resetWalkability();
}

void Graph::resetWalkability() {
	for (unsigned int i = 0; i < map.size(); i++) {
		map[i]->isWalkable = (unitType.isFlyer()) ? true : terrain->isWalkable(map[i]->getX(), map[i]->getY());
		map[i]->updated = false;
	}

	if (!unit->isFlying()) {
		for (unsigned int i = 0; i < map.size(); i++) {

			if (map[i]->isWalkable) {
				int unit_height = Utility::PositionToWalkPosition(unitType.height())/2 + 1;
				int unit_witdh = Utility::PositionToWalkPosition(unitType.width())/2 + 1;

				int pos_x_start = max(0, map[i]->getX() - unit_witdh);
				int pos_y_start = max(0, map[i]->getY() - unit_height);
				int pos_x_end = min(width - 1, map[i]->getX() + unit_witdh);
				int pos_y_end = min(height - 1, map[i]->getY() + unit_height);

				for (int x = pos_x_start; x <= pos_x_end; x++) {
					for (int y = pos_y_start; y <= pos_y_end; y++) {
						int index = getIndex(x, y);
						if (!map[index]->updated && !map[index]->isWalkable) {
							map[i]->isWalkable = false;
							map[i]->updated = true;
							break;
						}
					}
					if (map[i]->updated) break;
				}
			}
		}
	}
}

DangerFunction* Graph::getDangerFunction(BWAPI::UnitType enemyType) {
	if (dangerFunctions.find(enemyType) == dangerFunctions.end()) {
		string path = Const::PATH_FA + enemyType.toString() + ".txt";
		switch (Const::MODEL) {
			case 0: 
				dangerFunctions[enemyType] = new ComputedDangerFunction(enemyType);
				break;
			case 1:
				dangerFunctions[enemyType] = new ActualDangerFunction(enemyType,
					new RBF(Const::CENTERS, Const::ALPHA, Const::SIGMA, 0.0, Const::RADIUS, path));
				break;
			case 2:
				dangerFunctions[enemyType] = new RLDangerFunction(enemyType,
					new RBF(Const::CENTERS, Const::ALPHA, Const::SIGMA, 0.0, Const::RADIUS, path));
				break;
			default:
				dangerFunctions[enemyType] = new ComputedDangerFunction(enemyType);
		}
		dangerFunctions[enemyType]->setUnitPtr(unit);
	}
	return dangerFunctions.at(enemyType);
}

// learning for one enemy unit
void Graph::updateDangerFunctions() {
	// set enemy unit for training
	if (enemy == NULL) {
		BWAPI::Unitset units = Broodwar->getAllUnits();
		for (auto u = units.begin(); u != units.end(); ++u) {
			if ((*u)->getPlayer()->isEnemy(Broodwar->self())) {
				enemy = *u;	
				BWAPI::Position pos = enemy->getPosition();
				last_state = Utility::distance(unit->getPosition().x, unit->getPosition().y, pos.x, pos.y);
			}
		}
	}
	else {
		double new_state;
		if (enemy->isVisible()) {
			BWAPI::Position pos = enemy->getPosition();
			new_state = Utility::distance(unit->getPosition().x, unit->getPosition().y, pos.x, pos.y);
			getDangerFunction(enemy->getType())->learn(last_state, new_state);
		}
		else {
			new_state = last_state + unitType.topSpeed();
			getDangerFunction(enemy->getType())->learn(last_state, new_state);
			enemy = NULL;
		}
		last_state = new_state;
	}
}


void Graph::updateUnits() {
	BWAPI::Unitset units = Broodwar->getAllUnits();
	for (auto u = units.begin(); u != units.end(); ++u) {
		
		if ((*u) == unit) continue;

		if ((*u)->isFlying() == unit->isFlying()) {
			updateWalkability(*u);
			
		}
		if ((*u)->getPlayer()->isEnemy(Broodwar->self())) {
			updateDanger(*u); 
		}
	}
}

// disable Nodes under all units
void Graph::updateWalkability(BWAPI::UnitInterface* unit) {
	int pos_x_start = Utility::PositionToWalkPosition(unit->getLeft() - unitType.dimensionRight()) - 2;
	int pos_y_start = Utility::PositionToWalkPosition(unit->getTop() - unitType.dimensionDown() - 2);
	int pos_x_end = Utility::PositionToWalkPosition(unit->getRight() + unitType.dimensionLeft() + 2);
	int pos_y_end = Utility::PositionToWalkPosition(unit->getBottom() + unitType.dimensionUp() + 2);

	for (int x = pos_x_start; x < pos_x_end; x++) {
		for (int y = pos_y_start; y < pos_y_end; y++) {
			int index = getIndex(x, y);
			if (index >= 0 && index < map.size()) {
				map[index]->occupied = true;
			}
		}
	}
}

// update all nodes with danger
void Graph::updateDanger(BWAPI::UnitInterface* enemy) {		
	BWAPI::UnitType enemyType = enemy->getType();
	BWAPI::Position position = enemy->getPosition();

	int pos_x = Utility::PositionToWalkPosition(position.x);
	int pos_y = Utility::PositionToWalkPosition(position.y);
		
	int pos_x_start = max(0, pos_x - WALK_MAX_RANGE);
	int pos_y_start = max(0, pos_y - WALK_MAX_RANGE);
	int pos_x_end = min(width - 1, pos_x + WALK_MAX_RANGE);
	int pos_y_end = min(height - 1, pos_y + WALK_MAX_RANGE);

	// in walktiles
	for (int x = pos_x_start; x <= pos_x_end; x++) {
		for (int y = pos_y_start; y <= pos_y_end; y++) {
			int index = getIndex(x, y);
			if (existsNode(map[index]) && Const::MODEL != -1) {
				double dist = Utility::distance(x, y, pos_x, pos_y) * Const::WALK_TILE;

				double danger = getDangerFunction(enemyType)->compute(dist);
				if (danger < 0.00001) danger = 0;

				map[getIndex(x, y)]->addDangerCost(max(0.0, danger));
			}
		}
	}
}

bool Graph::existsNode(Node* node) {	
	if (node == NULL || !node->isWalkable || node->occupied) return false;
	return true;
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

	std::pair<int, int> start = std::pair<int, int>(Utility::PositionToWalkPosition(s.x), Utility::PositionToWalkPosition(s.y));
	std::pair<int, int> end = std::pair<int, int>(Utility::PositionToWalkPosition(e.x), Utility::PositionToWalkPosition(e.y));
 
	int start_index = getIndex(start.first, start.second);
	// check if exists a path

	if (start_index < 0 || start_index >= map.size() || 
		(!unitType.isFlyer() && !terrain->isReachable(start.first, start.second, end.first, end.second))) {
		Broodwar->sendText("no path");
		return std::vector<BWAPI::Position>();
	}

	// init start node
	Node* snode = map[start_index];	
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
			if (!existsNode(neighbour)) continue;

			double g_score = current->g + current->getCost(i, weight);

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