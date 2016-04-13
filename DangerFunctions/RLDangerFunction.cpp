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

		vector<double> new_state = createInput(new_dist);
		vector<double> new_output = FA->compute(new_state);

		vector<double> last_state = createInput(dist);
		vector<double> last_output = FA->compute(last_state);

		//Utility::printToFile(Const::PATH_ERROR, std::to_string(hp) + " " + std::to_string(maxHp));

		vector<double> target = vector<double>(1, ((hp - actualHp) + Const::GAMMA * new_output.at(0)));

		//if ((hp - actualHp) != 0) Utility::printToFile(Const::PATH_DEBUG, std::to_string(dist) + " " + std::to_string(new_dist));

		if (FA->error(target, last_output)[0] != 0) {
			FA->adjust(last_state, last_output, target);
			Utility::printToFile(Const::PATH_DEBUG, std::to_string(dist) + " " + std::to_string(new_dist) + " " + std::to_string(new_output.at(0)));
			DangerFunction::visualize(Const::PATH_WRITE + (string) "DF_tmp.dat", false);
		}

		hp = actualHp;
	}
	else {
		Utility::printToFile(Const::PATH_ERROR, "ActualDangerFunction - no unit pointer");
	}
}

double RLDangerFunction::compute(double dist) {
	return FA->compute(createInput(dist)).at(0);
}