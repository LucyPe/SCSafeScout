#pragma once
#include "ComputedDangerFunction.h"
#include "../Const.h"
#include "../Utility.h"

ComputedDangerFunction::ComputedDangerFunction(BWAPI::UnitType enemy) : DangerFunction(enemy) {
	unitWeapons[0] = enemy.groundWeapon();
	unitWeapons[1] = enemy.airWeapon();
}

ComputedDangerFunction::~ComputedDangerFunction() {}

double ComputedDangerFunction::compute(double dist) {
	if (unitPtr != NULL) {
		int index = (int)unitPtr->isFlying();
		if ((dist < unitWeapons[index].minRange()) || (dist > unitWeapons[index].maxRange() + 2 * fmaxf(enemyType.width(), enemyType.height()))) {
			return 0;
		}
		else {
			return ((unitWeapons[index].damageAmount() + unitWeapons[index].damageBonus()) /*/ unitWeapons[index].damageCooldown()*/);
		}
	}
	else {
		 return 0;
	}
}