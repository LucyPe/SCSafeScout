#pragma once
#include "DangerFunction.h"

class ComputedDangerFunction : public DangerFunction {

private: 
	BWAPI::WeaponType unitWeapons[2];
	static double dmgTable[3][3];

	double dmgFactor(int);

public:
	ComputedDangerFunction(BWAPI::UnitType);
	~ComputedDangerFunction();

	double compute(double);
};

