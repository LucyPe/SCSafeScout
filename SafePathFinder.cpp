#pragma once
#include "SafePathFinder.h"
#include "Const.h"

SafePathFinder::SafePathFinder(BWAPI::Game* g) {
	Broodwar = g;
	path = std::vector<BWAPI::Position>();
	map = new Graph(g);
}

SafePathFinder::~SafePathFinder() {
	delete(map);
}

bool SafePathFinder::existPath() {
	return path.size() != 0;
}

BWAPI::Position SafePathFinder::nextPosition() {
	return path[path.size() - 1];
}

bool SafePathFinder::findPath(BWAPI::Position start, BWAPI::Position end, BWAPI::UnitInterface* unit) {
	map->setUnit(unit);
	path = map->AStar(start, end, unit);

	// smoothness hack
	if (path.size() > 7) {
		for (int i = 0; i < 5; i++) {
			path.pop_back();
		}
	}
	return path.size() != 0;
}

void SafePathFinder::changePosition(BWAPI::Position position, BWAPI::UnitInterface* unit) {
	//Broodwar->sendText("pos %d, %d", position.x, position.y);
	findPath(unit->getPosition(), position, unit);
}


void SafePathFinder::moveUnit(BWAPI::UnitInterface* unit, BWAPI::Position position, int frame) {
	BWAPI::Unitset enemies = unit->getUnitsInRadius(Const::MAX_RANGE);

	if (existPath()) {
		unit->move(nextPosition());
		if (Utility::PositionInRange(nextPosition(), unit->getPosition(), Const::WALK_TILE * 10)) {
			path.pop_back();

			// update FA
			if (Const::LEARNING && (frame % 50 == 0)) map->updateDangerFunctions(unit);

			// if enemy is near -> recalculate path
			if (!enemies.empty())  {
				findPath(unit->getPosition(), position, unit);
			}
		}
		if (Const::LEARNING && (path.size() <= 10)) Broodwar->restartGame();
	}
	else {
		if (!findPath(unit->getPosition(), position, unit)) {
			changePosition(Utility::getRandomPosition(Broodwar->mapWidth(), Broodwar->mapHeight()),unit);
		}
	}
}

void SafePathFinder::showGrid() {
	std::vector<Node*> nodes = map->getNodes();

	for (unsigned int i = 0; i < nodes.size(); i++) {
		//if (map->terrain->isWalkable(nodes[i]->getX(), nodes[i]->getY())) {
		if (nodes[i] != NULL) {
			Broodwar->drawBoxMap(nodes[i]->getX() * Const::WALK_TILE, nodes[i]->getY() * Const::WALK_TILE, nodes[i]->getX() * Const::WALK_TILE + Const::WALK_TILE, nodes[i]->getY() * Const::WALK_TILE + Const::WALK_TILE, BWAPI::Color(255, 255, 255), false);
			for (unsigned int n = 0; n < 8; n++) {
				//if (nodes[i]->getNeighbour(n) == NULL)
				if (nodes[i]->getDangerCost(n) > 0)
					Broodwar->drawCircleMap(nodes[i]->getX() * Const::WALK_TILE + n, nodes[i]->getY() * Const::WALK_TILE + 3, 1, BWAPI::Color(0, 255 - 200 * n, 10 * n), true);
			}
		}
		/*else {
			if (!Broodwar->isWalkable(nodes[i]->getX(), nodes[i]->getY())) 
				Broodwar->drawBoxMap(nodes[i]->getX() * WALK_TILE, nodes[i]->getY() * WALK_TILE, nodes[i]->getX() * WALK_TILE + WALK_TILE, nodes[i]->getY() * WALK_TILE + WALK_TILE, BWAPI::Color(0, 0, 255), false);
		 else 
				Broodwar->drawBoxMap(nodes[i]->getX() * WALK_TILE, nodes[i]->getY() * WALK_TILE, nodes[i]->getX() * WALK_TILE + WALK_TILE, nodes[i]->getY() * WALK_TILE + WALK_TILE, BWAPI::Color(255, 0, 0), false);
		}*/
	}
}

void SafePathFinder::showPath() {
	for (BWAPI::Position p : path) {
		Broodwar->drawCircleMap(p.x, p.y, 3, BWAPI::Color(255, 0, 0), true);
	}
	Broodwar->drawCircleMap(200, 150, 3, BWAPI::Color(0, 255, 0), true);
}

void SafePathFinder::showPolygons() {
	map->getTerrain()->drawPolygons();
}

void SafePathFinder::drawEnemiesAttackRange() {
	BWAPI::Playerset players = Broodwar->enemies();
	for (auto player = players.begin(); player != players.end(); ++player) {
		BWAPI::Unitset units = (*player)->getUnits();

		for (auto enemy = units.begin(); enemy != units.end(); ++enemy) {
			BWAPI::WeaponType weapon = (*enemy)->getType().groundWeapon();
			int weaponRange = (int) (weapon.maxRange() + fmax((*enemy)->getType().height(), (*enemy)->getType().width()));

			BWAPI::Position position = (*enemy)->getPosition();
			Broodwar->drawCircleMap(position.x, position.y, weaponRange, BWAPI::Colors::Green, false);
			for (int x = ((position.x - weaponRange) / Const::WALK_TILE); x <= ((position.x + weaponRange) / Const::WALK_TILE); x++) {
				for (int y = ((position.y - weaponRange) / Const::WALK_TILE); y <= ((position.y + weaponRange) / Const::WALK_TILE); y++) {
					if ((Utility::distance(position.x, position.y, x * Const::WALK_TILE, y * Const::WALK_TILE)) <= weaponRange) {
						Broodwar->drawBoxMap(x * Const::WALK_TILE, y * Const::WALK_TILE, x * Const::WALK_TILE + Const::WALK_TILE, y * Const::WALK_TILE + Const::WALK_TILE, BWAPI::Color(255, 0, 0), false);
					}
				}
			}
		}
	}
}

void SafePathFinder::drawTerrainData() {
	//we will iterate through all the base locations, and draw their outlines.
	for (std::set<BWTA::BaseLocation*>::const_iterator i = BWTA::getBaseLocations().begin(); i != BWTA::getBaseLocations().end(); i++) {
		BWAPI::TilePosition p = (*i)->getTilePosition();
		BWAPI::Position c = (*i)->getPosition();

		//draw outline of center location
		Broodwar->drawBoxMap(p.x * 32, p.y * 32, p.x * 32 + 4 * 32, p.y * 32 + 3 * 32, BWAPI::Colors::Blue);

		//draw a circle at each mineral patch
		for (BWAPI::Unitset::iterator j = (*i)->getStaticMinerals().begin(); j != (*i)->getStaticMinerals().end(); j++)
		{
			BWAPI::Position q = (*j)->getInitialPosition();
			Broodwar->drawCircleMap(q.x, q.y, 30, BWAPI::Colors::Cyan);
		}

		//draw the outlines of vespene geysers
		for (BWAPI::Unitset::iterator j = (*i)->getGeysers().begin(); j != (*i)->getGeysers().end(); j++)
		{
			BWAPI::TilePosition q = (*j)->getInitialTilePosition();
			Broodwar->drawBoxMap(q.x * 32, q.y * 32, q.x * 32 + 4 * 32, q.y * 32 + 2 * 32, BWAPI::Colors::Orange);
		}

		//if this is an island expansion, draw a yellow circle around the base location
		if ((*i)->isIsland())
			Broodwar->drawCircleMap(c, 80, BWAPI::Colors::Yellow);
	}

	//we will iterate through all the regions and draw the polygon outline of it in green.
	for (std::set<BWTA::Region*>::const_iterator r = BWTA::getRegions().begin(); r != BWTA::getRegions().end(); r++)
	{
		Broodwar->drawCircleMap((*r)->getCenter(), 3, BWAPI::Colors::Purple, true);
		BWTA::Polygon p = (*r)->getPolygon();
		for (int j = 0; j<(int)p.size(); j++)
		{
			BWAPI::Position point1 = p[j];
			BWAPI::Position point2 = p[(j + 1) % p.size()];
			Broodwar->drawLineMap(point1, point2, BWAPI::Colors::Green);
		}
	}

	//we will visualize the chokepoints with red lines
	for (std::set<BWTA::Region*>::const_iterator r = BWTA::getRegions().begin(); r != BWTA::getRegions().end(); r++)
	{
		for (std::set<BWTA::Chokepoint*>::const_iterator c = (*r)->getChokepoints().begin(); c != (*r)->getChokepoints().end(); c++)
		{
			Broodwar->drawCircleMap((*c)->getCenter(), 3 , BWAPI::Colors::Yellow,true);
			BWAPI::Position point1 = (*c)->getSides().first;
			BWAPI::Position point2 = (*c)->getSides().second;
			Broodwar->drawLineMap(point1, point2, BWAPI::Colors::Red);
		}
	}
}