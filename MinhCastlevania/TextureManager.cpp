#include"TextureManager.h"
#include<string>
#include"Game.h"

#include<fstream>

CTextureManager * CTextureManager::__instance = NULL;

CTextureManager::CTextureManager()
{
}

#define MAX_LENGTH_LINE 100
#define MAX_LINE 1000

void CTextureManager::LoadResource(string fileTexture)
{
	
	char str[MAX_LENGTH_LINE];
	ifstream ifs(fileTexture);

	if (ifs.fail())
	{
		DebugOut(L"[Texture] File texture global not found\n");
		return;
	}
	while (ifs.getline(str, MAX_LINE))
	{
		string line(str);
		vector<string> tokens = split(line);
		if (tokens.size() < 2) return;
		Add(static_cast<ObjectType>(atoi(tokens[0].c_str())), tokens[1].c_str(),0);
	}
	ifs.close();

	DebugOut(L"[Texture] Load success resource texture global\n");
}

void CTextureManager::LoadResourceScene(string fileTexture)
{
	char str[MAX_LENGTH_LINE];
	ifstream ifs(fileTexture);

	if (ifs.fail())
	{
		DebugOut(L"[Texture] File texture scene not found\n");
		return;
	}

	while (ifs.getline(str, MAX_LINE))
	{
		string line(str);
		vector<string> tokens = split(line);
		if (tokens.size() < 2) return;
		Add(static_cast<ObjectType>(atoi(tokens[0].c_str())), tokens[1].c_str(),1);
	}
	ifs.close();
	DebugOut(L"[Texture] Load success resource texture scene\n");
}

void CTextureManager::Add(ObjectType textureType, const char* filePath, bool isScene, int R, int G, int B)
{
	LPDIRECT3DTEXTURE9 Texture;

	D3DXIMAGE_INFO info;
	HRESULT result = D3DXGetImageInfoFromFileA(filePath, &info);
	if (result != D3D_OK)
	{
		DebugOut(L"[Texture.cpp] GetImageInfoFromFile failed: %s\n", filePath);
		return;
	}

	LPDIRECT3DDEVICE9 d3ddv = CGame::GetInstance()->GetDirect3DDevice();

	result = D3DXCreateTextureFromFileExA(
		d3ddv,
		filePath,
		info.Width,
		info.Height,
		1,
		D3DUSAGE_DYNAMIC,
		D3DFMT_UNKNOWN,
		D3DPOOL_DEFAULT,
		D3DX_DEFAULT,
		D3DX_DEFAULT,
		D3DCOLOR_XRGB(R, G, B),
		&info,
		NULL,
		&Texture);
	if (result != D3D_OK)
	{
		OutputDebugString(L"[ERROR] CreateTextureFromFile failed\n");
		return;
	}

	if (isScene)
		texturesScene[textureType] = Texture;
	else
		textures[textureType] = Texture;

	DebugOut(L"[Texture] Add success texture ID: %d\n", textureType);
}

CTextureManager *CTextureManager::GetInstance()
{
	if (__instance == NULL) __instance = new CTextureManager();
	return __instance;
}

LPDIRECT3DTEXTURE9 CTextureManager::Get(ObjectType textureType)
{
	LPDIRECT3DTEXTURE9 tex = textures[textureType];
	if(tex==NULL)
		tex = texturesScene[textureType];
	if (tex == NULL)
		DebugOut(L"[TextureManager] Not found texture ID: %d\n", textureType);
	return tex;
}

void CTextureManager::Clear()
{
	ClearTextureScene();
	for (auto x : textures)
	{
		LPDIRECT3DTEXTURE9 tex = x.second;
		if (tex!=NULL) tex->Release();
	}
	textures.clear();
	DebugOut(L"[Texture] Clear all texture success\n");
}

void CTextureManager::ClearTextureScene()
{
	for (auto x : texturesScene)
	{
		LPDIRECT3DTEXTURE9 tex = x.second;
		if (tex != NULL) tex->Release();
	}
	texturesScene.clear();
	DebugOut(L"[Texture] Clear texture scene success\n");
}
