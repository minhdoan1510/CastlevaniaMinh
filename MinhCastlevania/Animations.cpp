#include "Animations.h"
#include "Utils.h"
#include "TextureManager.h"
#include<fstream>

CAnimationSets * CAnimationSets::_instance = NULL;

CAnimation* CAnimation::Clone()
{
	return new CAnimation(*this);
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
	frames.push_back(frame);
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
		DWORD t = frames[currentFrame]->GetTime();
		if (now - lastFrameTime > t)
		{
			lifeFrameTime = now - lastFrameTime;
			currentFrame++;
			lastFrameTime = now;

			if (currentFrame == frames.size() - 1)
			{
				DoneFrameFinal = 1;
			}
			else
				DoneFrameFinal = 0;

			if (currentFrame == frames.size())
			{
				currentFrame = 0;
			}
			
		}
	}
	if(flipX)
		frames[currentFrame]->GetSprite()->DrawFlipX(_x, _y, alpha, R, G, B);
	else
	frames[currentFrame]->GetSprite()->Draw(_x, _y, alpha, R, G, B);
}

void CAnimation::RenderFrame(int idFrame, float _x, float _y, bool flipX, int alpha, int R, int G, int B)
{
	if (flipX)
		frames[idFrame]->GetSprite()->DrawFlipX(_x, _y, alpha, R, G, B);
	else
		frames[idFrame]->GetSprite()->Draw(_x, _y, alpha, R, G, B);
}

void CAnimation::KeepFramePresent()
{
	if (currentFrame == -1)
		return;
	currentFrame--;
	if (currentFrame < 0)
		currentFrame = frames.size() - 1;
}

int CAnimation::GetCurrentFrame()
{

	return currentFrame;
}

CAnimations * CAnimations::_instance = NULL;

CAnimations * CAnimations::GetInstance()
{
	if (_instance == NULL) _instance = new CAnimations();
	return _instance;
}

void CAnimations::Add(int id, LPANIMATION ani)
{
	animations[id] = ani;
}

LPANIMATION CAnimations::Get(int id)
{
	LPANIMATION ani = animations[id];
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
		DebugOut(L"[ERROR] Failed to find animation set id: %d\n",id);
	 
	return ani_set;
}

void CAnimationSets::LoadResource()
{
	#pragma region ani


	ifstream f("Resources/txt/animation.txt");
	char str[100];
	while (f.getline(str, 1000))
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
		CAnimations::GetInstance()->Add(ani_id, ani);
	}
	f.close();

#pragma endregion

	#pragma region aniSet
	ifstream ifs("Resources/txt/animationset.txt");
	while (ifs.getline(str,1000))
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
		CAnimationSets::GetInstance()->Add(ani_set_id, s);
	}
	ifs.close();
#pragma endregion
}

void CAnimationSets::Add(int id, LPANIMATION_SET ani_set)
{
	animation_sets[id] = ani_set;
}