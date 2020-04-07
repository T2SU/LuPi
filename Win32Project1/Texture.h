#pragma once
#include <d3dx9.h>

class CTexture
{
public:
	LPDIRECT3DTEXTURE9 pTexture;
	D3DXIMAGE_INFO info;
	CTexture(WCHAR *sFilePath);
	CTexture(void* buf, UINT len, DWORD dwColorKey);
	~CTexture();
};

