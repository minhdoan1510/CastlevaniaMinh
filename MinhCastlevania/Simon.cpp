#include <algorithm>
#include <assert.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

#include "Simon.h"
#include "Game.h"
#include "Item.h"
#include "Brick.h"
#include "Knife.h"
#include "Trigger.h"
#include "Axe.h"
#include "Boomerang.h"
#include "Gunpowder.h"

#include "Camera.h"

CSimon* CSimon::intance = NULL;

CSimon::CSimon()
{
	sprite = CSprites::GetInstance()->Get(SIMON);
	texture = CTextureManager::GetInstance()->Get(SIMON);
	isuntouchable = 0;
	nx = 1;
	objType = SIMON;
	weaponcurrent = WHIP;	
	listWeapon[WHIP] = new CWhip();
	heart = 30;
	lifeSimon = SIMON_DEFAULT_LIFE;
	HPSimon = SIMON_DEFAULT_HP;
	isWalking = 0;
	CCamera::GetInstance()-> SetPosition(0, 0);
	isAniDead = 0;
	secondaryweaponcurrent = Null;
	isOnStair = 0;
	isAutoGo = 0;
	isWantOnBottomstair = 0;
	isWantOnTopstair = 0;
	isProcessStair = 0;
	numberStair = 0;
	h_StepStair = 0;
	isContainTopTrigger=0;
	isContainBottomTrigger=0;
	amountSecondWeapon = 1;
	//SetAutoGo(200, 1, 1);
}

void CSimon::clearListSecondWeapon()
{
	CWeapon* whipBackup = listWeapon[WHIP];

	for (pair<int,CWeapon*> item : listWeapon)
	{
		if (!dynamic_cast<CWhip*> (item.second))
		{
			SAFE_DELETE(item.second);
		}
	}
	listWeapon.clear();
	listWeapon[WHIP] = whipBackup;
}

CSimon* CSimon::GetIntance()
{
	if (intance == NULL)
		intance = new CSimon();
	return intance;
}

void CSimon::SetPosition(float _x, float _y)
{
	CGameObject::SetPosition(_x, _y);
	x_backup = _x;
	y_backup = _y;
	nx_backup = nx;
}

void CSimon::SetFinish(bool _i)
{
	IsFinish = _i;
	IsDead = _i;
	isAniDead = _i;
}

void CSimon::DeathSimon()
{
	isAniDead = true;
}

void CSimon::SetAutoGo(float _AutoGo_X_Distance, int DirectBegin, int _DirectEnd)
{
	if (isAutoGo) return;
	AutoGo_X_Backup = x;
	AutoGo_X_Distance = _AutoGo_X_Distance;
	isAutoGo = true;
	nx = DirectBegin;
	DirectEnd = _DirectEnd;
	isWalking = 1;
	isJumping = 0;
	isSitting = 0;
	isAttacking = 0;
	isOnStair = 0;
	vx = nx * SIMON_SPEED_AUTO_GO;
}

bool CSimon::IsAutoGo()
{
	return isAutoGo;
}

void CSimon::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (IsFinish)
	{
		ResetSimon();
		return;
	}
	
	// AABB check incontain stair trigger 
	if (!IsAutoGo())
	{
		bool colBottomStair = 0, colTopStair = 0;
		isContainTopTrigger = 0;
		isContainBottomTrigger = 0;
		for (int i = 0; i < coObjects->size(); i++)
		{
			if (dynamic_cast<CTrigger*>(coObjects->at(i)))
			{
				switch (static_cast<CTrigger*> (coObjects->at(i))->GetTriggerType())
				{
				case BOTTOM_STAIRS_TRIGGER:
					if (nx != static_cast<CTrigger*>(coObjects->at(i))->GetDirectStair() && isOnStair&& static_cast<CTrigger*>(coObjects->at(i))->GetDirectStair()==directOnStair) // Go down collision with Bottom stairTrigger
					{
						if (IsContain(this->GetBBox(), coObjects->at(i)->GetBBox()))
						{
							y -= 50;
							float vyBackup = vy;
							vy = 99999;
							dy = dt * vy;
							vector<LPCOLLISIONEVENT> _coEvents;
							vector<LPCOLLISIONEVENT> _coEventsResult;
							_coEvents.clear();
							vector<LPGAMEOBJECT> listBrick;
							listBrick.clear();
							for (int j = 0; j < coObjects->size(); j++)
								if (dynamic_cast<CBrick*>(coObjects->at(j)))
									listBrick.push_back(coObjects->at(j));
							CalcPotentialCollisions(&listBrick, _coEvents);
							if (_coEvents.size() == 0)
							{
								x += dx;
								y += dy;
							}
							else
							{
								float min_tx, min_ty, nx = 0, ny;

								FilterCollision(_coEvents, _coEventsResult, min_tx, min_ty, nx, ny);
								x += min_tx * dx + nx * 0.4f;
								y += min_ty * dy + ny * 0.4f;
								if (nx != 0 || ny != 0)
								{
									vx = 0;
									vy = 0;
									isOnStair = 0; 
									isWalking = 0;
								}
							}

							for (UINT i = 0; i < _coEvents.size(); i++)
								delete _coEvents[i];
							vy = vyBackup;
							break;
						}
					}
					if (isWantOnBottomstair&& !isOnStair) // Go Up collision with Bottom stairTrigger
					{
						if (IsContain(this->GetBBox(), coObjects->at(i)->GetBBox()))
						{
							if (x != static_cast<CTrigger*>(coObjects->at(i))->GetX_CheckPoint())
							{
								int nx_directbegin;
								float _dis = x - static_cast<CTrigger*>(coObjects->at(i))->GetX_CheckPoint();
								if (_dis > 0)
									nx_directbegin = -1;
								else
									nx_directbegin = 1;
								SetAutoGo(abs(_dis), nx_directbegin, static_cast<CTrigger*>(coObjects->at(i))->GetDirectStair());
							}
							else
							{
								isOnStair = 1;
								vy = vx = 0;
								isWantOnBottomstair = 0;
								directOnStair = static_cast<CTrigger*>(coObjects->at(i))->GetDirectStair();
							}
							colBottomStair = 1;
						}
					}
					if (IsContain(this->GetBBox(), coObjects->at(i)->GetBBox()))
						isContainBottomTrigger = 1;
					break;
				case TOP_STAIRS_TRIGGER:
					if (nx == static_cast<CTrigger*>(coObjects->at(i))->GetDirectStair() && isOnStair)// Go down collision with Top stairTrigger
					{
						if (IsContain(this->GetBBox(), coObjects->at(i)->GetBBox()))
						{
							float _vy = vy;
							vy = 999.0f;
							y -= 100;
							dy = vy * dt;
							vector<LPCOLLISIONEVENT> _coEvents;
							vector<LPCOLLISIONEVENT> _coEventsResult;
							_coEvents.clear();
							vector<LPGAMEOBJECT> listBrick;
							listBrick.clear();

							for (int j = 0; j < coObjects->size(); j++)
								if (dynamic_cast<CBrick*>(coObjects->at(j)))
									listBrick.push_back(coObjects->at(j));

							CalcPotentialCollisions(&listBrick, _coEvents);
							if (_coEvents.size() == 0)
							{
								x += dx;
								y += dy;
							}
							else
							{
								float min_tx, min_ty, nx = 0, ny;

								FilterCollision(_coEvents, _coEventsResult, min_tx, min_ty, nx, ny);
								x += min_tx * dx + nx * 0.4f;
								y += min_ty * dy + ny * 0.4f;
								if (nx != 0 || ny != 0)
								{
									vx = 0;
									vy = 0;
									isOnStair = 0;
									isWalking = 0;
								}
							}
							vy = _vy;
							dy = vy * dt;
							for (UINT i = 0; i < _coEvents.size(); i++)
								delete _coEvents[i];
							return;
						}
					}
					if (isWantOnTopstair && !isOnStair) // Go Up collision with Bottom stairTrigger
					{
						if (IsContain(this->GetBBox(), coObjects->at(i)->GetBBox()))
						{
							if (abs(x - static_cast<CTrigger*>(coObjects->at(i))->GetX_CheckPoint())>=1)
							{
								int nx_directbegin;
								float _dis = x - static_cast<CTrigger*>(coObjects->at(i))->GetX_CheckPoint();
								if (_dis > 0)
									nx_directbegin = -1;
								else
									nx_directbegin = 1;

								SetAutoGo(abs(_dis), nx_directbegin, ((static_cast<CTrigger*>(coObjects->at(i))->GetDirectStair()==1)?-1:1));
								isWantOnTopstair = 0;
								directOnStair = static_cast<CTrigger*>(coObjects->at(i))->GetDirectStair();
							}
							else
							{
								isOnStair = 1;
								isWantOnTopstair = 0;
								/*y += 16;
								if (directOnStair == 1)
									x -= 16;
								else
									x += 16;*/
							}
							colTopStair = true;
						}
					}
					if (IsContain(this->GetBBox(), coObjects->at(i)->GetBBox()))
						isContainTopTrigger = 1;
					break;
				case PASS_SCENE_TRIGGER:
					if (IsContain(this->GetBBox(), coObjects->at(i)->GetBBox()))
					{
						CSceneManager::GetInstance()->PassScene();
						return;
					}
					break;
				}

			}
		}
		if (!colBottomStair)
			isWantOnBottomstair = 0;
		if (!colTopStair)
			isWantOnTopstair = 0;
	}


	if (!isOnStair)
	{
		if (isJumping)
			vy += SIMON_GRAVITY_JUMPING * dt;
		else
			vy += SIMON_GRAVITY * dt; //trọng lực
	}

	CGameObject::Update(dt);

	//update position for mainWeapon
	listWeapon[WHIP]->SetPosition(x, y);

	if (IsDead || isFreeze)
	{
		vx = vy = 0;
		return;
	}
	for (auto item : listWeapon)
		item.second->Update(dt, coObjects);

	#pragma region Update State
		if (isAniDead == true)
		{
			aniState = SIMON_ANI_DEATH;
			isuntouchable = true;
			if (animation_set->at(aniState)->GetDoneFrameFinal())
			{
				isAniDead = 0;
				IsDead = true;
				ResetSimon();
			}
		}
		else
		{
			if (isOnStair)
			{
				if (isAttacking)
				{
					if (nx == directOnStair)
					{
						aniState = SIMON_ANI_STAIR_UP_ATTACKING;
					}
					else
					{
						aniState = SIMON_ANI_STAIR_DOWN_ATTACKING;
					}

					if (animation_set->at(aniState)->GetDoneFrameFinal())
					{
						isAttacking = false;
						animation_set->at(aniState)->ResetFarmeCurrent();
					
						if (nx == directOnStair)
						{
							aniState = SIMON_ANI_STAIR_UP;
						}
						else
						{
							aniState = SIMON_ANI_STAIR_DOWN;
						}
					}
				}
				else

				if (isWalking||isProcessStair)
				{
					if (nx == directOnStair)
					{
						aniState = SIMON_ANI_STAIR_UP;
					}
					else
					{
						aniState = SIMON_ANI_STAIR_DOWN;
					}
					h_StepStair += dt * vy;
					
					if (animation_set->at(aniState)->GetDoneFrameFinal())
					{
						animation_set->at(aniState)->ResetFarmeCurrent();
						numberStair++;
						isProcessStair = 0;
						vy = vx = 0;
						dx = dy = 0;
						isWalking = 0;
						DebugOut(L" %4.2f ", h_StepStair);

						if (abs(h_StepStair) < 10)
						{
							if (directOnStair == 1)
							{
								if (h_StepStair > 0 && h_StepStair != 16)
								{
									x += h_StepStair;
									y -= h_StepStair;
								}
								else if (h_StepStair < 0 && h_StepStair != -16)
								{
									x += h_StepStair;
									y -= h_StepStair;
								}
							}
							else
							{
								if (h_StepStair > 0 && h_StepStair != 16)
								{
									x -= h_StepStair;
									y -= h_StepStair;
								}
								else if (h_StepStair < 0 && h_StepStair != -16)
								{
									x += h_StepStair;
									y += h_StepStair;
								}
							}
						}
						else
						{
							if (directOnStair == 1)
							{
								if (h_StepStair > 0 && h_StepStair != 16)
								{
									x += h_StepStair - 16;
									y -= h_StepStair - 16;

									DebugOut(L" %4.2f \n", h_StepStair - 16);
								}
								else if (h_StepStair < 0 && h_StepStair != -16)
								{
									x += h_StepStair + 16;
									y -= h_StepStair + 16;
									DebugOut(L" %4.2f \n", h_StepStair + 16);
								}
							}
							else
							{
								if (h_StepStair > 0 && h_StepStair != 16)
								{
									x -= h_StepStair - 16;
									y -= h_StepStair - 16;

									DebugOut(L" %4.2f \n", h_StepStair - 16);
								}
								else if (h_StepStair < 0 && h_StepStair != -16)
								{
									x += h_StepStair + 16;
									y += h_StepStair + 16;

									DebugOut(L" %4.2f \n", h_StepStair + 16);
								}
							}
						}
						h_StepStair = 0;
					}
				}
				else
					if (nx == directOnStair)
					{
						aniState = SIMON_ANI_STAIR_STANDING_UP;
					}
					else
					{
						aniState = SIMON_ANI_STAIR_STANDING_DOWN;
					}
			}
			else
			{
				isProcessStair = 0;
				numberStair = 0;
				if (isSitting == true)
				{
					if (isAttacking == true)
					{
						if (aniState != SIMON_ANI_SITTING_ATTACKING)
						{
							aniState = SIMON_ANI_SITTING_ATTACKING;
						}
						else
						{
							if (animation_set->at(aniState)->GetDoneFrameFinal())
							{
								isAttacking = false;
								animation_set->at(aniState)->ResetFarmeCurrent();
								aniState = SIMON_ANI_SITTING;
							}
						}
					}
					else
						aniState = SIMON_ANI_SITTING;
				}
				else
				{
					if (isAttacking == true)
					{
						if (aniState != SIMON_ANI_STANDING_ATTACKING)
						{
							aniState = SIMON_ANI_STANDING_ATTACKING;
							animation_set->at(aniState)->ResetFarmeCurrent();
						}
						else
						{
							if (animation_set->at(aniState)->GetDoneFrameFinal())
							{
								isAttacking = false;
								aniState = SIMON_ANI_IDLE;
							}
						}
					}
					else
					{
						if (isWalking == true)
						{
							if (isJumping == false)
							{
								aniState = SIMON_ANI_WALKING;
							}
							else
							{
								aniState = SIMON_ANI_JUMPING;
							}
						}
						else
						{
							if (isJumping == true)
							{
								/*if (vy > 0)
									aniState = SIMON_ANI_IDLE;
								else*/
								aniState = SIMON_ANI_JUMPING;
							}
							else
							{
								aniState = SIMON_ANI_IDLE;
							}
						}
					}
				}
			}
		}
	#pragma endregion		

	#pragma region collisionHandle


		//Kiểm tra item có nằm trong người của Simon
		for (size_t i = 0; i < coObjects->size(); i++)
			if (dynamic_cast<CItem*>(coObjects->at(i)))
			{
				if (IsContain(this->GetBBox(), coObjects->at(i)->GetBBox()))
				{
					CollisionWithItem(static_cast<CItem*>(coObjects->at(i))->GetItemType());
					coObjects->at(i)->Death();
				}
			}

		vector<LPGAMEOBJECT>* _coObjects = new vector<LPGAMEOBJECT>();
		for (int i = 0; i < coObjects->size(); i++)
		{
			if (isOnStair && dynamic_cast<CBrick*>(coObjects->at(i)))continue;
			_coObjects->push_back(coObjects->at(i));
		}


		vector<LPCOLLISIONEVENT> coEvents;
		vector<LPCOLLISIONEVENT> coEventsResult;

		coEvents.clear();
		CalcPotentialCollisions(_coObjects, coEvents);

		//Xử lý va chạm với từng đối tượng
		if (coEvents.size() == 0)
		{
			x += dx;
			y += dy;
		}
		else
		{
			float min_tx, min_ty, nx = 0, ny = 0;
			FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny);

			x += min_tx * dx + nx * 0.4f;
			if (ny < 0)
				y += min_ty * dy + ny * 0.4f;
			else
				y += dy;

			for (int i = 0; i < coEventsResult.size(); i++)
			{
				LPCOLLISIONEVENT e = coEventsResult.at(i);
				if (dynamic_cast<CItem*>(e->obj))
				{
					CollisionWithItem(static_cast<CItem*>(e->obj)->GetItemType());
					e->obj->Death();
				}
				else if (!isOnStair && dynamic_cast<CBrick*>(e->obj))
				{
					if (ny == -1)
					{
						vy = 0.1f;
						dy = vy * dt;

						if (isJumping)
						{
							isJumping = false;
							y -= 18;
						}
					}
				}

			}
		}

		for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];

	#pragma endregion	

	if (isAutoGo)
	{
		if (abs(x - AutoGo_X_Backup) >= AutoGo_X_Distance)
		{
			isAutoGo = false;
			x = x - (abs(x - AutoGo_X_Backup) - AutoGo_X_Distance);
			vx = 0;
			isWalking = 0;
			nx = DirectEnd;
		}
	}

	//Block Simon on Camera

	float boundR, boundL;
	CCamera::GetInstance()->GetBoundLeftRight(boundL, boundR);

	x = (x < boundL - 12) ? boundL - 12 : x;
	x = (x > boundR - 12) ? boundR - 12 : x;
	//x = (x + SIMON_BBOX_WIDTH > CCamera::GetInstance()->GetBoundaryLeft()) ? CCamera::GetInstance()->GetBoundaryLeft() - SIMON_BBOX_WIDTH : x;

}

void CSimon::Render()
{
	//CCamera::GetInstance()->SetPosition(x - SCREEN_WIDTH / 2, 0);
	RenderBoundingBox();
	D3DXVECTOR2 pos(x, y);
	int alpha = 255;
	/*if (aniState == SIMON_ANI_STANDING_ATTACKING)
		DebugOut(L"Ani Đánh\n");*/
	if (IsDead)
	{
		if (nx<0)
			animation_set->at(aniState)->Render(pos.x, pos.y, false, alpha);
		else
			animation_set->at(aniState)->Render(pos.x, pos.y, true, alpha);
	}
	else
	{
		if (this->IsFreeze())
		{
			animation_set->at(aniState)->KeepFramePresent();
			if (nx < 0)
				animation_set->at(aniState)->Render(pos.x, pos.y, false, alpha, rand() % 256, rand() % 256, rand() % 256);
			else
				animation_set->at(aniState)->Render(pos.x, pos.y, true, alpha, rand() % 256, rand() % 256, rand() % 256);
		}
		else
		{
			if (nx < 0)
				animation_set->at(aniState)->Render(pos.x, pos.y, false, alpha);
			else
				animation_set->at(aniState)->Render(pos.x, pos.y, true, alpha);
		}
	}

	for (auto item : listWeapon)
	{
		if (!item.second->IsFinish())
		{
			item.second->Render();
		}
	}
}

void CSimon::SetFreeze(bool _isFreeze)
{
	isFreeze = _isFreeze;
	TimeFreeze = GetTickCount64();
}

bool CSimon::IsFreeze()
{
	if (GetTickCount64() - TimeFreeze >= TIME_FREEZE_MAX)
	{
		isFreeze = false;
		return false;
	}
	return true;
}

void CSimon::ResetSimon()
{
	if (lifeSimon > 0)
	{
		lifeSimon--;
		ResetPositionBackup();
	}
	else
	{
		lifeSimon = SIMON_DEFAULT_LIFE;
	}
	secondaryweaponcurrent = Null;
	isuntouchable = 0;
	weaponcurrent = WHIP;
	for (auto item : listWeapon)
	{
		SAFE_DELETE(item.second);
	}
	listWeapon.clear();
	listWeapon[WHIP] = new CWhip();
	heart = 5;
	HPSimon = 16;
	isWalking = 0;
	isJumping = 0;
	isAttacking = 0;
	isSitting = 0;
	isFreeze = 0;
	isuntouchable = 0;
}

void CSimon::SetInjured(ObjectType _obj)
{

}

void CSimon::Attack(bool isMainWeapon)
{
	if (isJumping && vy > 0.1) 
		return;
	if (aniState == SIMON_ANI_SITTING_ATTACKING || aniState == SIMON_ANI_STANDING_ATTACKING)// || isAttacking) 
		return;
	if(isMainWeapon)
	{
		if (listWeapon.at(WHIP)->Attack(x, y, nx))
		{
			isAttacking = true;
			//DebugOut(L"Ấn đánh\n");
		}
	}
	else
	{
		if (secondaryweaponcurrent != Null)
		{
			if (heart > 0)
			{
				for (pair<int, CWeapon*> item : listWeapon)
				{
					if (!dynamic_cast<CWhip*> (item.second))
					{
						if (item.second->Attack(x, y, nx))
						{
							DebugOut(L"dcd");
							isAttacking = true;
							heart--;
							return;
						}
					}
				}


				/*if (listWeapon.at(secondaryweaponcurrent)->Attack(x, y, nx))
				{
					isAttacking = true;
					heart--;
				}*/
			}
		}
	}
}

void CSimon::WantUpOnStair()
{
	if (isOnStair||isJumping) return;
	isWantOnBottomstair = 1;
}

void CSimon::CollisionWithItem(ObjectType _type)
{
	switch (_type)
	{
	case HEART_BIG_ITEM:
		AddHeart(5);
		break;
	case HEART_ITEM:
		AddHeart(1);
		break;
	case WHIP_ITEM:
		if (static_cast<CWhip*>(listWeapon.at(WHIP))->GetLever() == 3) break;
		static_cast<CWhip*>(listWeapon.at(WHIP))->SetLever(static_cast<CWhip*>(listWeapon.at(WHIP))->GetLever() + 1);
		SetFreeze(1);
		break;
	case KNIFE_ITEM:
		clearListSecondWeapon();
		for (int i = 0; i < amountSecondWeapon; i++)
		{
			listWeapon[KNIFE + i] = new CKnife();
		}
		secondaryweaponcurrent = KNIFE;

		break;
	case AXE_ITEM:
		clearListSecondWeapon(); 
		for (int i = 0; i < amountSecondWeapon; i++)
		{
			listWeapon[AXE+i] = new CAxe();
		}
		secondaryweaponcurrent = AXE;

		break;
	case BOOMERANG_ITEM:
		clearListSecondWeapon();
		for (int i = 0; i < amountSecondWeapon; i++)
		{
			listWeapon[BOOMERANG+i] = new CBoomerang();
		}
		secondaryweaponcurrent = BOOMERANG;

		break;
	case GUNPOWDER_ITEM:
		clearListSecondWeapon();
		for (int i = 0; i < amountSecondWeapon; i++)
		{
			listWeapon[GUNPOWDER+i] = new CGunpowder();
		}
		secondaryweaponcurrent = GUNPOWDER;

		break;
	case DOUBLE_WEAPON:
		if (secondaryweaponcurrent != NULL)
		{
			switch (secondaryweaponcurrent)
			{
			case KNIFE:
				listWeapon[KNIFE + 1] = new CKnife();
				break;
			case AXE:
				listWeapon[AXE + 1] = new CAxe();
				break;
			case BOOMERANG:
				listWeapon[BOOMERANG + 1] = new CBoomerang();
				break;
			case GUNPOWDER:
				listWeapon[GUNPOWDER + 1] = new CGunpowder();
			}
		}
		amountSecondWeapon = 2;
		break;
	}
}

void CSimon::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x + 12;
	t = y;
	r = l + SIMON_BBOX_WIDTH - 27;
	if (isSitting)
	{
		b = t + SIMON_BBOX_SITTING_HEIGHT;
	}
	else
	{
		if ((isJumping && !isAttacking) && vy < 10)
			b = y + SIMON_BBOX_JUMPING_HEIGHT;
		else
			b = y + SIMON_BBOX_HEIGHT;
	}
}

void CSimon::Left()
{
	if (isAttacking||IsFreeze())
		return;
	nx = -1;
}

void CSimon::Right()
{
	if (isAttacking||IsFreeze())
		return;
	nx = 1;
}

void CSimon::Go()
{
	if (IsFreeze()||(isAttacking&&!isJumping)||isWantOnBottomstair)return;
	if(isOnStair == 0)
		vx = SIMON_WALKING_SPEED * nx;
	else
	{
		if (nx == 1 && directOnStair == 1)
		{
			vx = SIMON_SPEED_ONSTAIR;
			vy = -SIMON_SPEED_ONSTAIR;
		}
		else if (nx == -1 && directOnStair == 1)
		{
			vx = -SIMON_SPEED_ONSTAIR;
			vy = SIMON_SPEED_ONSTAIR;
		}
		else if (nx == 1 && directOnStair == -1)
		{
			vx = SIMON_SPEED_ONSTAIR;
			vy = SIMON_SPEED_ONSTAIR;
		}
		else if (nx == -1 && directOnStair == -1)
		{
			vx = -SIMON_SPEED_ONSTAIR;
			vy = -SIMON_SPEED_ONSTAIR;
		}
		//if (!isWalking)
			isProcessStair = 1;
	}
	isWalking = 1;
}

void CSimon::Sit()
{
	if (IsFreeze())return;
	if (isJumping&&aniState == SIMON_ANI_JUMPING) 
		return;
	if (isAttacking) return;

	if (!isContainTopTrigger&&!isWantOnTopstair)
	{
		vx = 0;
		isWalking = 0;
		if (!isSitting)
			y = y + PULL_UP_SIMON_SITTING;
		isSitting = 1;
	}
	else
	{
		isWantOnTopstair = 1;
		Go();
	}
}

void CSimon::ResetSit()
{
	if (IsFreeze())return;
	if (isSitting)
	{
		isSitting = 0;
		y = y - PULL_UP_SIMON_SITTING;
	}
}

void CSimon::Jump()
{
	if (IsFreeze())return;
	if (isJumping || isAttacking || isSitting)
		return;
	vy = - SIMON_VJUMP;
	isJumping = true;
}

void CSimon::Stop()
{
	if (IsFreeze() || isAutoGo)return;
	if (!isOnStair)
	{
		vx = 0;
		isWalking = 0;
		//isWantOnstair = 0;
		if (isSitting && !isAttacking)
		{
			y = y - PULL_UP_SIMON_SITTING;
			isSitting = 0;
		}
	}
	else
	{
		//vy =  vx = 0;		
		//isWalking = 0;
	}
}