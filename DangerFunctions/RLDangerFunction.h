#pragma once
#include "DangerFunction.h"
#include "../FunctionApproximators/FunctionApproximator.h"

class RLDangerFunction : public DangerFunction {

private: 
	FunctionApproximator* FA;

	double hp;
	vector<double> createInput(double);

public:
	RLDangerFunction(BWAPI::UnitType, FunctionApproximator*);
	~RLDangerFunction();

	void setUnitPtr(BWAPI::UnitInterface*);

	double compute(double dist);
	void learn(double, double);
};

