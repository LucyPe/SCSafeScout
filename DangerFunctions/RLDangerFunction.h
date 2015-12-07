#pragma once
#include "DangerFunction.h"
#include "../FunctionApproximators/FunctionApproximator.h"

class RLDangerFunction : public DangerFunction {

private: 
	FunctionApproximator* FA;

public:
	RLDangerFunction(BWAPI::UnitType, FunctionApproximator*);
	~RLDangerFunction();

	double compute(double dist, bool isAir);
	//void saveToFile(std::string file);
};

