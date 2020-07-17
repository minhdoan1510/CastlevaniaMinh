#pragma once
#include "dsound.h"
#include "windows.h"
#include <map>
#include <string>
#include <iostream>
#include <mmsystem.h>
#pragma comment(lib, "dsound.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "winmm.lib")
#include <dsound.h>
#include "GameDefine.h"


class CSound
{
private:
	CSound(HWND hWnd);
	static CSound* Instance;
	IDirectSoundBuffer* primaryBuffer;
	std::map<std::string, IDirectSoundBuffer8*> soundBufferMap;
	IDirectSound8* pDevice;
	bool isMute;

	std::map<std::string, IDirectSoundBuffer8*> soundTemp; // sound lưu thông tin để resume

public:
	struct WaveHeaderStruct
	{
		char chunkId[4];
		unsigned long chunkSize;
		char format[4];
		char subChunkId[4];
		unsigned long subChunkSize;
		unsigned short audioFormat;
		unsigned short numChannels;
		unsigned long sampleRate;
		unsigned long bytesPerSecond;
		unsigned short blockAlign;
		unsigned short bitsPerSample;
		char dataChunkId[4];
		unsigned long dataSize;
	};

	static CSound* GetInstance();
	float volume;
	void static create(HWND hWnd);
	void play(std::string name, bool infiniteLoop, int times = 100000);
	void stop(std::string name = "");
	void loadSound(std::string fileName, std::string name);
	void UnLoadSound(string name);
	void LoadSoundResource();
	~CSound();
};


