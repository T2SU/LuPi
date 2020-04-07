#pragma once
#include "UIMessageProcessor.h"
#include "FieldTexture.h"
#include "LPLogger.h"
#include <Windows.h>
class CUIButton : public CUIMessageProcessor
{
protected:
	CFieldTexture* pTexture;
	CFieldTexture* pNormal;
	CFieldTexture* pOver;
	CFieldTexture* pPressed;
	bool bMouseOver;
	bool bPressed;
	DWORD dwLButtonDownTime;
	DWORD dwMButtonDownTime;
	DWORD dwRButtonDownTime;
	wchar_t *sCaption;
public:

	CUIButton(DWORD dwButtonIndex, FLOAT x, FLOAT y, DWORD dwAlpha, wchar_t *caption)
	{
		pNormal = new CFieldTexture(L"UIButton", dwButtonIndex, x, y, 1.0f, 1.0f, 0.0f, dwAlpha, 255, 255, 255);
		pOver = new CFieldTexture(L"UIButton", dwButtonIndex + 1, x, y, 1.0f, 1.0f, 0.0f, dwAlpha, 255, 255, 255);
		pPressed = new CFieldTexture(L"UIButton", dwButtonIndex + 2, x, y, 1.0f, 1.0f, 0.0f, dwAlpha, 255, 255, 255);
		pTexture = pNormal;
		bMouseOver = false;
		bPressed = false;
		sCaption = caption;
	}
	~CUIButton() {
		if (pNormal)
			delete pNormal;
		if (pOver)
			delete pOver;
		if (pPressed)
			delete pPressed;
	}
	LRESULT WINAPI MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	virtual void OnMouseOver() { pTexture = pOver; };
	virtual void OnMouseNoOver() { pTexture = pNormal; };
	virtual void OnMousePressed(DWORD dwButton) { pTexture = pPressed; };
	virtual void OnMouseNoPressed(DWORD dwButton) { pTexture = pNormal; };
	virtual void OnMouseClicked(DWORD dwButton) {};
	virtual void Draw();
};

