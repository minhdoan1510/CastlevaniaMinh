#pragma once
#include<unordered_map>
#include"GameDefine.h"

using namespace std;

class CTextureManager
{
	static CTextureManager * __instance;
	unordered_map<ObjectType, LPDIRECT3DTEXTURE9> textures;
	unordered_map<ObjectType, LPDIRECT3DTEXTURE9> texturesScene;

public: 
	void Add(ObjectType textureType, const char* filePath, bool isScene, int R = 83, int G = 83, int B = 83);
	static CTextureManager* GetInstance();
	CTextureManager();

	void LoadResource(string fileTexture);
	void LoadResourceScene(string fileTexture);

	//void Add(ObjectType textureType, char* filePath, int column, int row, int totalframes, int R, int G, int B);
	//void Add(ObjectType textureType, const char* filePath, int R = 83, int G = 83, int B = 83);
	//void Add(ObjectType textureType, CTexture* texture);
	LPDIRECT3DTEXTURE9 Get(ObjectType textureType);
	//void GetInfoTex(ObjectType textureType,int& w, int& h);
	void Clear();
	void ClearTextureScene();
};