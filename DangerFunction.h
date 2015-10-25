#pragma once
#include <BWAPI.h>
#include "MLP.h"

#define PATH_READ "bwapi-data/write/"
#define PATH_WRITE "bwapi-data/write/"
#define X_AXE 400

class DangerFunction {

protected:
	BWAPI::UnitType enemyType;
	BWAPI::UnitInterface* unitPtr;
	ofstream testFile;

public:
	DangerFunction(BWAPI::UnitType);
	virtual ~DangerFunction();

	BWAPI::UnitType getEnemyType();
	void setUnitPtr(BWAPI::UnitInterface*);

	virtual double compute(double dist) {
		return 0;
	};
	virtual void learn(double dist) {};
	void visualize(string);
};

