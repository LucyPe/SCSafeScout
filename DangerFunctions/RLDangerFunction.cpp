#pragma once
#include "RLDangerFunction.h"
#include "../Const.h"
#include "../Utility.h"

RLDangerFunction::RLDangerFunction(BWAPI::UnitType unit, FunctionApproximator* fApprox) : DangerFunction(unit) {
	FA = fApprox;
	maxHp = 1;
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
	maxHp = unitPtr->getType().maxHitPoints() + unitPtr->getType().maxShields();
	hp = (unitPtr->getHitPoints() + unitPtr->getShields());
}

void RLDangerFunction::learn(double dist, BWAPI::UnitInterface* enemy) {
	if (unitPtr != NULL) {
		double actualHp = (unitPtr->getHitPoints() + unitPtr->getShields());
		vector<double> last_state = createInput(dist);
		vector<double> output = FA->compute(last_state);

		double new_dist = dist;
		if (enemy != NULL) {
			BWAPI::Position unit_pos = unitPtr->getPosition();
			BWAPI::Position enemy_pos = enemy->getPosition();
			new_dist = Utility::distance(unit_pos.x, unit_pos.y, enemy_pos.x, enemy_pos.y);
		}
		else {
			Utility::printToFile(Const::PATH_ERROR, "ActualDangerFunction - no enemy unit pointer");
		}

		vector<double> new_state = createInput(new_dist);
		vector<double> new_output = FA->compute(new_state);

		vector<double> target = vector<double>(1, ((hp - actualHp) + Const::GAMMA * new_output.at(0)));

		if (FA->error(target, output)[0] != 0) {
			FA->adjust(last_state, output, target);
			DangerFunction::visualize(std::to_string(Const::MODEL) + "_" + "Zerg_Hydralisk" + ".dat", false);
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