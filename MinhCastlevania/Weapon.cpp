#include "Weapon.h"
#include "Item.h"

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

void CWeapon::CollisionWithEnemy(CEnemy* e,int damage)
{
	e->Death(damage);
}

void CWeapon::CollisionWithObjects(vector<CGameObject*>* objects)
{
	for (int i = 0; i < objects->size(); i++)
	{
		if (dynamic_cast<CItem*> (objects->at(i))) continue;
		if (IsContain(this->GetBBox(), objects->at(i)->GetBBox()))
		{
			if (dynamic_cast<CEnemy*> (objects->at(i)))
				static_cast<CEnemy*>(objects->at(i))->Death(this->GetDamage());
			else
				objects->at(i)->Death();
			if (isUsedOnceTime)
				this->isFinish = 1;
			isCollision = 1;
		}
	}
}
