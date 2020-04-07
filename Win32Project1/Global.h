#pragma once

#include <assert.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <Windows.h>

namespace Global
{
	extern HWND HandleWND;

}

namespace Event
{
	enum Timer
	{
		ControlKeyInput,
	};
}


bool IsPressed(SHORT keyState);


namespace Direction
{

	enum Direction
	{
		East,
		EastNorthEast,
		NorthEast,
		NorthNorthEast,
		North,
		NorthNorthWest,
		NorthWest,
		WestNorthWest,
		West,
		WestSouthWest,
		SouthWest,
		SouthSouthWest,
		South,
		SouthSouthEast,
		SouthEast,
		EastSouthEast
	};

	Direction GetDirection(LONG deltaX, LONG deltaY);
	
}

namespace D3D9
{
	HRESULT InitDevice(HWND fhWnd);
	void ResetDevice(HWND hWnd);
	extern LPDIRECT3D9					D3D;
	extern LPDIRECT3DDEVICE9			D3DDevice;
	extern LPD3DXSPRITE					D3DXSprite;
	void MakeD3DParamater( HWND hWnd, D3DPRESENT_PARAMETERS *in);
}