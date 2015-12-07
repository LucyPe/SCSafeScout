#pragma once
#include <BWAPI.h>
#include <fstream>

#define PATH_READ "bwapi-data/write/"
#define PATH_WRITE "bwapi-data/write/"
#define X_AXE 400

using namespace std;

class DangerFunction {

protected:
	BWAPI::UnitType enemyType;
	BWAPI::UnitInterface* unitPtr;
	ofstream testFile;

public:
	DangerFunction(BWAPI::UnitType);
	virtual ~DangerFunction();

	BWAPI::UnitType getEnemyType();
	virtual void setUnitPtr(BWAPI::UnitInterface*);

	virtual double compute(double dist) {
		return 0;
	};
	virtual void learn(double dist) {};
	void visualize(string);
};

