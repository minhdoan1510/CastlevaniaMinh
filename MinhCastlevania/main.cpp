﻿#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include "Game.h"
#include "SceneManager.h"
#include "WinScene.h"
#include "resource.h"
#include "Sound.h"
#include "PlayScence.h"


LRESULT CALLBACK WinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) {
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_SETFOCUS:
		try {
			if (dynamic_cast<CPlayScene*> (CSceneManager::GetInstance()->GetCurrentScene()))
				static_cast<CPlayScene*> (CSceneManager::GetInstance()->GetCurrentScene())->ResumeMusic();
		}
		catch(exception e) {}
		//CSound::GetInstance()->SetVolume(100.0f);
		DebugOut(L"in  ");
		break;
	case WM_KILLFOCUS:
		CSound::GetInstance()->stop();
		//CSound::GetInstance()->SetVolume(50.0f);
		DebugOut(L"out  ");
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

void Update(DWORD dt)
{
	CSceneManager::GetInstance()->Update(dt);
}

void Render()
{
	LPDIRECT3DDEVICE9 d3ddv = CGame::GetInstance()->GetDirect3DDevice();
	LPDIRECT3DSURFACE9 bb = CGame::GetInstance()->GetBackBuffer();
	LPD3DXSPRITE spriteHandler = CGame::GetInstance()->GetSpriteHandler();

	if (d3ddv->BeginScene())
	{
		if(dynamic_cast<CWinScene*>(CSceneManager::GetInstance()->GetCurrentScene()))
			d3ddv->ColorFill(bb, NULL, BACKGROUND_WIN_COLOR);
		else
			d3ddv->ColorFill(bb, NULL, BACKGROUND_COLOR);

		spriteHandler->Begin(D3DXSPRITE_ALPHABLEND);

		CSceneManager::GetInstance()->Render();

		spriteHandler->End();
		d3ddv->EndScene();
	}


	d3ddv->Present(NULL, NULL, NULL, NULL);
}

HWND CreateGameWindow(HINSTANCE hInstance, int nCmdShow, int ScreenWidth, int ScreenHeight)
{
	WNDCLASSEX wc;
	wc.cbSize = sizeof(WNDCLASSEX);

	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.hInstance = hInstance;

	wc.lpfnWndProc = (WNDPROC)WinProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = WINDOW_CLASS_NAME;
	wc.hIconSm = NULL;

	RegisterClassEx(&wc);

	HWND hWnd =
		CreateWindow(
			WINDOW_CLASS_NAME,
			MAIN_WINDOW_TITLE,
			WS_OVERLAPPEDWINDOW, 
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			ScreenWidth,
			ScreenHeight,
			NULL,
			NULL,
			hInstance,
			NULL);

	if (!hWnd) 
	{
		OutputDebugString(L"[main.cpp] CreateWindow failed");
		DWORD ErrCode = GetLastError();
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return hWnd;
}

int Run()
{
	MSG msg;
	int done = 0;
	DWORD frameStart = GetTickCount();
	DWORD tickPerFrame = 1000 / MAX_FRAME_RATE;
	DWORD timeLost=0;
	while (!done)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{	
			if (msg.message == WM_QUIT) done = 1;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

			DWORD now = GetTickCount();
			DWORD dt = now - frameStart ;// -(now - timeLostFocus);//loại bỏ thời gian lostfocus
		
			//timeLostFocus = now;
			if (dt >= tickPerFrame)
			{
				frameStart = now;

				CGame::GetInstance()->ProcessKeyboard();

				Update(dt);
				Render();
			}
			else
				Sleep(tickPerFrame - dt);
	}

	return 1;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	HWND hWnd = CreateGameWindow(hInstance, nCmdShow, SCREEN_WIDTH, SCREEN_HEIGHT);

	CGame::GetInstance()->Init(hWnd);
	CGame::GetInstance()->Load(L"Resources/settings.txt");
	CGame::GetInstance()->InitKeyboard();

	SetWindowPos(hWnd, 0, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER);
	Run();

	return 0;
}