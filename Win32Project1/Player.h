#pragma once
#include <Windows.h>
#include "FieldTexture.h"
#include "Tools.h"
#include "PlayerUI.h"
#include "FieldObject.h"


class CPlayer : public Field::CFieldObject
{
private:
	CTexture* pPlayerTexture;
public:
	CPlayer();
	~CPlayer();
	void RenderUI();
	void InitUI();
	void DrawObject();
	void OnCheckKeyState();
	LRESULT WINAPI MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

};

