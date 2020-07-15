#include "PhantomBatBullet.h"

CPhantomBatBullet::CPhantomBatBullet(float _x, float _y)
{
    x = _x;
    y = _y;
    sprite = CSprites::GetInstance()->Get(PHANTOMBAT_BULLET);
    weaponType = PHANTOMBAT_BULLET;
    isFinish = 0;
}

void CPhantomBatBullet::SetVectorMove(float _nvx, float _nvy)
{
    vx = SPEED_MOVE * ((_nvx > 0) ? 1 : -1);
    vy =(float)abs((float)(_nvy / _nvx)) * SPEED_MOVE* ((_nvy > 0) ? 1 : -1);
}

void CPhantomBatBullet::Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects)
{
    if (isFinish) return;
    CGameObject::Update(dt);
    x += dx;
    y += dy;
    if (!CCamera::GetInstance()->IsContainCam(GetBBox()))
        isFinish = true;
    if (IsContain(GetBBox(), CSimon::GetIntance()->GetBBox()))
    {
        isFinish = true;
        CSimon::GetIntance()->SetInjured(DAMAGE_BULLET,(vx>0)?1:-1);
    }
}

bool CPhantomBatBullet::Attack(float _x, float _y, int nx)
{
    return false;
}

void CPhantomBatBullet::GetBoundingBox(float& l, float& t, float& r, float& b)
{
    l = x;
    t = y;
    r = x + BULLET_BBOX.x;
    b = y + BULLET_BBOX.y;
}

void CPhantomBatBullet::Render()
{
    if (isFinish) return;
    RenderBoundingBox();
    if (vx >= 0)
        sprite->Draw(x, y);
    else
        sprite->DrawFlipX(x, y);
}

int CPhantomBatBullet::GetDamage()
{
    return DAMAGE_BULLET;
}

bool CPhantomBatBullet::IsFinish() {
    return isFinish;
}