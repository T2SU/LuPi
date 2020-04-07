#pragma once
#include <Windows.h>
class CPlayerUI
{
public:
	virtual void DrawUI() = 0;
	virtual void InitUI() = 0;
	virtual LRESULT WINAPI MsgProcUI(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) = 0;
};

