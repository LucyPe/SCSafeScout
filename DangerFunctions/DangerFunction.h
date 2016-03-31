#pragma once
#include <BWAPI.h>
#include <fstream>

using namespace std;

class DangerFunction {

protected:
	// enemy unit type
	BWAPI::UnitType enemyType;
	// pointer for agent unit
	BWAPI::UnitInterface* unitPtr;

public:
	DangerFunction(BWAPI::UnitType);
	virtual ~DangerFunction();

	// returns enemy type
	BWAPI::UnitType getEnemyType();

	//set pointer for agent unit
	virtual void setUnitPtr(BWAPI::UnitInterface*);

	// compute danger function
	virtual double compute(double dist) { return 0; };

	/* update danger function 
	* params:
	* distance from enemy unit in state1
	* pointer to enemy unit for computing distance from agent in state2
	*/
	virtual void learn(double dist, BWAPI::UnitInterface*) {};

	// save danger function to file
	void visualize(string, bool);
};

