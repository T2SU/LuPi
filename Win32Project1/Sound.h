#pragma once
#include "Mp3Player.h"
#include <fmod.hpp>
#include <Windows.h>
class CSound
{
private:
	void* buf;
public:
	FMOD::Sound *sound;
	CSound(WCHAR *sFilePath);
	CSound(void* buf, UINT len);
	~CSound();
};

