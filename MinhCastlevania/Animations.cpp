#include "Animations.h"
#include "Utils.h"
#include "TextureManager.h"
#include <fstream>
#include "strsafe.h"

CAnimationSets * CAnimationSets::_instance = NULL;

CAnimation::CAnimation(int _defaultTime)
{
	lifeFrameTime = 0;
	defaultTime = _defaultTime;
	lastFrameTime = -1;
	currentFrame = -1;
	frames = new vector<LPANIMATION_FRAME>();
}

CAnimation::CAnimation(vector<LPANIMATION_FRAME>* _frames, int _defaultTime)
{
	lifeFrameTime = 0;
	defaultTime = _defaultTime;
	lastFrameTime = -1; 
	currentFrame = -1;
	frames = _frames;
}

CAnimation* CAnimation::Clone()
{
	return new CAnimation(frames, defaultTime);
}

void CAnimation::Add(int spriteId, DWORD time)
{
	int t = time;
	if (time == 0) t = this->defaultTime;

	LPSPRITE sprite = CSprites::GetInstance()->Get(spriteId);

	if (sprite == NULL)
	{
		DebugOut(L"[ERROR] Sprite ID %d cannot be found!\n", spriteId);
	}

	LPANIMATION_FRAME frame = new CAnimationFrame(sprite, t);
	frames->push_back(frame);
}

void CAnimation::Render(float _x, float _y, bool flipX ,  int alpha, int R, int G, int B)
{
	DWORD now = GetTickCount64();
	if (currentFrame == -1)
	{
		currentFrame = 0;
		lastFrameTime = now;
		lifeFrameTime = 0;
	}
	else
	{
		DoneFrameFinal = 0;
		DWORD t = frames->at(currentFrame)->GetTime();
		if (now - lastFrameTime > t)
		{
			lifeFrameTime = now - lastFrameTime;
			currentFrame++;
			lastFrameTime = now;

			if (currentFrame == frames->size() - 1)
			{
				DoneFrameFinal = 1;
			}
			else
				DoneFrameFinal = 0;

			if (currentFrame == frames->size())
			{
				currentFrame = 0;
			}
			
		}
	}
	if(flipX)
		frames->at(currentFrame)->GetSprite()->DrawFlipX(_x, _y, alpha, R, G, B);
	else
	frames->at(currentFrame)->GetSprite()->Draw(_x, _y, alpha, R, G, B);
}

void CAnimation::RenderFrame(int idFrame, float _x, float _y, bool flipX, int alpha, int R, int G, int B)
{
	if (idFrame == -1)
		idFrame = 0;
	if (flipX)
		frames->at(idFrame)->GetSprite()->DrawFlipX(_x, _y, alpha, R, G, B);
	else
		frames->at(idFrame)->GetSprite()->Draw(_x, _y, alpha, R, G, B);
}

int CAnimation::GetLastFrame()
{
	return frames->size()-1;
}

int CAnimation::GetCurrentFrame()
{

	return currentFrame;
}

void CAnimation::FreezeFrame()
{
	currentFrame--;
	if (currentFrame == -1|| currentFrame == -2)
	{
		currentFrame = frames->size() - 1;
	}
}

CAnimations * CAnimations::_instance = NULL;

CAnimations * CAnimations::GetInstance()
{
	if (_instance == NULL) _instance = new CAnimations();
	return _instance;
}

void CAnimations::Add(int id, LPANIMATION ani, bool isScene)
{
	if(isScene)
		animationsScene[id] = ani;
	else
		animations[id] = ani;
}

LPANIMATION CAnimations::Get(int id)
{
	LPANIMATION ani = animations[id];
	if (ani == NULL)
		ani = animationsScene[id];
	if (ani == NULL)
		DebugOut(L"[ERROR] Failed to find animation id: %d\n", id);
	return ani;
}

void CAnimations::Clear()
{
	for (auto x : animations)
	{
		LPANIMATION ani = x.second;
		delete ani;
	}

	animations.clear();
}

void CAnimations::ClearAniScene()
{
	for (auto x : animationsScene)
	{
		LPANIMATION ani = x.second;
		delete ani;
	}

	animationsScene.clear();
}

CAnimationSets::CAnimationSets()
{

}

CAnimationSets* CAnimationSets::GetInstance()
{
	if (_instance == NULL) _instance = new CAnimationSets();
	return _instance;
}

LPANIMATION_SET CAnimationSets::Get(ObjectType id)
{
	LPANIMATION_SET ani_set = animation_sets[id]; 
	if (ani_set == NULL)
		ani_set = animation_setsScene[id];
	if (ani_set == NULL)
		DebugOut(L"[ERROR] Failed to find animation set id: %d\n",id);
	 
	return ani_set;
}

#define MAX_LENGTH_LINE 500
#define MAX_LINE 1000
void CAnimationSets::LoadResource(string Folderpath)
{
	#pragma region ani
	ifstream f(Folderpath+"/animation.txt");
	char str[MAX_LENGTH_LINE];
	while (f.getline(str, MAX_LINE))
	{
		string line(str);
		vector<string> tokens = split(line);

		if (tokens.size() < 3) return;
		int ani_id = atoi(tokens[0].c_str());
		int frameTime = atoi(tokens[1].c_str());
		LPANIMATION ani = new CAnimation(frameTime);
		int id_sprite;
		for (int i = 2; i < tokens.size(); i++)
		{
			if (tokens[i].c_str() == "")
				continue;
			id_sprite = atoi(tokens[i].c_str());
			ani->Add(id_sprite);
		}
		CAnimations::GetInstance()->Add(ani_id, ani,0);
	}
	f.close();

#pragma endregion

	#pragma region aniSet
	ifstream ifs(Folderpath + "/animationset.txt");
	while (ifs.getline(str, MAX_LINE))
	{
		string line(str);
		vector<string> tokens = split(line);
		if (tokens.size() < 2) return;
		int ani_set_id = atoi(tokens[0].c_str());
		LPANIMATION_SET s = new CAnimationSet();
		int ani_id;
		for (int i = 1; i < tokens.size(); i++)
		{
			ani_id = atoi(tokens[i].c_str());
			s->insert(make_pair(ani_id, CAnimations::GetInstance()->Get(ani_id)));
		}
		CAnimationSets::GetInstance()->Add(ani_set_id, s,0);
	}
	ifs.close();
#pragma endregion


}

void CAnimationSets::LoadResourceScene(string Folderpath)
{

#pragma region ani
	ifstream f(Folderpath + "/animation.txt");
		char str[MAX_LENGTH_LINE];
	if (f.fail())
	{
		DebugOut(L"[Texture] File animation.txt scene not found\n");
	}
	else
	{
		while (f.getline(str, MAX_LINE))
		{
			string line(str);
			vector<string> tokens = split(line);

			if (tokens.size() < 3) return;
			int ani_id = atoi(tokens[0].c_str());
			int frameTime = atoi(tokens[1].c_str());
			LPANIMATION ani = new CAnimation(frameTime);
			int id_sprite;
			for (int i = 2; i < tokens.size(); i++)
			{
				if (tokens[i].c_str() == "")
					continue;
				id_sprite = atoi(tokens[i].c_str());
				ani->Add(id_sprite);
			}
			CAnimations::GetInstance()->Add(ani_id, ani, 1);
		}
	}
	f.close();

#pragma endregion

#pragma region aniSet
	ifstream ifs(Folderpath + "/animationset.txt");
	if (f.fail())
	{
		DebugOut(L"[Texture] File animationset.txt scene not found\n");
	}
	else
	{
		while (ifs.getline(str, MAX_LINE))
		{
			string line(str);
			vector<string> tokens = split(line);
			if (tokens.size() < 2) return;
			int ani_set_id = atoi(tokens[0].c_str());
			LPANIMATION_SET s = new CAnimationSet();
			int ani_id;
			for (int i = 1; i < tokens.size(); i++)
			{
				ani_id = atoi(tokens[i].c_str());
				s->insert(make_pair(ani_id, CAnimations::GetInstance()->Get(ani_id)));
			}
			CAnimationSets::GetInstance()->Add(ani_set_id, s, 1);
		}
	}
	ifs.close();
#pragma endregion
}

void CAnimationSets::Add(int id, LPANIMATION_SET ani_set,bool isScene)
{
	if(isScene)
		animation_setsScene[id] = ani_set;
	else
	animation_sets[id] = ani_set;
}

void CAnimationSets::Clear()
{
	ClearAniScene();
	for (auto x : animation_sets)
	{
		LPANIMATION_SET ani = x.second;
		delete ani;
	}
	animation_sets.clear();
	DebugOut(L"[Animation] Clear all animation success\n");
}

void CAnimationSets::ClearAniScene()
{
	for (auto x : animation_setsScene)
	{
		LPANIMATION_SET ani = x.second;
		delete ani;
	}
	animation_setsScene.clear();
	DebugOut(L"[Animation] Clear animation scene success\n");
}