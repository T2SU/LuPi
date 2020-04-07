#pragma once
#include "PlayerUI.h"
#include "FieldManager.h"
#include "UIButton.h"

class CGameUI : public CPlayerUI {
public:
	CGameUI();
	~CGameUI();
	void DrawUI();
	void InitUI();
	LRESULT WINAPI MsgProcUI(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
private:
	CUIButton* button1 = 0;
};
