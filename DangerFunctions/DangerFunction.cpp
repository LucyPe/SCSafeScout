#pragma once
#include "DangerFunction.h"
#include "../Const.h"
#include <iomanip>

DangerFunction::DangerFunction(BWAPI::UnitType enemy) {
	enemyType = enemy;
	unitPtr = NULL;
}

DangerFunction::~DangerFunction() {}

BWAPI::UnitType DangerFunction::getEnemyType() {
	return enemyType;
}

void DangerFunction::setUnitPtr(BWAPI::UnitInterface* unit) {
	unitPtr = unit;
}

void DangerFunction::visualize(string fileName, bool append) {
	ofstream output;
	if (append) output.open(Const::PATH_WRITE + fileName, std::ofstream::out | std::ofstream::app);
	else output.open(Const::PATH_WRITE + fileName, std::ofstream::out);
	
	if (output.is_open()) {
		output << std::setprecision(10);

		for (double i = 0; i < Const::MAX_RANGE; i++) {
			output << compute(i) << ", ";
		}
		output << endl;
		output.close();
	}
}