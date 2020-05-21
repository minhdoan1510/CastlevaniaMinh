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
#include "Enemy.h"

#include "Camera.h"
#include "Bridge.h"

CSimon* CSimon::intance = NULL;

CSimon::CSimon()
{
	//sprite = CSprites::GetInstance()->Get(SIMON);
	//texture = CTextureManager::GetInstance()->Get(SIMON);
	isuntouchable = 0;
	nx = 1;
	objType = SIMON;
	weaponcurrent = WHIP;	
	listWeapon[WHIP] = new CWhip();
	heart = SIMON_DEFAULT_HEART;
	lifeSimon = SIMON_DEFAULT_LIFE;
	HPSimon = SIMON_DEFAULT_HP;
	isWalking = 0;
	//CCamera::GetInstance()-> SetPosition(0, 0);
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
	isInjured = 0;
	flagRenderUntouch = 0;
	isEnterGameIntro = 0;
	timeOnceStair = 0;
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
	if (timeAniDeadth == 0)
	{
		timeAniDeadth = GetTickCount();
		StartUntouchable();
		vx = vy = 0;
	}
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
	aniState = SIMON_ANI_WALKING;
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
	if (GetTickCount() - untouchable_start > SIMON_UNTOUCHABLE_TIME)
	{
		isuntouchable = 0;
	}
	// AABB check incontain stair trigger 
	objAddAfterUpdate.clear();
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
								Go();
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
								Go();
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
						CSceneManager::GetInstance()->StartPassScene();
						aniState = SIMON_ANI_IDLE;
						return;
					}
					break;
				case ITEM_HIDDEN_TRIGGER:
					if (IsContain(this->GetBBox(), coObjects->at(i)->GetBBox()))
					{
						if (static_cast<CTrigger*>(coObjects->at(i))->IsActive())
						{
							objAddAfterUpdate.push_back(static_cast<CTrigger*>(coObjects->at(i))->GetItem());
							static_cast<CTrigger*>(coObjects->at(i))->SetIsActive(0);
						}
					}
					break;
				case DEADTH_TRIGGER:
					if (!isOnStair&&IsContain(this->GetBBox(), coObjects->at(i)->GetBBox()))
					{
						IsDead = true;
						ResetSimon();
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

	if (!isOnStair)//||static_cast<CPlayScene*>(CSceneManager::GetInstance()->GetCurrentScene())->IsTransScene())
	{
		if (isInjured)
		{
			vy += SIMON_GRAVITY * dt / 4;
		}
		else
		{
			if (isJumping)
				vy += SIMON_GRAVITY_JUMPING * dt;
			else
				vy += SIMON_GRAVITY * dt; //trọng lực
		}
	}

	//AABB Enemy
	if (!isuntouchable)
	{
		for (size_t i = 0; i < coObjects->size(); i++)
			if (dynamic_cast<CEnemy*>(coObjects->at(i))&& static_cast<CEnemy*>(coObjects->at(i))->IsActive())
			{
				if (IsContain(this->GetBBox(), coObjects->at(i)->GetBBox()))
				{
					CollisionWithEnemy(static_cast<CEnemy*>(coObjects->at(i)));
					break;
				}
			}
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
			vx = vy = 0;
			StartUntouchable();
			if (GetTickCount() - timeAniDeadth >= TIME_ANI_DEATH)
			{
				timeAniDeadth = 0;
				isAniDead = 0;
				IsDead = true;
				ResetSimon();
				return;
			}
		}
	else if (isInjured)
		{
			aniState = SIMON_ANI_INJURED;
		}
		else
		
		{
			if (isOnStair)
			{
				if (isAttacking)
				{
					if (lifetimeAttack == 0)
						lifetimeAttack = GetTickCount();
					if (nx == directOnStair)
					{
						aniState = SIMON_ANI_STAIR_UP_ATTACKING;
					}
					else
					{
						aniState = SIMON_ANI_STAIR_DOWN_ATTACKING;
					}

					//if (animation_set->at(aniState)->GetDoneFrameFinal())
					if (GetTickCount() - lifetimeAttack >= TIME_SIMON_ATTACK)
					{
						lifetimeAttack = 0;
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
				else if (isWalking||isProcessStair)
				{
					if (nx == directOnStair)
					{
						aniState = SIMON_ANI_STAIR_UP;
					}
					else
					{
						aniState = SIMON_ANI_STAIR_DOWN;
					}
					
					//if (animation_set->at(aniState)->GetDoneFrameFinal())
					if (GetTickCount() - timeOnceStair >= TIME_ONCE_STAIR)
					{
						animation_set->at(aniState)->ResetFarmeCurrent();
						numberStair++;
						isProcessStair = 0;
						vy = vx = 0;
						dx = dy = 0;
						isWalking = 0;
						timeOnceStair = 0;
						if (y_OnStair == 0)
							y_OnStair=y;
						h_StepStair = y - y_OnStair;
						//DebugOut(L" %4.2f ", h_StepStair);
						if (directOnStair == 1)
						{
							if (h_StepStair > 0 && h_StepStair != 16)
							{
								x += h_StepStair - 16;
								y -= h_StepStair - 16;

								//DebugOut(L" %4.2f \n", h_StepStair - 16);
							}
							else if (h_StepStair < 0 && h_StepStair != -16)
							{
								x += h_StepStair + 16;
								y -= h_StepStair + 16;
								//DebugOut(L" %4.2f \n", h_StepStair + 16);
							}
						}
						else
						{
							if (h_StepStair > 0 && h_StepStair != 16)
							{
								x -= h_StepStair - 16;
								y -= h_StepStair - 16;

								//DebugOut(L" %4.2f \n", h_StepStair - 16);
							}
							else if (h_StepStair < 0 && h_StepStair != -16)
							{
								x -= h_StepStair + 16;
								y -= h_StepStair + 16;

								//DebugOut(L" %4.2f \n", h_StepStair + 16);
							}
						}
						y_OnStair = y;


						/*if (directOnStair == 1)
						{
							if (h_StepStair > 0 && h_StepStair != 16)
							{
								x += h_StepStair - 16;
								y -= h_StepStair - 16;

								DebugOut(L" %4.2f \n", h_StepStair - 16);
							}
							else if (h_StepStair < 0 && h_StepStair != -16)
							{
								x -= h_StepStair + 16;
								y += h_StepStair + 16;
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

						h_StepStair = 0;*/
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
				y_OnStair = 0;
				isProcessStair = 0;
				numberStair = 0;
				if (isSitting == true)
				{
					if (isAttacking == true)
					{
						if (lifetimeAttack == 0)
							lifetimeAttack = GetTickCount();
						if (aniState != SIMON_ANI_SITTING_ATTACKING)
						{
							aniState = SIMON_ANI_SITTING_ATTACKING;
						}
						else
						{
							//if (animation_set->at(aniState)->GetDoneFrameFinal())//sửa lại
							if (GetTickCount() - lifetimeAttack >= TIME_SIMON_ATTACK)
							{
								lifetimeAttack = 0;
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
						if (lifetimeAttack == 0)
							lifetimeAttack = GetTickCount();
						if (aniState != SIMON_ANI_STANDING_ATTACKING)
						{
							aniState = SIMON_ANI_STANDING_ATTACKING;
							animation_set->at(aniState)->ResetFarmeCurrent();
						}
						else
						{
							//if (animation_set->at(aniState)->GetDoneFrameFinal())
							if (GetTickCount() - lifetimeAttack >= TIME_SIMON_ATTACK)
							{
								lifetimeAttack = 0;
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
								aniState = SIMON_ANI_JUMPING;
							}
							else
							{
								if (isEnterGameIntro)
									aniState = SIMON_ANI_ENTER_GAME;
								else
									aniState = SIMON_ANI_IDLE;
							}
						}
					}
				}
			}
		}
	#pragma endregion		

#pragma region collisionHandle
	

	//AABB Item 
	for (size_t i = 0; i < coObjects->size(); i++)
		if (dynamic_cast<CItem*>(coObjects->at(i)))
		{
			if (!static_cast<CItem*>(coObjects->at(i))->GetDeath()&&IsContain(this->GetBBox(), coObjects->at(i)->GetBBox()))
			{
				CollisionWithItem(static_cast<CItem*>(coObjects->at(i))->GetItemType());
				coObjects->at(i)->Death();
			}
		}

	vector<LPGAMEOBJECT>* _coObjects = new vector<LPGAMEOBJECT>();
	for (int i = 0; i < coObjects->size(); i++)
	{
		if (isOnStair && dynamic_cast<CBrick*>(coObjects->at(i)))
			continue; 
		if (dynamic_cast<CEnemy*>(coObjects->at(i)))
			continue;
		_coObjects->push_back(coObjects->at(i));
	}


	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	vector<CGameObject*>* colliable_objects = new vector<CGameObject*>(); // Filter Object Bridge Brick

	for (int i = 0; i < _coObjects->size(); i++)
		if (dynamic_cast<CBrick*> (_coObjects->at(i)) || dynamic_cast<CBridge*> (_coObjects->at(i)))
			colliable_objects->push_back(_coObjects->at(i));


	coEvents.clear();
	CalcPotentialCollisions(colliable_objects, coEvents);

	//Xử lý va chạm với từng đối tượng
	int nx_Back = nx;
	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
	}
	else
	{
		float min_tx, min_ty, _nx = 0, ny = 0;
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, _nx, ny);

		if (ny < 0)
			y += min_ty * dy + ny * 0.4f;
		else
			y += dy;

		if (coEventsResult.size() <= 1)
			x += min_tx * dx + _nx * 0.4f;
		else
		{
			float xtemp, ytemp;
			bool isStopX=0;
			coEventsResult.at(0)->obj->GetPosition(xtemp, ytemp);
			for (int i = 1; i < coEventsResult.size(); i++)
			{
				float xtemp1, ytemp1;
				coEventsResult.at(i)->obj->GetPosition(xtemp1, ytemp1);
				if (ytemp != ytemp1)
				{
					isStopX = true;
					break;
				}
			}
			if(isStopX)
				x += min_tx * dx + _nx * 0.4f;
			else
				x += dx;
				
		}
			
		for (int i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult.at(i);
			
			if (!isOnStair &&( dynamic_cast<CBrick*>(e->obj)))
			{
				if (ny == -1)
				{
					vy = 0.1f;
					dy = vy * dt;
					if (isInjured)
					{
						isInjured = 0;
						nx = -nx_Back;
					}
					if (isJumping)
					{
						isJumping = false;
						y -= 18;
						//vx = 0;// check again
					}
				}
			}
			else if (!isOnStair && dynamic_cast<CBridge*>(e->obj))
			{
				if (ny <0)
				{
					vy = 0.1f;
					dy = vy * dt;
					if (isInjured)
					{
						isInjured = 0;
						nx = -nx_Back;
					}
					if (isJumping)
					{
						isJumping = false;
						y -= 18;
					}
				}
			}

		}
	}

	for (int i = 0; i < coEvents.size(); i++) delete coEvents[i];

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

void CSimon::UpdateAutoGo(DWORD dt)
{
	vy = 0;
	CGameObject::Update(dt);
	x += dx;
	y += dy; 
	if (isAutoGo)
	{
		if (abs(x - AutoGo_X_Backup) >= AutoGo_X_Distance)
		{
			isAutoGo = false;
			x = x - (abs(x - AutoGo_X_Backup) - AutoGo_X_Distance);
			vx = 0;
			isWalking = 0;
			nx = DirectEnd;
			aniState = SIMON_ANI_IDLE;
		}
	}
}

void CSimon::Render()
{
	//CCamera::GetInstance()->SetPosition(x - SCREEN_WIDTH / 2, 0);
	RenderBoundingBox();
	D3DXVECTOR2 pos(x, y);
	int alpha = 255;
	if (isuntouchable)
	{
		if (flagRenderUntouch)
			alpha = 200;
		else
			alpha = 128;
		flagRenderUntouch = !flagRenderUntouch;
	}

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
			if (nx < 0)
				animation_set->at(aniState)->RenderFrame(animation_set->at(aniState)->GetCurrentFrame(),pos.x, pos.y, false, alpha, rand() % 256, rand() % 256, rand() % 256);
			else
				animation_set->at(aniState)->RenderFrame(animation_set->at(aniState)->GetCurrentFrame(),pos.x, pos.y, true, alpha, rand() % 256, rand() % 256, rand() % 256);
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
	heart = SIMON_DEFAULT_HEART;
	HPSimon = SIMON_DEFAULT_HP;
	isWalking = 0;
	isJumping = 0;
	isAttacking = 0;
	isSitting = 0;
	isFreeze = 0;
	isuntouchable = 0;
	isOnStair = 0;
	//IsDead = 0;
}

void CSimon::SetInjured(int damage, int direct)
{
	HPSimon -= damage;
	if (HPSimon <= 0)
	{
		DeathSimon();
		HPSimon = 0;
	}
	if (direct != 0)
		SetStateAttacked(direct);
	else
		StartUntouchable();
}

void CSimon::SetStateAttacked(int direct)
{
	if (!isOnStair) {
		if (direct==1)
		{
			vx = SIMON_VJUMP / 3;
			nx = 1;
		}
		else if(direct == -1)
		{
			vx = -SIMON_VJUMP / 3;
			nx = -1;
		}
		vy = -SIMON_VJUMP;

		isInjured = 1;
		aniState = SIMON_ANI_INJURED;
	}
	StartUntouchable();
}

void CSimon::Attack(bool isMainWeapon)
{
	/*if (isJumping && vy > 0.1) 
		return;*/
	
	if (aniState == SIMON_ANI_SITTING_ATTACKING || aniState == SIMON_ANI_STANDING_ATTACKING)// || isAttacking) 
		return;
	
	if(isMainWeapon)
	{
		if (listWeapon.at(WHIP)->Attack(x, y, nx))
		{
			isAttacking = true;

			if (isJumping)
			{
				y -= PULL_UP_SIMON_SITTING;// kéo simon lên. tránh overlaping
			}
			//DebugOut(L"Ấn đánh\n");
			isAttackingMainWeapon = 1;
		}
	}
	else
	{
		if (secondaryweaponcurrent != Null)
		{
			if (heart > 0&& !isOnStair)
			{
				for (pair<int, CWeapon*> item : listWeapon)
				{
					if (!dynamic_cast<CWhip*> (item.second))
					{
						if (item.second->Attack(x, y, nx))
						{
							//DebugOut(L"dcd");
							isAttacking = true;
							isAttackingMainWeapon = 0;
							if (isJumping)
							{
								y -= PULL_UP_SIMON_SITTING;// kéo simon lên. tránh overlaping
							}
							heart--;
							return;
						}
					}
				}
			}
		}
	}
}

bool CSimon::IsAttack()
{
	return isAttacking;
}

bool CSimon::IsAttackMainWeapon()
{
	return isAttackingMainWeapon;
}

unordered_map<int, CWeapon*> CSimon::GetListWeapon()
{
	return listWeapon;
}

void CSimon::WantUpOnStair()
{
	if (isOnStair || isJumping) return;
	if (isAutoGo) return;
	//DebugOut(L"csd\n");
	if (isContainBottomTrigger)
		isWantOnBottomstair = 1;
}

void CSimon::CollisionWithItem(ObjectType _type)
{
	switch (_type)
	{
	case HEART_BIG_ITEM:
		AddHeart(AMOUNT_HEART_BIG);
		break; 
	case CHICKEN_ITEM:
		HPSimon += HP_INCREASE_CHICKEN;
		break;
	case HEART_ITEM:
		AddHeart(AMOUNT_HEART);
		break;
	case WHIP_ITEM:
		SetFreeze(1);
		if (static_cast<CWhip*>(listWeapon.at(WHIP))->GetLever() == 3) break;
		static_cast<CWhip*>(listWeapon.at(WHIP))->SetLever(static_cast<CWhip*>(listWeapon.at(WHIP))->GetLever() + 1);
		
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

void CSimon::CollisionWithEnemy(CGameObject* obj)
{
	//SetInjured(static_cast<CEnemy*>(obj)->GetDamageofEnemy(),);
	
	if (!isOnStair) {
		RECT rectEnemy = obj->GetBBox();
		RECT rectThis = this->GetBBox();
		if ((rectEnemy.right + rectEnemy.left) / 2 < (rectThis.right + rectThis.left) / 2)
		{
			SetInjured(static_cast<CEnemy*>(obj)->GetDamageofEnemy(), 1);
			//SetStateAttacked(1);
			/*vx = SIMON_VJUMP / 3;
			nx = 1;*/
		}
		else
		{
			SetInjured(static_cast<CEnemy*>(obj)->GetDamageofEnemy(), -1);
			//SetStateAttacked(-1);
			/*vx = -SIMON_VJUMP / 3;
			nx = -1;*/
		}
		//vy = -SIMON_VJUMP;

	//isInjured = 1;
	//aniState = SIMON_ANI_INJURED;
	}
	else
		SetInjured(static_cast<CEnemy*>(obj)->GetDamageofEnemy(), 0);
	//StartUntouchable();
	if (!static_cast<CEnemy*>(obj)->IsArmor())
	{
		static_cast<CEnemy*>(obj)->Death(1);
	}
}

void CSimon::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x + 15;
	t = y;
	r = x + SIMON_BBOX_WIDTH - 14;
	if (isSitting)
	{
		b = t + SIMON_BBOX_SITTING_HEIGHT;
	}
	else
	{
		if ((isJumping && !isAttacking))// && vy < 10)
			b = y + SIMON_BBOX_JUMPING_HEIGHT;
		else// if ((isJumping && vy<0)||(!isJumping))
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
	if (IsFreeze()||(isAttacking&&!isJumping)||isWantOnBottomstair|| (isAttacking && isOnStair))return;
	if(isOnStair == 0)
		vx = SIMON_WALKING_SPEED * nx;
	else
	{
		//DebugOut(L"thang\n");
		y_OnStair = y;
		timeOnceStair = GetTickCount();
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

void CSimon::EnterIntroGameState(bool isEnter)
{
	isEnterGameIntro = isEnter;
}