#pragma once
#include "Terrain.h"
#include "Const.h"

Terrain::Terrain(BWAPI::Game* game, int w, int h) {
	Broodwar = game;
	height = h;
	width = w;

	walkability = std::vector<bool>();

	if (!readTerrainData()) {

		unwalkablePolygons = std::vector<BWTA::Polygon>();
		for (BWTA::Polygon* p : BWTA::getUnwalkablePolygons()) unwalkablePolygons.push_back(*p);

		regionsPolygons = std::vector<BWTA::Polygon>();
		for (std::set<BWTA::Region*>::const_iterator r = BWTA::getRegions().begin(); r != BWTA::getRegions().end(); r++) {
			regionsPolygons.push_back((*r)->getPolygon());
		}
		saveTerrainData();
	}
}

bool Terrain::readTerrainData() {
	std::ifstream file;
	file.open(Const::PATH_MAP + Broodwar->mapHash(), std::ofstream::in);
	if (file.is_open()) {
		bool input;
		for (int i = 0; i < height * width; i++) {
			file >> input;
			walkability.push_back(input);
		}

		file.close();
		return true;
	}
	return false;
}

void Terrain::saveTerrainData() {
	std::ofstream file;
	file.open(Const::PATH_MAP + Broodwar->mapHash(), std::ofstream::out);
	if (file.is_open()) {
		for (int h = 0; h < height; h++) {
			for (int w = 0; w < width; w++) {
				if (walkable(w, h)) walkability.push_back(1);
				else walkability.push_back(0);
				file << walkability.at(walkability.size() - 1) << " ";
			}
		}
	}
}

bool Terrain::pointInPolygon(int walk_x, int walk_y, BWTA::Polygon* polygon) {
	for (int x = walk_x * Const::WALK_TILE; x <= (walk_x * Const::WALK_TILE + Const::WALK_TILE); x += Const::WALK_TILE) {
		for (int y = walk_y * Const::WALK_TILE; y <= (walk_y * Const::WALK_TILE + Const::WALK_TILE); y += Const::WALK_TILE) {
				
			int i, j, nvert = polygon->size();
			bool c = false;

			for (i = 0, j = nvert - 1; i < nvert; j = i++) {
				BWAPI::Position pos_i = polygon->at(i) * Const::WALK_TILE;
				BWAPI::Position pos_j = polygon->at(j) * Const::WALK_TILE;

				if (((pos_i.y > y) != (pos_j.y > y)) &&
					(x < (pos_j.x - pos_i.x) * (y - pos_i.y) / (pos_j.y - pos_i.y) + pos_i.x)) {
					c = !c;
				}	
			}
			if (c) return true;
		}
	}
	return false;
}

bool Terrain::walkable(int x, int y) {
	for (BWTA::Polygon pol : unwalkablePolygons) {
		if (pointInPolygon(x, y, &pol)) {
			for (BWTA::Polygon pol : regionsPolygons) {
				if (pointInPolygon(x * Const::WALK_TILE, y * Const::WALK_TILE, &pol)) return false;
			}
			return false;
		}
	}
	return true;
}

bool Terrain::isWalkable(int x, int y) {
	return walkability[y * width + x];
}

bool Terrain::isReachable(int sx, int sy, int ex, int ey) {
	if (isWalkable(sx, sy) && (isWalkable(ex, ey))
		/*&& (BWTA::isConnected(Utility::WalkToBuildPosition(sx),
			Utility::WalkToBuildPosition(sy),
			Utility::WalkToBuildPosition(ex),
			Utility::WalkToBuildPosition(ey)))*/) return true;
	return false;
}

/* draw regions (Green) and unwalkablePolygons(Orange) */
void Terrain::drawPolygons() {
	for (unsigned int p = 0; p < regionsPolygons.size(); p++) {
		BWTA::Polygon pol = regionsPolygons.at(p);
		for (int j = 0; j< (int)pol.size(); j++) {
			Broodwar->drawLineMap(pol.at(j) * 8, pol.at((j + 1) % pol.size()) * 8, BWAPI::Colors::Green);
		}
	}

	for (unsigned int p = 0; p < unwalkablePolygons.size(); p++) {
		BWTA::Polygon pol = unwalkablePolygons.at(p);
		for (int j = 0; j< (int)pol.size(); j++) {
			Broodwar->drawLineMap(pol.at(j) * 8, pol.at((j + 1) % pol.size()) * 8, BWAPI::Colors::Orange);
		}
	}
}