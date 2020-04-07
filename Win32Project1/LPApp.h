#pragma once

#include <Windows.h>
#include <fmod.hpp>
#include "Field.h"
#include "PlayerUI.h"
#include "Player.h"

namespace LPApp
{
	LRESULT WINAPI MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	BOOL Initialize( HWND fhWnd );
	void UnInitialize();
	VOID EventLoop();
	VOID Render();
	VOID RecoverDevice();
	VOID DrawFadeEffect();
	VOID StartFadeEffect();

	extern bool g_bIsActivated;
	extern FMOD::System *FMOD__System;
	extern FMOD::Channel *FMOD__BGMusicChannel;
}



namespace Instance
{
	extern CPlayer* Player;
	extern Field::CField*  Field;
	extern CPlayerUI* PlayerUI;
}

