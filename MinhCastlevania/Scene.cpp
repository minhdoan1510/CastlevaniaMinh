#include "Scene.h"

CScene::CScene(int _id, string _folderPath)
{
	id = _id;
	folderPath = _folderPath;
}

string CScene::GetFolderPath()
{
	return folderPath;
}

int CScene::GetID()
{
	return id;
}
