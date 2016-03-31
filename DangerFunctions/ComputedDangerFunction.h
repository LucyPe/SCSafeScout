#pragma once
#include "DangerFunction.h"

class ComputedDangerFunction : public DangerFunction {

private: 
	// stores ground and air weapon
	BWAPI::WeaponType unitWeapons[2];
	// table for computing damage according to enemy damage type and agent unit size 
	static double dmgTable[3][3];

	// computes enemy damage with dmgTable
	double dmgFactor(int);

public:
	ComputedDangerFunction(BWAPI::UnitType);
	~ComputedDangerFunction();

	double compute(double);
};

