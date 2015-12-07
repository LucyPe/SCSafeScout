#pragma once
#include "DangerFunction.h"

class ComputedDangerFunction : public DangerFunction {

private: 
	BWAPI::WeaponType unitWeapons[2];

public:
	ComputedDangerFunction(BWAPI::UnitType);
	~ComputedDangerFunction();

	double compute(double);
};

