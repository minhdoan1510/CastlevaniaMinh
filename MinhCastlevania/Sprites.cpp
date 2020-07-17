#include "Sprites.h"
#include "Game.h"
#include "Utils.h"
#include "Camera.h"
#include <fstream>

CSprites* CSprites::__instance = NULL;

CSprite::CSprite(int id, int left, int top, int right, int bottom, LPDIRECT3DTEXTURE9 tex)
{
	this->id = id;
	this->left = left;
	this->top = top;
	this->right = right;
	this->bottom = bottom;
	this->texture = tex;
}

CSprite::CSprite(int id, LPDIRECT3DTEXTURE9 tex)
{
	this->id = id;
	left = 0;
	top = 0;
	GetInfo(tex, right, bottom);
	this->texture = tex;
}

CSprites *CSprites::GetInstance()
{
	if (__instance == NULL) __instance = new CSprites();
	return __instance;
}

void CSprite::GetInfo(LPDIRECT3DTEXTURE9 tex, int& w, int& h)
{
	D3DSURFACE_DESC desc;
	tex->GetLevelDesc(0, &desc);
	w = desc.Width;
	h = desc.Height;
}

void CSprite::Draw(float _x, float _y, int alpha, int R, int G ,int B)
{
	/*RECT checkCam;
	checkCam.left = _x;
	checkCam.top = _y;
	checkCam.right = _x + right - left;
	checkCam.bottom = _y + bottom - top;

	if (!CCamera::GetInstance()->IsContainCam(checkCam))
		return;*/

	CGame * game = CGame::GetInstance();
	RECT r;
	r.left = left;
	r.top = top;
	r.right = right;
	r.bottom = bottom;

	D3DXVECTOR2 pos = CCamera::GetInstance()->Transform(_x, _y+ PULL_SCREEN_Y);
	game->GetSpriteHandler()->Draw(texture, &r, NULL, &D3DXVECTOR3(pos.x, pos.y, 0), D3DCOLOR_ARGB(alpha, R, G, B));
}

void CSprite::DrawFlipX(float _x, float _y, int alpha, int R, int G, int B)
{
	/*RECT checkCam;
	checkCam.left = _x;
	checkCam.top = _y;
	checkCam.right = _x + right - left;
	checkCam.bottom = _y + bottom - top;

	if (!CCamera::GetInstance()->IsContainCam(checkCam))
		return;*/

	CGame* game = CGame::GetInstance();
	RECT r;
	r.left = left;
	r.top = top;
	r.right = right;
	r.bottom = bottom;

	D3DXVECTOR2 pos = CCamera::GetInstance()->Transform(_x, _y+ PULL_SCREEN_Y);

	D3DXMATRIX AA, BB;
	game->GetSpriteHandler()->GetTransform(&AA);
	D3DXMatrixTransformation2D(&BB, &D3DXVECTOR2(pos.x, pos.x), 0.0f, &D3DXVECTOR2(-1.0f, 1.0f), NULL, 0.0f, NULL);
	D3DXMATRIX CC = AA * BB;
	game->GetSpriteHandler()->SetTransform(&CC);
	pos.x -= right-left;
	D3DXVECTOR3 p(trunc(pos.x), trunc(pos.y), 0);
	game->GetSpriteHandler()->Draw(texture, &r, NULL, &D3DXVECTOR3(p.x, p.y, 0), D3DCOLOR_ARGB(alpha, R, G, B));
	game->GetSpriteHandler()->SetTransform(&AA);
}

void CSprite::DrawFrame(int _x, int _y, RECT rect, int alpha, int R, int G, int B)
{
	/*RECT checkCam;
	checkCam.left = _x;
	checkCam.top = _y;
	checkCam.right = _x + rect.right - rect.left;
	checkCam.bottom = _y + rect.bottom - rect.top;*/

	/*if (!CCamera::GetInstance()->IsContainCam(checkCam))
		return;*/

	CGame* game = CGame::GetInstance();
	D3DXVECTOR2 pos = CCamera::GetInstance()->Transform(_x, _y + PULL_SCREEN_Y);
	game->GetSpriteHandler()->Draw(texture, &rect, NULL, &D3DXVECTOR3(pos.x, pos.y, 0), D3DCOLOR_ARGB(alpha, R, G, B));
}

RECT CSprite::GetRECT()
{
	RECT rect;
	rect.left = left;
	rect.right = right;
	rect.bottom = bottom;
	rect.top = top;
	return  rect;
}

void CSprites::Add(int id, int left, int top, int right, int bottom, LPDIRECT3DTEXTURE9 tex, bool isScene)
{
	if (isScene)
	{
		LPSPRITE s = new CSprite(id, left, top, right, bottom, tex);
		spritesScene[id] = s;
		DebugOut(L"[INFO] sprite added: %d \n", id);
	}
	else {
		LPSPRITE s = new CSprite(id, left, top, right, bottom, tex);
		sprites[id] = s;
		DebugOut(L"[INFO] sprite added: %d \n", id);
	}
}

void CSprites::Add(int id, CSprite* cs, bool isScene)
{

	if (isScene)
	{
		spritesScene[id] = cs;
	}
	else {
		sprites[id] = cs;
	}
}

void CSprites::Add(int id, int idTexture, bool isScene)
{
	LPDIRECT3DTEXTURE9 tex = CTextureManager::GetInstance()->Get(static_cast<ObjectType>(idTexture));
	if (isScene)
	{
		spritesScene[id] = new CSprite(id, tex);
	}
	else {
		sprites[id] = new CSprite(id, tex);
	}
}

LPSPRITE CSprites::Get(int id)
{
	CSprite* c = sprites[id];
	if (c == NULL)
	{
		c = spritesScene[id];
	}
	return c;
}

void CSprites::Clear()
{
	ClearSpritesScene();
	for (auto x : sprites)
	{
		LPSPRITE s = x.second;
		delete s;
	}
	this->sprites.clear();
	DebugOut(L"[Texture] Clear all sprite success\n");
}

void CSprites::ClearSpritesScene()
{
	for (auto x : spritesScene)
	{
		LPSPRITE s = x.second;
		delete s;
	}
	this->spritesScene.clear();
	DebugOut(L"[Texture] Clear sprite scene success\n");
}

#define MAX_LENGTH_LINE 100
#define MAX_LINE 1000

void CSprites::LoadResource(string filesprite)
{
	LPDIRECT3DTEXTURE9 tex;
	ifstream f;
	//f.open(ToLPCWSTR("Resources/txt/sprite.txt"));
	f.open(filesprite);
	char str[MAX_LENGTH_LINE];
	while (f.getline(str, MAX_LINE))
	{
		string line(str);
		vector<string> tokens = split(line);
		if (tokens.size() == 6)
		{

			int ID = atoi(tokens[0].c_str());
			int l = atoi(tokens[1].c_str());
			int t = atoi(tokens[2].c_str());
			int r = atoi(tokens[3].c_str());
			int b = atoi(tokens[4].c_str());
			int texID = atoi(tokens[5].c_str());


			LPDIRECT3DTEXTURE9 tex = CTextureManager::GetInstance()->Get(static_cast<ObjectType>(texID));
			if (tex == NULL)
			{
				DebugOut(L"[ERROR] Texture ID %d not found!\n", texID);
				continue;
			}
			CSprites::GetInstance()->Add(ID, l, t, r, b, tex, 0);
		}
		else if (tokens.size() == 2)
		{
			int ID = atoi(tokens[0].c_str());
			int texID = atoi(tokens[1].c_str());
			CSprites::GetInstance()->Add(ID, texID, 0);
		}
	}

	DebugOut(L"[Texture] Load success resource sprite global\n");
}

void CSprites::LoadResourceScene(string filesprite)
{
	LPDIRECT3DTEXTURE9 tex;
	ifstream f;
	//f.open(ToLPCWSTR("Resources/txt/sprite.txt"));
	f.open(filesprite);
	char str[MAX_LENGTH_LINE];
	while (f.getline(str, MAX_LINE))
	{
		string line(str);
		vector<string> tokens = split(line);
		if (tokens.size() == 6)
		{
			int ID = atoi(tokens[0].c_str());
			int l = atoi(tokens[1].c_str());
			int t = atoi(tokens[2].c_str());
			int r = atoi(tokens[3].c_str());
			int b = atoi(tokens[4].c_str());
			int texID = atoi(tokens[5].c_str());

			LPDIRECT3DTEXTURE9 tex = CTextureManager::GetInstance()->Get(static_cast<ObjectType>(texID));
			if (tex == NULL)
			{
				DebugOut(L"[ERROR] Texture ID %d not found!\n", texID);
				continue;
			}
			CSprites::GetInstance()->Add(ID, l, t, r, b, tex, 1);
		}
		else if (tokens.size() == 2)
		{
			int ID = atoi(tokens[0].c_str());
			int texID = atoi(tokens[1].c_str());
			CSprites::GetInstance()->Add(ID, texID, 1);
		}
	}
	DebugOut(L"[Texture] Load success resource sprite scene\n");
}