#pragma once
#include "RLDangerFunction.h"
#include "../Const.h"
#include "../Utility.h"

RLDangerFunction::RLDangerFunction(BWAPI::UnitType unit, RBF* fApprox) : DangerFunction(unit) {
	FA = fApprox;
}

RLDangerFunction::~RLDangerFunction() {
	FA->saveToFile();
	delete(FA);
}

void RLDangerFunction::setUnitPtr(BWAPI::UnitInterface* unit) {
	DangerFunction::setUnitPtr(unit);
	hp = (unitPtr->getHitPoints() + unitPtr->getShields());
	maxHp = 40;
}

void RLDangerFunction::learn(double dist, double new_dist) {
	if (unitPtr != NULL) {
		double actualHp = (unitPtr->getHitPoints() + unitPtr->getShields());

		double new_state = createInput(new_dist);
		double last_state = createInput(dist);

		double new_output = FA->compute(new_state);
		double last_output = FA->compute(last_state);

		double target = (hp - actualHp) + Const::GAMMA * new_output;

		if (FA->error(target, last_output) != 0) {
			FA->adjust(last_output, target);
			//DangerFunction::visualize(Const::PATH_WRITE + (string) "DF_tmp.dat", false);
		}

		hp = actualHp;
	}
	else {
		Utility::printToFile(Const::PATH_ERROR, "ActualDangerFunction - no unit pointer");
	}
}

double RLDangerFunction::compute(double dist) {
	return FA->compute(createInput(dist));
}