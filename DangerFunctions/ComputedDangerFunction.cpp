#pragma once
#include "ComputedDangerFunction.h"
#include "../Const.h"
#include "../Utility.h"

double ComputedDangerFunction::dmgTable[3][3] = {
	{ 1.0, 1.0, 0.5 },	//SMALL
	{ 0.5, 1.0, 0.75 }, //NORMAL
	{ 0.75, 1.0, 1.0 }	//LARGE
};

ComputedDangerFunction::ComputedDangerFunction(BWAPI::UnitType enemy) : DangerFunction(enemy) {
	unitWeapons[0] = enemy.groundWeapon();
	unitWeapons[1] = enemy.airWeapon();
}

ComputedDangerFunction::~ComputedDangerFunction() {}

double ComputedDangerFunction::compute(double dist) {
	if (unitPtr != NULL) {
		int index = (int)unitPtr->isFlying();

		if ((dist >= unitWeapons[index].minRange()) && (dist <= unitWeapons[index].maxRange() /*+ 2 * fmaxf(enemyType.width(), enemyType.height())*/)) {
			double dmg = dmgFactor(index) * unitWeapons[index].damageAmount() /*+ unitWeapons[index].damageBonus()*/;
			// dps
			return (dmg / unitWeapons[index].damageCooldown()); 
		}
	}
	else {
		Utility::printToFile(Const::PATH_ERROR, "ConmuptedDangerFunction - no unit pointer");
	}
	return 0;
}

double ComputedDangerFunction::dmgFactor(int index) {
	BWAPI::UnitSizeType size = unitPtr->getType().size();
	BWAPI::DamageType dmgType = unitWeapons[index].damageType();
	int x, y;

	if (size == BWAPI::UnitSizeTypes::Small) x = 0;
	else if (size == BWAPI::UnitSizeTypes::Large) x = 2;
	else x = 1;

	if (dmgType == BWAPI::DamageTypes::Concussive) y = 0;
	else if (dmgType == BWAPI::DamageTypes::Explosive) y = 2;
	else y = 1;

	return dmgTable[x][y];
}