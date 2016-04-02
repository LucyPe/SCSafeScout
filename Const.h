#pragma once

namespace Const {
	
	/* params: 
	>> GRID >> PATH >> MOVE >> NO_GUI >> TERRAIN_DATA >> ENEMY_RANGE
	>> dangerWeight	
	*/

	/* Tracking params*/
	static int ITERATION = 0;
	static int POSITION_COUNT = 0;

	const double DANGER_WEIGTH = 1; // interval <0,1>

	/* Learning params*/
	const int LEARNING_FRAME_RATE = 5;
	const int MODEL = 2;
	const int MODE = 0; // 0 - training, 1 - testing
	static bool SIDE = 1;


	/* MAP variables */
	const int WALK_TILE = 8;
	const int BUILD_TILE = 32;
	const double MAX_RANGE = 8 * BUILD_TILE; // 256

	const int PATH_UPDATE_FRAME_RATE = 5;

	/* Neural Networks variables */
	const int NEURONS = 5;
	const int CENTERS = 20;

	static double ALPHA = 0.05;
	static double GAMMA = 0.7; //sqrt(0.08,20)
	const double SIGMA = (MAX_RANGE / CENTERS) / MAX_RANGE;
	const double RADIUS = (MAX_RANGE / CENTERS) / MAX_RANGE;

	/* File paths */
	const char * const PATH_READ = "bwapi-data/read/";
	const char * const PATH_WRITE = "bwapi-data/write/";
	const char * const PATH_MAP = "bwapi-data/BWTA/";
	const char * const PATH_TEMP = "bwapi-data/write/temp.txt";
	const char * const PATH_DEBUG = "bwapi-data/write/debug.txt";
	const char * const PATH_ERROR = "bwapi-data/write/errors.txt";

	static std::string PATH_FA = PATH_WRITE + std::to_string(ITERATION) + "_FA_" + std::to_string(MODEL) + "_";
	static std::string PATH_DF = PATH_WRITE + std::to_string(ITERATION) + "_DF_" + std::to_string(MODEL) + "_";

}