#pragma once
#include "ActualDangerFunction.h"
#include "../Const.h"
#include "../Utility.h"

ActualDangerFunction::ActualDangerFunction(BWAPI::UnitType enemy, RBF* fApprox) : DangerFunction(enemy) {
	FA = fApprox;
}

ActualDangerFunction::~ActualDangerFunction() {
	FA->saveToFile();
	delete(FA);
}

void ActualDangerFunction::setUnitPtr(BWAPI::UnitInterface* unit) {
	DangerFunction::setUnitPtr(unit);
	hp = (unitPtr->getHitPoints() + unitPtr->getShields());
}

void ActualDangerFunction::learn(double dist, double = 0) {
	if (unitPtr != NULL) {
		double actualHp = (unitPtr->getHitPoints() + unitPtr->getShields());
		double input = createInput(dist);
		double output = FA->compute(input);
		double target = (hp - actualHp);
				
		if (FA->error(target, output) != 0) {
			FA->adjust(output, target);
			DangerFunction::visualize(Const::PATH_WRITE + (string) "DF_tmp.dat", false);
		}

		hp = actualHp;
	}
	else {
		Utility::printToFile(Const::PATH_ERROR, "ActualDangerFunction - no unit pointer");
	}
}

double ActualDangerFunction::compute(double dist) {
	double result = FA->compute(createInput(dist));
	return result;
}
