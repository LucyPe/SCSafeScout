#pragma once
#include "../FunctionApproximators/FunctionApproximator.h"
#include "DangerFunction.h"

class ActualDangerFunction : public DangerFunction {

private:
	FunctionApproximator* FA;

	double hp;
	double maxHp;
	vector<double> createInput(double);

public:
	ActualDangerFunction(BWAPI::UnitType, FunctionApproximator*);
	~ActualDangerFunction();

	void setUnitPtr(BWAPI::UnitInterface*);

	double compute(double);
	void learn(double);
};