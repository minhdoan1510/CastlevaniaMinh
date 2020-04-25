#include"TextureManager.h"
#include<string>
#include"Game.h"

#include<fstream>

CTextureManager * CTextureManager::__instance = NULL;

CTextureManager::CTextureManager()
{
}

void CTextureManager::LoadResource(int scene)
{
	Add(ObjectType::TEX_ALL, "Resources/tex/texture.png");
	Add(ObjectType::SIMON_DEADTH, "Resources/simondeath.png");

	////TextureSimon
	//Add(ObjectType::SIMON, new CTexture("Resources/simon.png", 8, 3, 24));
	////Add(ObjectType::SIMON_TRANS, new CTexture("Resources/simon.png", 8, 3, 24));
	//Add(ObjectType::TEX_BRICK_TYPE_1, new CTexture("Resources/ground/2.png"));

	////MapTextture
	int _h, _w , _r, _c;
	string path;
	for (int i = 0; i < scene; i++)
	{
		//string path = "Resources/map/" + to_string(i+1) + "/map.txt";
		/*ifstream f(path, ios::in);
		f >> _h >> _w >> _r >> _c;
		f.close();*/
		path = "Resources/map/" + to_string(i+1) + "/map.png";
		Add(static_cast<ObjectType>(MAP1+i), path.c_str());
	}

	////WeaponTextture
	//Add(ObjectType::WHIP, new CTexture("Resources/weapon/whip.png", 4, 3, 12));
	//Add(ObjectType::AXE, new CTexture("Resources/weapon/axe.png", 4, 1, 4));
	//Add(ObjectType::BOOMERANG, new CTexture("Resources/weapon/boomerang.png", 3, 1, 3));

	////MapObject
	//Add(ObjectType::TEX_BRICK_TYPE_TRANSPARENT, new CTexture("Resources/ground/bricktransparent.png"));
	//Add(ObjectType::TEX_CANDLE_TYPE_1, new CTexture("Resources/ground/candletype1.png",2,1,2));
	//Add(ObjectType::TEX_CANDLE_TYPE_2, new CTexture("Resources/ground/candletype2.png",2,1,2));


	////DEATH_EFFECT
	//Add(ObjectType::TEX_DEATH_OBJECT, new CTexture("Resources/Effects/death_object.png",4,1,4));
	//Add(ObjectType::TEX_MONEY, new CTexture("Resources/item/money.png", 1, 5, 5));

	////Item_Object
	//Add(ObjectType:: TEX_ITEM, new CTexture("Resources/item/Item.png",13,2,26));

	////FONT_Board
	//Add(ObjectType::FONT_BOARD, new CTexture("Resources/board/alphabet.png", 13, 3, 39));
	//Add(ObjectType::FRAME_ITEM_BOARD, new CTexture("Resources/board/ItemFrame.png"));
	//Add(ObjectType::HEART_ITEM_BOARD, new CTexture("Resources/board/heart.png"));
	//Add(ObjectType::HEAL_ITEM_BOARD, new CTexture("Resources/board/hp.png",3,1,3));
}

//void CTextureManager::Add(ObjectType textureType, char* filePath, int column, int row, int totalframes, int R, int G, int B)
//{
//	Add(textureType, new CTexture(filePath, column, row, totalframes, R, G, B));
//}

void CTextureManager::Add(ObjectType textureType, const char* filePath, int R, int G, int B)
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

	textures[textureType] = Texture;
}

//void CTextureManager::Add(ObjectType textureType, CTexture* texture)
//{
//	textures[textureType] = texture;
//}

CTextureManager *CTextureManager::GetInstance()
{
	if (__instance == NULL) __instance = new CTextureManager();
	return __instance;
}

LPDIRECT3DTEXTURE9 CTextureManager::Get(ObjectType textureType)
{
	return textures[textureType];
}

//void CTextureManager::GetInfoTex(ObjectType id , int& w, int& h)
//{
//	D3DSURFACE_DESC desc;
//	textures[id]->GetLevelDesc(0, &desc);
//	desc.Width;
//}

void CTextureManager::Clear()
{
	for (auto x : textures)
	{
		LPDIRECT3DTEXTURE9 tex = x.second;
		if (tex!=NULL) tex->Release();
	}
	
	textures.clear();
}

