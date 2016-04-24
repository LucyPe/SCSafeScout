#pragma once
#include <BWAPI.h>
#include <string>
#include <fstream>

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
	BWAPI::Position getTrainPosition(bool*);

	//file
	void printToFile(std::string, int);
	void printToFile(std::string, std::string line);
	void printToFile(std::fstream*, std::string line);

	void readFromFile(std::string, int*);
}