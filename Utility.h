#pragma once
#include "cmath"
#include <BWAPI.h>

#define WALK_TILE 8
#define BUILD_TILE 32

namespace Utility {
	BWAPI::Position getWalkTile(BWAPI::Position);
	BWAPI::Position getWalkTile(int, int);

	int PositionToWalkPosition(int);
	BWAPI::Position PositionToWalkTile(BWAPI::Position);
	int WalkToBuildPosition(int);
	BWAPI::TilePosition WalkToBuildTile(BWAPI::Position);
	int PositionToBuildPosition(int);
	BWAPI::Position PositionToBuildTile(BWAPI::Position);

	bool PositionInRange(int, int, int, int, int);
	bool PositionInRange(BWAPI::Position, BWAPI::Position, int);

	double distance(std::pair<int, int>, std::pair<int, int>);
	double distance(int, int, int, int);

	// map width, map height
	BWAPI::Position getRandomPosition(int, int);
	BWAPI::Position getMousePosition(BWAPI::Game*);
}