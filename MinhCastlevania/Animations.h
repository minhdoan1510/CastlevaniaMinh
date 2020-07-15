#pragma once
#include <Windows.h>
#include <d3dx9.h>
#include <unordered_map>

#include "Sprites.h"

class CAnimationFrame
{
	LPSPRITE sprite;
	DWORD time;

public:
	CAnimationFrame(LPSPRITE sprite, int time) { this->sprite = sprite; this->time = time; }
	DWORD GetTime() { return time; }
	LPSPRITE GetSprite() { return sprite; }
};

typedef CAnimationFrame *LPANIMATION_FRAME;

class CAnimation
{
	DWORD lastFrameTime;
	DWORD lifeFrameTime;
	int currentFrame;
	int defaultTime;
	vector<LPANIMATION_FRAME>* frames;
	bool DoneFrameFinal = false;
	bool isStop;
public:
	CAnimation(int _defaultTime = 100);
	CAnimation(vector<LPANIMATION_FRAME> * _frames, int _defaultTime);
	CAnimation* Clone();
	void Add(int spriteId, DWORD time = 0);

	void Render(float x, float y,bool flipX,  int alpha = 255 ,int R = 255, int G = 255, int B= 255);
	void RenderFrame(int idFrame, float x, float y, bool flipX, int alpha = 255, int R = 255, int G = 255, int B = 255);
	int GetLastFrame();
	int GetCurrentFrame();
	bool IsFinalFrame() { return currentFrame == frames->size()-1; }
	int GetFrameTime(){}
	bool GetDoneFrameFinal() {
		return DoneFrameFinal; }
	DWORD GetLifeFrameTime() { return lastFrameTime; }
	void FreezeFrame();
	void SetStop(bool _isStop);
	void ResetFarmeCurrent() { currentFrame = -1; DoneFrameFinal = 0; }
};

typedef CAnimation *LPANIMATION;

class CAnimations
{
	static CAnimations * _instance;
	unordered_map<int, LPANIMATION> animations;
	unordered_map<int, LPANIMATION> animationsScene;
public:
	//void Add(int id, LPANIMATION ani);
	LPANIMATION Get(int id);
	void Clear();
	void ClearAniScene();
	void Add(int id, LPANIMATION ani, bool isScene);
	static CAnimations * GetInstance();
};

typedef unordered_map<int,LPANIMATION> CAnimationSet;
typedef CAnimationSet* LPANIMATION_SET;

class CAnimationSets
{
	static CAnimationSets * _instance;
	unordered_map<int, LPANIMATION_SET> animation_sets;
	unordered_map<int, LPANIMATION_SET> animation_setsScene;
public:
	static CAnimationSets * GetInstance();
	CAnimationSets();
	void LoadResource(string Folderpath);
	void LoadResourceScene(string Folderpath);
	void Add(int id, LPANIMATION_SET ani_set, bool isScene);
	void Clear();
	void ClearAniScene();
	//void Add(int id, LPANIMATION_SET ani);
	LPANIMATION_SET Get(ObjectType id);
};