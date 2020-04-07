#pragma once
#include "PlayerUI.h"
#include "FieldManager.h"
#include "UIButton.h"

class CMainMenuUI : public CPlayerUI {
public:
	CMainMenuUI();
	~CMainMenuUI();
	void DrawUI();
	void InitUI();
	LRESULT WINAPI MsgProcUI(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
private:
	CUIButton* button1 = 0;
	CUIButton* button2 = 0;


};