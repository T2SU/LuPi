
#include "Global.h"

#include "FieldManager.h"
#include "Player.h"

#include "Tools.h"
#include "LPLogger.h"
#include "LPApp.h"
#include "LPDataManager.h"
Field::CField  *Instance::Field    = 0;
CPlayer        *Instance::Player   = 0;
CPlayerUI      *Instance::PlayerUI = 0;
FMOD::System   *LPApp::FMOD__System;
FMOD::Channel  *LPApp::FMOD__BGMusicChannel;
bool		   LPApp::g_bIsActivated = false;
bool           bLostDevice = false;
int		       nFadeAlpha = -1;
char           cFadeDirection = -1;
bool		   bPostTransferField = false;

BOOL LPApp::Initialize( HWND fhWnd )
{
	BOOL bSucceeded = SUCCEEDED ( D3D9::InitDevice( fhWnd ) );
	if (bSucceeded)
	{
		FMOD::System_Create(&LPApp::FMOD__System);
		LPApp::FMOD__System->init(32, FMOD_INIT_NORMAL, 0);

		LPDataManager::LoadData();
		FieldManager::GetField(0, &Instance::Field);
		Instance::Player = new CPlayer;           // 테스트 플레이어
		Instance::Player->InitUI();

		atexit(LPApp::UnInitialize);
	}



	return bSucceeded;
}

void LPApp::UnInitialize()
{
	LPLogger::WriteConsoleW(L"프로그램 종료. 모든 데이터 할당 해제중.\n");
	delete Instance::Player;
	delete Instance::PlayerUI;
	delete Instance::Field;
	LPLogger::WriteConsoleW(L"FMOD 시스템 할당 해제중.\n");
	LPApp::FMOD__System->release();
	LPApp::FMOD__System->close();
	LPLogger::WriteConsoleW(L"할당 해제 완료. 프로그램 종료.\n");
}


LRESULT WINAPI LPApp::MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	switch ( msg )
	{
		case WM_DESTROY:
			PostQuitMessage( 0 );
			return 0;
		case WM_ACTIVATEAPP:
			g_bIsActivated = wParam == TRUE;
			return 0;
	}
	if (Instance::Player)
	{
		LRESULT r = Instance::Player->MsgProc(hWnd, msg, wParam, lParam);
		return r;
	}

	return DefWindowProc( hWnd, msg, wParam, lParam );
}



VOID LPApp::EventLoop()
{
	MSG msg;
	ZeroMemory(&msg, sizeof(msg));
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else if (bLostDevice)
			LPApp::RecoverDevice();
		else
			LPApp::Render();

		if (bPostTransferField)
		{
			FieldManager::PostTransferField();
			// 4. PostTransferField 과정이 끝나면 페이드 아웃 이펙트 시작.
			bPostTransferField = false;
			cFadeDirection = 1;
		}
			
	}
}



VOID LPApp::Render()
{
	if ( Instance::Field )
		Instance::Field->Begin();

	if ( SUCCEEDED( D3D9::D3DDevice->BeginScene() ) )
	{

		if ( Instance::Field )
			Instance::Field->Render();

		if ( Instance::Player )
			Instance::Player->RenderUI();

		DrawFadeEffect();

		D3D9::D3DDevice->EndScene();
	}
	HRESULT hResult = D3D9::D3DDevice->Present(NULL, NULL, NULL, NULL);
	if (D3DERR_DEVICELOST == hResult)
		bLostDevice = true;
}

VOID LPApp::RecoverDevice()
{
	HRESULT hr = D3D9::D3DDevice->TestCooperativeLevel();
	switch (hr)
	{
		case D3D_OK:
			// device is no longer lost and reset has completed successfully; recover resources here
			bLostDevice = false;
			return;  // note: this assumes that this test is the last thing in the function it's being called from
		case D3DERR_DEVICELOST:
			Sleep(5);
			// device is still lost; can't do anything so do nothing
			break;
		case D3DERR_DEVICENOTRESET:
			// device is ready to be reset; Release resources here and then Reset it
			// (be sure to test the hr from your Reset call too!)
			//ReleaseResources();
			D3D9::ResetDevice(Global::HandleWND);
			break;
		default:
			MessageBox(NULL, L"TestCooperativeLevel() failed", L"Error", MB_OK);
			PostQuitMessage(0);
			return;
	}
}

VOID LPApp::DrawFadeEffect()
{
	if (cFadeDirection == 0)
	{
		// 2. Fade 이펙트 시작
		nFadeAlpha += 15;
		if (nFadeAlpha >= 255)
		{
			nFadeAlpha = 255;
			cFadeDirection = -1;
			// 3. 화면이 검어지면 TransferField 시전.
			bPostTransferField = true;
		}
	}
	else if (cFadeDirection == 1)
	{
		// 5. Fade Out 이펙트 시작
		nFadeAlpha -= 15;
		if (nFadeAlpha <= 0)
		{
			// 이펙트가 모두 끝나면 화면을 되돌림.
			nFadeAlpha = -1;
			cFadeDirection = -1;
		}
	}
	if (nFadeAlpha >= 0)
	{
		// - 페이드 이펙트가 진행중이면 화면에 페이드 이펙트 드로잉.
		if (SUCCEEDED(D3D9::D3DXSprite->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE)))
		{
			// Draw2D  테스트중
			D3DXMATRIX  mat; // 3d 행렬

			D3DXVECTOR2 pos(0, 0);//화면 좌표
			D3DXVECTOR2 scale(1, 1);//스케일
			RECT		rect = { 0, 0, 1024, 768 }; // 그릴 행렬 사이즈

			D3DXMatrixTransformation2D(&mat, NULL, 0, &scale, NULL, 0, &pos);


			D3D9::D3DXSprite->SetTransform(&mat);
			D3D9::D3DXSprite->Draw(
				LPDataManager::g_pBlackTexture, // pTexture
				&rect,
				NULL,
				NULL,
				D3DCOLOR_ARGB(nFadeAlpha, 255, 255, 255) // dwColor
			);

			D3D9::D3DXSprite->End();
		}
	}
}

VOID LPApp::StartFadeEffect()
{
	nFadeAlpha = 0;
	cFadeDirection = 0;
	// 1. Fade 이펙트 등록 -> 시작.
}