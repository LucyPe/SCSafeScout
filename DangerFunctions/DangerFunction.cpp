#pragma once
#include "DangerFunction.h"
#include "../Const.h"

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

void DangerFunction::visualize(string fileName) {
	ofstream output;
	output.open(Const::PATH_WRITE + fileName, std::ofstream::out | std::ofstream::app);

	if (output.is_open()) {
		for (double i = 0; i < Const::MAX_RANGE; i++) {
			output << compute(i / Const::MAX_RANGE) << ", ";
		}
		output << endl;
		output.close();
	}
}