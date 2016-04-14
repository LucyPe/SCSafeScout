#pragma once
#include "DangerFunction.h"
#include "RBF.h"

class RLDangerFunction : public DangerFunction {

private: 
	RBF* FA;

	double hp;

public:
	RLDangerFunction(BWAPI::UnitType, RBF*);
	~RLDangerFunction();

	void setUnitPtr(BWAPI::UnitInterface*);

	double compute(double dist);
	void learn(double, double);
};

