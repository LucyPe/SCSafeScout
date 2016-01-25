#pragma once
#include "RLDangerFunction.h"

RLDangerFunction::RLDangerFunction(BWAPI::UnitType unit, FunctionApproximator* fApprox) : DangerFunction(unit) {
	FA = fApprox;
}


RLDangerFunction::~RLDangerFunction() {
	delete(FA);
}

double RLDangerFunction::compute(double dist, bool isAir) {
	return 0;
}