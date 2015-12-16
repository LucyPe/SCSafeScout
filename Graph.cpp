#include "Graph.h"

Graph::Graph(BWAPI::Game* g) {
	Broodwar = g;
	// mapHeight() and mapWidth() in build tiles
	height = Broodwar->mapHeight() * 4 - 4;
	width = Broodwar->mapWidth() * 4;

	terrain = new Terrain(g, width, height);
	map = std::vector<Node*>();
	dangerFunctions = std::map<BWAPI::UnitType, DangerFunction*>();
	
	initNodes();

	/* Functions*/

	//dangerFunctions[BWAPI::UnitTypes::Zerg_Drone] = new ComputedDangerFunction(BWAPI::UnitTypes::Zerg_Drone);
	/*
	dangerFunctions[BWAPI::UnitTypes::Zerg_Mutalisk] = new ActualDangerFunction(
		BWAPI::UnitTypes::Zerg_Mutalisk,
		new MLP(1, 1, 5, 0.2, "bwapi-data/write/ADF_" + BWAPI::UnitTypes::Zerg_Mutalisk.toString() + ".txt"));
	*/

	dangerFunctions[BWAPI::UnitTypes::Zerg_Drone] = new ActualDangerFunction(
		BWAPI::UnitTypes::Zerg_Drone,
		new MLP(1, 1, 5, 0.2, "bwapi-data/write/ADF_" + BWAPI::UnitTypes::Zerg_Drone.toString() + ".txt"));


	dangerFunctions[BWAPI::UnitTypes::Zerg_Hydralisk] = new ActualDangerFunction(
		BWAPI::UnitTypes::Zerg_Hydralisk,
		new MLP(1, 1, 5, 0.2, "bwapi-data/write/ADF_" + BWAPI::UnitTypes::Zerg_Hydralisk.toString() + ".txt"));
	

	/*dangerFunctions[BWAPI::UnitTypes::Zerg_Hydralisk] = new ActualDangerFunction(
	BWAPI::UnitTypes::Zerg_Hydralisk,
	new RBF(1, 1, 20, 0.1, 15, "ADF_" + BWAPI::UnitTypes::Zerg_Hydralisk.toString() + ".txt"));*/
}

Graph::~Graph() {
	deleteNodes();

	for (std::map<BWAPI::UnitType, DangerFunction*>::iterator itr = dangerFunctions.begin(); itr != dangerFunctions.end(); ++itr) {
		itr->second->visualize("Function_" + itr->second->getEnemyType().toString() + ".dat");
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
		if (map[i] != NULL) {
			map[i]->f = 0;
			map[i]->g = INT_MAX;
			map[i]->prev = NULL;
			map[i]->resetDangerCost();
		}
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
		dangerFunctions[unitType] = new ComputedDangerFunction(unitType);
	}
	return dangerFunctions.at(unitType);
}

void  Graph::setUnit(BWAPI::UnitInterface* unit) {
	for (std::map<BWAPI::UnitType, DangerFunction*>::iterator itr = dangerFunctions.begin(); itr != dangerFunctions.end(); ++itr) {
		itr->second->setUnitPtr(unit);
	}
}

/*
double Graph::danger(int dmg) {
	return dmg;
}*/

/*
double Graph::euklid(std::pair<int, int> start, std::pair<int, int> end) {
	return sqrt(pow(end.first - start.first, 2.0) + pow(end.second - start.second, 2.0));
}
*/

//DANGER
/*
void Graph::resetDanger() {
	for (unsigned int i = 0; i < map.size(); i++) {
		if (map[i] != NULL) map[i]->resetDangerCost();
	}
}*/
/*
void Graph::updateDanger(int x, int y, double damage) {
	int i = getIndex(x, y);
	if (map[i] != NULL) map[i]->setDanger(danger(damage));
}*/

double Graph::getNodeCost(Node* node, int i, BWAPI::UnitInterface* unit) {
	double danger = 0;
	//double max = unit->getType().maxHitPoints() + unit->getType().maxShields();
	if (!node->isUpdated(i)) {
		
		Node* n = node->getNeighbour(i);
		//Broodwar->sendText("update %d", i);
		BWAPI::Unitset units = Broodwar->getUnitsInRadius(n->getX() * WALK_TILE, n->getY() * WALK_TILE, 8 * BUILD_TILE);
		for (auto enemy = units.begin(); enemy != units.end(); ++enemy) {
			if ((*enemy)->getPlayer()->isEnemy(Broodwar->self())) {
				BWAPI::Position pos = (*enemy)->getPosition();
				double dist = Utility::distance(n->getX() * WALK_TILE, n->getY() * WALK_TILE, pos.x, pos.y);
				danger += getDangerFunction((*enemy)->getType())->compute(dist);
			}
		}
		//Broodwar->sendText("update %d", (int) danger);
		if (danger < 0) danger = 0;
		node->setDangerCost(i, danger * 40);
	}
	return node->getCost(i);
}


void Graph::updateDangerFunctions(BWAPI::UnitInterface* unit) {
	BWAPI::Unitset units = Broodwar->getUnitsInRadius(unit->getPosition().x, unit->getPosition().y, 400);
	for (auto enemy = units.begin(); enemy != units.end(); ++enemy) {
		if ((*enemy)->getPlayer()->isEnemy(Broodwar->self())) {
			BWAPI::Position pos = (*enemy)->getPosition();
			double dist = Utility::distance(unit->getPosition().x, unit->getPosition().y, pos.x, pos.y);
			getDangerFunction((*enemy)->getType())->learn(dist);
		}
	}
}


//A*
std::vector<BWAPI::Position> Graph::getPath(Node* current) {
	std::vector<BWAPI::Position> result = std::vector<BWAPI::Position>();
	while (current != NULL && current->prev != NULL) {
		result.push_back(BWAPI::Position(current->getX() * WALK_TILE, current->getY() * WALK_TILE));
		current = current->prev;
	}
	resetNodes();
	return result;
}

std::vector<BWAPI::Position> Graph::AStar(BWAPI::Position s, BWAPI::Position e, BWAPI::UnitInterface* unit) {
	std::pair<int, int> start = std::pair<int, int>(Utility::PositionToWalkPosition(s.x), Utility::PositionToWalkPosition(s.y));
	std::pair<int, int> end = std::pair<int, int>(Utility::PositionToWalkPosition(e.x), Utility::PositionToWalkPosition(e.y));

	int sindex = start.second * width + start.first;
	Node* snode = map[sindex];

	// check if exists a path
	if (!terrain->isReachable(start.first, start.second, end.first, end.second)) {
		Broodwar->sendText("no path");
		return getPath(snode);
	}

	// init start node
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
			double g_score = current->g + getNodeCost(current, i, unit);

			if (g_score < neighbour->g) {
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

