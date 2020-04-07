
#include "MainMenuUI.h"
#include "UIButton.h"
#include <Windows.h>



CMainMenuUI::CMainMenuUI()
{
}

CMainMenuUI::~CMainMenuUI()
{
	delete button1;
	delete button2;
}

void CMainMenuUI::DrawUI()
{
	button1->Draw();
	button2->Draw();
}

void CMainMenuUI::InitUI()
{
	if (button1)
		delete button1;
	if (button2)
		delete button2;


	class Button1 : public CUIButton {
	public:
		Button1() : CUIButton(0, 750, 550, 255, L"게임 시작") {};
		void OnMouseClicked(DWORD dwButton) {
			FieldManager::RegisterTransferField(1);
		}
	};
	class Button2 : public CUIButton {
	public:
		Button2() : CUIButton(0, 750, 630, 255, L"종료") {};
		void OnMouseClicked(DWORD dwButton) {
			PostQuitMessage(0);
		}
	};

	button1 = new Button1;
	button2 = new Button2;
}

LRESULT WINAPI CMainMenuUI::MsgProcUI(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	button1->MsgProc(hWnd, msg, wParam, lParam);
	button2->MsgProc(hWnd, msg, wParam, lParam);
	return DefWindowProc(hWnd, msg, wParam, lParam);
}