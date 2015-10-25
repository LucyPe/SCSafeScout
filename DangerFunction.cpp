#include "DangerFunction.h"

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
	output.open(PATH_WRITE + fileName, std::ofstream::out | std::ofstream::app);

	// exists file
	if (output.is_open()) {
		for (int i = 0; i < X_AXE; i++) {
			output << compute(i) << ", ";
		}
		output << endl;
		output.close();
	}
}