#pragma once
#include "DangerFunction.h"
#include "../FunctionApproximators/FunctionApproximator.h"

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