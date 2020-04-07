#pragma once
#include <Windows.h>
class CUIMessageProcessor
{
public:
	virtual LRESULT WINAPI MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) { 
		return DefWindowProc(hWnd, msg, wParam, lParam); 
	};
};

