#pragma once

namespace Const {
	
	/* params: 
	>> NO_GUI >> GRID >> MOVE >> PATH >> TERRAIN_DATA >> ENEMY_RANGE
	>> dangerWeight	
	*/

	/* Learning params*/
	const int LEARNING_FRAME_RATE = 5;
	const int MODEL = 2; // -1 - no danger, 0 - Computed, 1 - Actual, 2 - RL
	const int MODE = 2; // 0 - training, 1 - testing (3ways), 2 - testing (performance)

	const BWAPI::Position TEST_POSITION = BWAPI::Position(1800, 150);

	/* MAP variables */
	const int WALK_TILE = 8;
	const int BUILD_TILE = 32;
	const double MAX_RANGE = 10 * BUILD_TILE; // 320

	const int PATH_UPDATE_FRAME_RATE = 5;

	/* Neural Networks variables */
	const int CENTERS = 20;

	const double ALPHA = 0.02;
	const double GAMMA = 0.99998; //sqrt(0.08,20)
	const double SIGMA = (MAX_RANGE / CENTERS) / MAX_RANGE;
	const double RADIUS = (MAX_RANGE / CENTERS) / MAX_RANGE;

	/* File paths */
	const char * const PATH_READ = "bwapi-data/read/";
	const char * const PATH_WRITE = "bwapi-data/write/";
	const char * const PATH_MAP = "bwapi-data/BWTA2/";
	const char * const PATH_TEMP = "bwapi-data/write/temp.txt";
	const char * const PATH_DEBUG = "bwapi-data/write/debug.txt";
	const char * const PATH_ERROR = "bwapi-data/write/errors.txt";

	const std::string PATH_DATA = PATH_WRITE + (std::string) "data_" + std::to_string(MODEL) + ".txt";
	const std::string PATH_FA = PATH_WRITE + (std::string) "FA_" + std::to_string(MODEL) + "_";
	const std::string PATH_DF = PATH_WRITE + (std::string) "DF_" + std::to_string(MODEL) + "_";

}