#pragma once
#include "DangerFunction.h"
#include "RBF.h"

class ActualDangerFunction : public DangerFunction {

private:
	RBF* FA;

	//current hp
	double hp;

public:
	ActualDangerFunction(BWAPI::UnitType, RBF*);
	~ActualDangerFunction();

	void setUnitPtr(BWAPI::UnitInterface*);

	double compute(double);
	void learn(double, double);
};