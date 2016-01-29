#pragma once
#include "Utility.h"
#include "Const.h"
#include "cmath"

BWAPI::Position Utility::getWalkTile(BWAPI::Position pos) {
	return getWalkTile(pos.x, pos.y);
}

BWAPI::Position Utility::getWalkTile(int x, int y) {
	return BWAPI::Position(x / Const::WALK_TILE, y / Const::WALK_TILE);
}

bool Utility::PositionInRange(int sx, int sy, int ex, int ey, int range) {
	if ((sx >= (ex - range)) && (sx <= (ex + range)) && (sy >= (ey - range)) && (sy <= (ey + range))) {
		return true;
	}
	return false;
}

bool Utility::PositionInRange(BWAPI::Position s, BWAPI::Position e, int range) {
	return PositionInRange(s.x, s.y, e.x, e.y, range);
}

int Utility::PositionToWalkPosition(int x) {
	return (int)floor(((double)x / Const::WALK_TILE) + 0.5);
}

int Utility::WalkToBuildPosition(int x) {
	return (int)floor(((double)x / 4) + 0.5);
}

int Utility::PositionToBuildPosition(int x) {
	return (int)floor(((double)x / Const::WALK_TILE) + 0.5);
}

BWAPI::Position Utility::PositionToWalkTile(BWAPI::Position p) {
	return BWAPI::Position(PositionToWalkPosition(p.x), PositionToWalkPosition(p.y));
}

BWAPI::TilePosition Utility::WalkToBuildTile(BWAPI::Position p) {
	return BWAPI::TilePosition(WalkToBuildPosition(p.x), WalkToBuildPosition(p.y));
}

BWAPI::Position Utility::PositionToBuildTile(BWAPI::Position p) {
	return BWAPI::Position(PositionToBuildPosition(p.x), PositionToBuildPosition(p.y));
}

double Utility::distance(std::pair<int, int> start, std::pair<int, int> end) {
	return distance(start.first, start.second, end.first, end.second);
}

double Utility::distance(int sx, int sy, int ex, int ey) {
	return sqrt((ex - sx) * (ex - sx) + (ey - sy) * (ey - sy));
}

BWAPI::Position Utility::getRandomPosition(int w, int h) {
	BWAPI::Position pos = BWAPI::Position(rand() % (w * 32), rand() % (h * 32 - 32));
	return pos.makeValid();
}

BWAPI::Position Utility::getMousePosition(BWAPI::Game* Broodwar) {
	BWAPI::Position pos = BWAPI::Position(Broodwar->getScreenPosition().x + Broodwar->getMousePosition().x, Broodwar->getScreenPosition().y + Broodwar->getMousePosition().y);
	return pos.makeValid();
}

BWAPI::Position Utility::getTrainPosition(bool *side) {
	int x, y = 1320;
	int rx = (rand() % 201) - 100;
	int ry = (rand() % 201) - 100;
	if ((*side)) {
		x = 400;
	}
	else {
		x = 1600;
	}
	(*side) = !(*side);
	BWAPI::Position pos = BWAPI::Position(x + rx, y + ry);
	return pos.makeValid();
}

void Utility::printToFile(std::string fileName, std::string line) {
	std::ofstream file;
	file.open(fileName, std::ofstream::out | std::ofstream::app);
	file << line << std::endl;
	file.close();
}

void Utility::printToFile(std::fstream* file, std::string line) {
	(*file) << line << std::endl;
}