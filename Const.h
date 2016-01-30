#pragma once

namespace Const {
	
	const double DANGER_WEIGTH = 1; // interval <0,1>

	/* Learning params*/
	const bool LEARNING = 1;
	const int LEARNING_FRAME_RATE = 1;

	const bool NO_GUI = 0;

	/* MAP variables */
	const int WALK_TILE = 8;
	const int BUILD_TILE = 32;
	const double MAX_RANGE = 8 * BUILD_TILE; // 256

	/* Neural Networks variables */
	const bool FA = 1; //MLP or RBF xx

	const int NEURONS = 5;
	const int CENTERS = 20;

	const double ALPHA = 0.02;
	const double SIGMA = (MAX_RANGE / CENTERS) / MAX_RANGE;
	const double RADIUS = (MAX_RANGE / CENTERS) / MAX_RANGE;

	/* File paths */
	const char * const ADF_WRITE_PATH = "bwapi-data/write/ADF_";
	const char * const PATH_READ = "bwapi-data/read/";
	const char * const PATH_WRITE = "bwapi-data/write/";
	const char * const PATH_DEBUG = "bwapi-data/write/debug.txt";
	const char * const PATH_ERROR = "bwapi-data/write/errors.txt";

	/* Visual data */
	const bool TERRAIN_DATA = 1;
	const bool ENEMY_RANGE = 1;

	

	/**/
}