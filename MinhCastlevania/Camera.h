#pragma once
#include "GameDefine.h"

#define SPEED_AUTO_CAM	0.08f

class CCamera
{
	static CCamera* Intance;
	int xC;
	int yC;
	int width;
	int height;
	float boundWidth; 
	float boundHeight;
	float boundLeft;
	float boundRight;
	int floor;
	bool isAutoCam;
	float distanceAutoCam;
	int directAutoCam;
	float X_BackupAutoCam;
public:
	static CCamera* GetInstance();
	CCamera(int w, int h);
	~CCamera();
	D3DXVECTOR2 Transform(float x, float y);
	void SetPosition(float x, float y, int _floor);
	float GetXCam();
	float GetYCam();
	int GetCurrentFloor();
	int GetWidth();
	int GetHeight();
	RECT GetRectCam();
	void SetBoundSize(float _w, float _h);	
	float GetBoundHeight();
	float GetBoundWidth();
	void SetBoundLeftRight(float _l, float _r);
	void GetBoundLeftRight(float& _l, float& _r);
	bool IsContainCam(RECT rect);
	bool IsAutoCam();
	void AutoCamX(float _distance, int direct);
	void UpdateAutoCam(DWORD dt);
};



