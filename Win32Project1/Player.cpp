
#include <d3dx9.h>

#include "LPApp.h"
#include "Player.h"
#include "Global.h"

#include "MainMenuUI.h"
#include "GameUI.h"
#include "FieldManager.h"
#include "UIButton.h"
#include "LPDataManager.h"


int leftRight = -1;


CPlayer::CPlayer()
{
	SetX(0);
	SetY(0);
	SetAnimationIndex(0);
	SetDirection(12);
	pPlayerTexture = 0;
	dwAnimationDelay = 40;
	dwAnimationLastUpdated = GetTickCount();
}

CPlayer::~CPlayer()
{
	if (Instance::Field->GetFieldType() == 1)
	{
		KillTimer(Global::HandleWND, Event::Timer::ControlKeyInput);
	}
}

void CPlayer::InitUI()
{
	if (Instance::Field->GetFieldType() == 0)
	{
		LPLogger::WriteConsoleW(L"MainMenu UI Init중..\n");
		Instance::PlayerUI = new CMainMenuUI();
		Instance::PlayerUI->InitUI();
	}
	else if(Instance::Field->GetFieldType() == 1)
	{
		LPLogger::WriteConsoleW(L"Game UI Init중..\n");
		Instance::PlayerUI = new CGameUI();
		Instance::PlayerUI->InitUI();

		LPLogger::WriteConsoleW(L"Player Texture 생성중..\n");
		SetTimer(Global::HandleWND, Event::Timer::ControlKeyInput, 10, 0);

	}


	if (Instance::Field->GetFieldType() != 1)
	{
		KillTimer(Global::HandleWND, Event::Timer::ControlKeyInput);
	}
}

void CPlayer::RenderUI()
{
	Instance::PlayerUI->DrawUI();
}

void CPlayer::DrawObject()
{

	pPlayerTexture = LPDataManager::Texture::GetTextureSet(L"Heroine_Walk")->Get(GetDirection());

	if (SUCCEEDED(D3D9::D3DXSprite->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE)))
	{
		DWORD dwTWidth = pPlayerTexture->info.Width;
		DWORD dwPWidth = dwTWidth / 15;
		DWORD dwTHeight = pPlayerTexture->info.Height;

		// Draw2D  테스트중
		D3DXMATRIX  mat; // 3d 행렬

		D3DXVECTOR2 pos((FLOAT)GetX(), (FLOAT)GetY());//화면 좌표
		D3DXVECTOR2 scale(1, 1);//스케일
		RECT		rect = { (LONG)dwPWidth * GetAnimationIndex(), 0, (LONG)dwPWidth * GetAnimationIndex() + (LONG)dwPWidth, (LONG)dwTHeight }; // 그릴 행렬 사이즈

		D3DXVECTOR2* rotateCenter = NULL;

		D3DXMatrixTransformation2D(&mat, NULL, 0, &scale, rotateCenter, 0, &pos);

		D3D9::D3DXSprite->SetTransform(&mat);
		D3D9::D3DXSprite->Draw(
			pPlayerTexture->pTexture, // pTexture
			&rect,
			NULL,
			NULL,
			D3DXCOLOR(255, 255, 255, 255) // dwColor
			);

		D3D9::D3DXSprite->End();

		

	}
	// 
}
 
LRESULT WINAPI CPlayer::MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (msg == WM_TIMER)
	{
		OnCheckKeyState();
		return 0;
	}
	return Instance::PlayerUI->MsgProcUI(hWnd, msg, wParam, lParam);
}

void CPlayer::OnCheckKeyState()
{
	if (!LPApp::g_bIsActivated)
		return;

	LONG deltaX = 0, deltaY = 0;

	SHORT down = GetAsyncKeyState('S');
	SHORT up = GetAsyncKeyState('W');
	SHORT left = GetAsyncKeyState('A');
	SHORT right = GetAsyncKeyState('D');

	// 키 입력 얻기
	if (IsPressed(down))      deltaY += 2;
	if (IsPressed(up))        deltaY -= 2;
	if (IsPressed(left))      deltaX -= 2;
	if (IsPressed(right))     deltaX += 2;


	if (IsPressed(left) && IsPressed(right))
	{
		if (leftRight == 0)
			deltaX += 2;
		else if (leftRight == 1)
			deltaX -= 2;
	}
	else
	{
		if (IsPressed(left))		   leftRight = 0;
		else if (IsPressed(right))     leftRight = 1;
	}



	if (deltaX != 0 || deltaY != 0)
	{
		// 방향 갱신
		SetDirection(Direction::GetDirection(deltaX, deltaY));

		// 좌표 갱신
		SetX(GetX() + deltaX);
		SetY(GetY() + deltaY);

		// 애니메이션 인덱스 갱신
		DWORD dwCurrentTick = GetTickCount();
		if (dwAnimationDelay + dwAnimationLastUpdated < dwCurrentTick)
		{
			LONG lAindex = GetAnimationIndex();
			lAindex++;
			if (lAindex >= 15)
				lAindex = 0;
			SetAnimationIndex(lAindex);
			dwAnimationLastUpdated = dwCurrentTick;
		}
	}
}