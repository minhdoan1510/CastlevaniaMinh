﻿#include "Camera.h"
#include "MapManager.h"
#include "SceneManager.h"
#include<cmath>

#define W_CAM_BOUND 16
CCamera* CCamera::Intance = NULL;

CCamera::CCamera(int w, int h)
{
	width = w;
	height = h;
	floor = 1; 
	isAutoCam = 0;
	isLockCam = 0;
}

CCamera::~CCamera()
{
}

CCamera* CCamera::GetInstance()
{
	if (Intance == NULL)
		Intance = new CCamera(SCREEN_WIDTH, SCREEN_HEIGHT-PULL_SCREEN_Y);
	return Intance;
}

D3DXVECTOR2 CCamera::Transform(float x, float y)
{
	return D3DXVECTOR2(x - xC, y - yC);
}

void CCamera::SetPosition(float x, float y,int _floor)
{
	if (isAutoCam||isLockCam) return;
	if (x <= boundLeft)
		xC = boundLeft;
	else if (x > boundRight - width + W_CAM_BOUND)
		xC = boundRight - width + W_CAM_BOUND;
	else
		xC = x;
	yC = y;
	floor = _floor;
}

void CCamera::SetDefaultCam()
{
	xC = yC = 0;
}

float CCamera::GetXCam()
{
	return xC;
}

float CCamera::GetYCam()
{
	return yC;
}

int CCamera::GetCurrentFloor()
{
	return floor;
}

int CCamera::GetWidth()
{
	return width;
}

int CCamera::GetHeight()
{
	return height;
}

RECT CCamera::GetRectCam()
{
	RECT r;
	r.left = xC - W_CAM_BOUND;
	r.top = yC;
	r.right = xC + width ;
	r.bottom = yC + height;
	return r;
}

void CCamera::SetBoundSize(float _w, float _h)
{
	if (_w < width)
		boundWidth = width;
	else
		boundWidth = _w;
	boundHeight = _h;
}

float CCamera::GetBoundHeight()
{
	return boundHeight;
}

float CCamera::GetBoundWidth()
{
	return boundWidth;
}

void CCamera::SetBoundLeftRight(float _l, float _r)
{
	boundLeft = _l;
	boundRight = _r;
}

void CCamera::GetBoundLeftRight(float& _l, float& _r)
{
	_l = boundLeft;
	_r = boundRight;
}

bool CCamera::IsContainCam(RECT rect1)
{
	RECT rect2 = this->GetRectCam();
	if (rect1.left > rect2.right || rect1.right < rect2.left || rect1.top > rect2.bottom || rect1.bottom < rect2.top)
	{
		return false;
	}
	return true;
}

bool CCamera::IsAutoCam()
{
	return isAutoCam;
}

void CCamera::AutoCamX(float _distance, int _direct)
{
	isAutoCam = 1;
	distanceAutoCam = _distance;
	directAutoCam = _direct;
	X_BackupAutoCam = xC;
}


void CCamera::UpdateAutoCam(DWORD dt)
{
	xC += SPEED_AUTO_CAM * dt * directAutoCam;
	if (abs(xC - X_BackupAutoCam) >= distanceAutoCam)
	{
		//xC += distanceAutoCam - (xC - X_BackupAutoCam);
		isAutoCam = 0;
	}
}

void CCamera::LockCam()
{
	isLockCam = true;
}

void CCamera::UnlockCam()
{
	isLockCam = false;
}
