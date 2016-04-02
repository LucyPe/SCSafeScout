#pragma once
#include "RLDangerFunction.h"
#include "../Const.h"
#include "../Utility.h"

RLDangerFunction::RLDangerFunction(BWAPI::UnitType unit, FunctionApproximator* fApprox) : DangerFunction(unit) {
	FA = fApprox;
}

RLDangerFunction::~RLDangerFunction() {
	FA->saveToFile();
	delete(FA);
}

vector<double> RLDangerFunction::createInput(double dist) {
	return vector<double>(1, dist / Const::MAX_RANGE);
}

void RLDangerFunction::setUnitPtr(BWAPI::UnitInterface* unit) {
	DangerFunction::setUnitPtr(unit);
	hp = (unitPtr->getHitPoints() + unitPtr->getShields());
}

void RLDangerFunction::learn(double dist, double new_dist) {
	if (unitPtr != NULL) {
		double actualHp = (unitPtr->getHitPoints() + unitPtr->getShields());
		vector<double> last_state = createInput(dist);
		vector<double> output = FA->compute(last_state);

		//Utility::printToFile(Const::PATH_ERROR, std::to_string(hp) + " " + std::to_string(maxHp));

		vector<double> new_state = createInput(new_dist);
		vector<double> new_output = FA->compute(new_state);

		vector<double> target = vector<double>(1, ((hp - actualHp) + Const::GAMMA * new_output.at(0)));

		//Utility::printToFile(Const::PATH_DEBUG, std::to_string(dist) + " " + std::to_string(new_dist) + " " + std::to_string(new_output.at(0)));

		if (FA->error(target, output)[0] != 0) {
			FA->adjust(last_state, output, target);
			DangerFunction::visualize(Const::PATH_DF + "tmp.dat", false);
		}

		hp = actualHp;
	}
	else {
		Utility::printToFile(Const::PATH_ERROR, "ActualDangerFunction - no unit pointer");
	}
}

double RLDangerFunction::compute(double dist) {
	double result = FA->compute(createInput(dist)).at(0);
	return result;
}