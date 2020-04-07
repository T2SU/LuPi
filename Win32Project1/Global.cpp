
#include <tchar.h>
#include "Global.h"
#include "LPDataManager.h"

namespace Global
{
	HWND HandleWND;
}
namespace D3D9
{
	LPDIRECT3D9					D3D;
	LPDIRECT3DDEVICE9			D3DDevice;
	LPD3DXSPRITE				D3DXSprite;
}


void D3D9::MakeD3DParamater( HWND hWnd, D3DPRESENT_PARAMETERS *d3dpp )
{
	if (GetFileAttributes(L"HellGuy.txt") == INVALID_FILE_ATTRIBUTES)
	{
		RECT rWindow;
		GetClientRect(hWnd, &rWindow);
		d3dpp->Windowed = TRUE;
		d3dpp->BackBufferFormat = D3DFMT_UNKNOWN;
		d3dpp->BackBufferWidth = rWindow.right - rWindow.left;
		d3dpp->BackBufferHeight = rWindow.bottom - rWindow.top;
		d3dpp->PresentationInterval = D3DPRESENT_INTERVAL_ONE; //D3DPRESENT_INTERVAL_IMMEDIATE;
	}
	else
	{
		d3dpp->Windowed = FALSE;
		d3dpp->BackBufferFormat = D3DFMT_A8R8G8B8;
		d3dpp->BackBufferWidth = 1024;
		d3dpp->BackBufferHeight = 768;
		d3dpp->PresentationInterval = D3DPRESENT_INTERVAL_ONE;
	}

	d3dpp->SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp->hDeviceWindow = hWnd;
}

void D3D9::ResetDevice( HWND hWnd )
{
	
	if (D3D9::D3DDevice)
	{
		D3DPRESENT_PARAMETERS d3dpp;
		ZeroMemory(&d3dpp, sizeof(d3dpp));

		D3D9::D3DXSprite->OnLostDevice();
		LPDataManager::LostDevice();
		D3D9::MakeD3DParamater(hWnd, &d3dpp);
		D3D9::D3DDevice->Reset(&d3dpp);
		LPDataManager::ResetDevice();
		D3D9::D3DXSprite->OnResetDevice();

	}
}

HRESULT D3D9::InitDevice( HWND fhWnd )
{
	Global::HandleWND = fhWnd;

	D3D9::D3D = Direct3DCreate9( D3D_SDK_VERSION );
	D3DDISPLAYMODE d3ddm;

	if ( NULL == D3D9::D3D )
	{
		MessageBox( NULL, _T( "Direct3D 생성 실패!" ), _T( "오류 발생!" ), MB_OK | MB_ICONERROR );
		return E_FAIL;
	}



	if ( FAILED ( D3D9::D3D->GetAdapterDisplayMode ( D3DADAPTER_DEFAULT, &d3ddm ) ) )
	{
		MessageBox( NULL, _T( "어댑터 디스플레이 모드 Get 실패" ), _T( "오류 발생!" ), MB_OK | MB_ICONERROR );
		return E_FAIL;
	}

	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	D3D9::MakeD3DParamater(fhWnd, &d3dpp);


	if ( FAILED ( D3D9::D3D->CreateDevice ( 
		D3DADAPTER_DEFAULT, 
		D3DDEVTYPE_HAL, 
		fhWnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		&d3dpp, 
		&D3DDevice 
		) ) )
	{
		MessageBox( NULL, _T( "장치 생성 실패!" ), _T( "오류 발생!" ), MB_OK | MB_ICONERROR );
		return E_FAIL;
	}



	if ( FAILED ( D3DXCreateSprite ( D3DDevice, &D3DXSprite ) ) )
	{
		MessageBox( NULL, _T( "스프라이트 생성 실패!" ), _T( "오류 발생!" ), MB_OK | MB_ICONERROR );
		return E_FAIL;
	}

	D3DDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );
	D3DDevice->SetRenderState( D3DRS_LIGHTING, FALSE );

	return S_OK;
}

Direction::Direction Direction::GetDirection(LONG deltaX, LONG deltaY)
{
	if (deltaY > 0 && deltaX > 0)
		return (Direction::SouthEast); // ↘
	else if (deltaY > 0 && deltaX < 0)
		return (Direction::SouthWest); // ↙
	else if (deltaY < 0 && deltaX < 0)
		return (Direction::NorthWest); // ↖
	else if (deltaY < 0 && deltaX > 0)
		return (Direction::NorthEast); // ↗
	else if (deltaY < 0 && deltaX == 0)
		return (Direction::North); // ↑
	else if (deltaY == 0 && deltaX < 0)
		return (Direction::West); // ←
	else if (deltaY > 0 && deltaX == 0)
		return (Direction::South); // ↓
	else if (deltaY == 0 && deltaX > 0)
		return (Direction::East); // →
	assert(!L"Unknown Direction!!");
	return Direction::East;
}

bool IsPressed(SHORT keyState)
{
	return (keyState & 0x8000) == 0x8000;
}