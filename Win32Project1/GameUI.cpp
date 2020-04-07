#include "GameUI.h"
#include "UIButton.h"
#include <Windows.h>





CGameUI::CGameUI()
{
}

CGameUI::~CGameUI()
{
	delete button1;
}

void CGameUI::DrawUI()
{
	button1->Draw();
}

void CGameUI::InitUI()
{
	if (button1)
		delete button1;

	class Button1 : public CUIButton {
	public:
		Button1() : CUIButton(0, 800, 70, 255, L"메인 메뉴로") {};
		void OnMouseClicked(DWORD dwButton) {
			FieldManager::RegisterTransferField(0);
		}
	};

	button1 = new Button1;
}

LRESULT WINAPI CGameUI::MsgProcUI(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	button1->MsgProc(hWnd, msg, wParam, lParam);
	return DefWindowProc(hWnd, msg, wParam, lParam);
}