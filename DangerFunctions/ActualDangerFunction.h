#pragma once
#include "../FunctionApproximators/FunctionApproximator.h"
#include "DangerFunction.h"

class ActualDangerFunction : public DangerFunction {

private:
	FunctionApproximator* FA;

	//current hp
	double hp;

	// method for converting input to vector
	vector<double> createInput(double);

public:
	ActualDangerFunction(BWAPI::UnitType, FunctionApproximator*);
	~ActualDangerFunction();

	void setUnitPtr(BWAPI::UnitInterface*);

	double compute(double);
	void learn(double, double);
};