#include "ActualDangerFunction.h"

ActualDangerFunction::ActualDangerFunction(BWAPI::UnitType enemy, FunctionApproximator* fApprox) : DangerFunction(enemy) {
	FA = fApprox;
	maxHp = -1;
}

ActualDangerFunction::~ActualDangerFunction() {
	FA->saveToFile();
	delete(FA);
}

vector<double> ActualDangerFunction::createInput(double dist) {
	return vector<double>(1, dist / 400);
}

void ActualDangerFunction::setUnitPtr(BWAPI::UnitInterface* unit) {
	DangerFunction::setUnitPtr(unit);
	maxHp = -1;
}

void ActualDangerFunction::learn(double dist) {
	if (unitPtr != NULL) {
		if (maxHp == -1) {
			maxHp = unitPtr->getType().maxHitPoints() + unitPtr->getType().maxShields();
			hp = (unitPtr->getHitPoints() + unitPtr->getShields());

			testFile.open("bwapi-data/write/gg.txt", std::ofstream::out | std::ofstream::app);
			if (testFile.is_open()) {
				testFile << "init:" << maxHp << endl;
				testFile.close();
			}

		}
		else {
			double actualHp = (unitPtr->getHitPoints() + unitPtr->getShields());
			vector<double> input = createInput(dist);
			FA->adjust(input, FA->error(vector<double>(1, (hp - actualHp) / maxHp), FA->compute(createInput(dist))));

			testFile.open("bwapi-data/write/gg.txt", std::ofstream::out | std::ofstream::app);
			if (testFile.is_open()) {
				testFile << "learn:" << hp << " " << actualHp << " " << FA->error(vector<double>(1, (hp - actualHp) / maxHp), FA->compute(createInput(dist))).at(0) << " " << dist << " " << FA->compute(createInput(dist)).at(0) << endl;
				testFile.close();
			}

			hp = actualHp;
		}
	}
}

double ActualDangerFunction::compute(double dist) {
	double result = FA->compute(createInput(dist)).at(0);
	if (result < 0) result = 0;
	return (result * maxHp);
}
