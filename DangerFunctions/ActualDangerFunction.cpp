#pragma once
#include "ActualDangerFunction.h"
#include "../Const.h"
#include "../Utility.h"

ActualDangerFunction::ActualDangerFunction(BWAPI::UnitType enemy, FunctionApproximator* fApprox) : DangerFunction(enemy) {
	FA = fApprox;
	maxHp = 1;
}

ActualDangerFunction::~ActualDangerFunction() {
	FA->saveToFile();
	delete(FA);
}

vector<double> ActualDangerFunction::createInput(double dist) {
	return vector<double>(1, dist);
}

void ActualDangerFunction::setUnitPtr(BWAPI::UnitInterface* unit) {
	DangerFunction::setUnitPtr(unit);
	maxHp = unitPtr->getType().maxHitPoints() + unitPtr->getType().maxShields();
	hp = (unitPtr->getHitPoints() + unitPtr->getShields());
}

void ActualDangerFunction::learn(double dist) {
	if (unitPtr != NULL) {
		double actualHp = (unitPtr->getHitPoints() + unitPtr->getShields());
		vector<double> input = createInput(dist);
		vector<double> output = FA->compute(input);
		vector<double> target = createInput((hp - actualHp) / maxHp);
		//Utility::printToFile(Const::PATH_DEBUG, std::to_string((hp - actualHp) / maxHp));
		
		if (FA->error(target, output)[0] != 0) {
			FA->adjust(input, output, target);
		}

		/*testFile.open("bwapi-data/write/gg.txt", std::ofstream::out | std::ofstream::app);
		if (testFile.is_open()) {
			testFile << "learn:" << hp << " " << actualHp << " " << FA->error(vector<double>(1, (hp - actualHp) / maxHp), FA->compute(createInput(dist))).at(0) << " " << dist << " " << FA->compute(createInput(dist)).at(0) << endl;
			testFile.close();
		}*/

		hp = actualHp;
	}
	else {
		Utility::printToFile(Const::PATH_ERROR, "ActualDangerFunction - no unit pointer");
	}
}

double ActualDangerFunction::compute(double dist) {
	double result = FA->compute(createInput(dist)).at(0);
	return result;
}
