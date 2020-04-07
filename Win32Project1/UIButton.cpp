#include "UIButton.h"
#include "Tools.h"
#include "Global.h"


void CUIButton::Draw()
{
	if (pTexture)
		pTexture->Draw(D3D9::D3DXSprite);
	if (sCaption) {
		RECT rc;
		pTexture->TextureArea(&rc);
		DWORD dwFlag = DT_VCENTER | DT_NOCLIP;
		LPDataManager::Font::NanumGothic->DrawTextW(NULL, sCaption, -1, &rc, DT_CENTER | dwFlag, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	}

}

LRESULT WINAPI CUIButton::MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (pTexture) {
		RECT rect;
		pTexture->TextureArea(&rect);
		switch (msg) {

			case WM_MOUSEMOVE: {
				SHORT mouseX = (SHORT)LOWORD(lParam);
				SHORT mouseY = (SHORT)HIWORD(lParam);
				if (Tools::InRect(&rect, mouseX, mouseY)) {
					if (!bMouseOver) {
						bMouseOver = true;
						OnMouseOver();
					}
				}
				else {
					if (bMouseOver) {
						bMouseOver = false;
						OnMouseNoOver();
					}
					if (dwLButtonDownTime) {
						dwLButtonDownTime = 0;
						OnMouseNoPressed(MK_LBUTTON);
					}
				}
				break;
			}
			case WM_LBUTTONDOWN: {
				SHORT mouseX = (SHORT)LOWORD(lParam);
				SHORT mouseY = (SHORT)HIWORD(lParam);
				if (Tools::InRect(&rect, mouseX, mouseY)) {
					dwLButtonDownTime = GetTickCount();
					OnMousePressed(MK_LBUTTON);
				}
				break;
			}
			case WM_LBUTTONUP: {
				SHORT mouseX = (SHORT)LOWORD(lParam);
				SHORT mouseY = (SHORT)HIWORD(lParam);
				if (Tools::InRect(&rect, mouseX, mouseY)) {
					if (dwLButtonDownTime) {
						if (GetTickCount() - dwLButtonDownTime < 500) { // Click Event
							OnMouseClicked(MK_LBUTTON);
						}
					}
					dwLButtonDownTime = 0;
					OnMouseNoPressed(MK_LBUTTON);
					if (bMouseOver)
						OnMouseOver();
				}
				break;
			}

		}
	}
	
	return DefWindowProc(hWnd, msg, wParam, lParam);
}