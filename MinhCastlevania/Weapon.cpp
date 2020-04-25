#include "Weapon.h"

CWeapon::CWeapon()
{
	isFinish = true;
	lifeTime = 0;
	objType = WEAPON;
	weaponType = Null;
}

bool CWeapon::IsFinish()
{
	return isFinish;
}

ObjectType CWeapon::GetType()
{
	return weaponType;
}