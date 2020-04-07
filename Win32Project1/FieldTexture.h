#pragma once
#include "Texture.h"
#include "LPDataManager.h"
#include <string>
#include <d3dx9.h>
class CFieldTexture
{
private:
	CTexture* pTexture;
	FLOAT fOffsetX;
	FLOAT fOffsetY;
	FLOAT fScaleX;
	FLOAT fScaleY;
	FLOAT fRotation;
	DWORD dwColor;
public:
	CFieldTexture(std::wstring tSet, DWORD dwIndex, FLOAT fOffsetX, FLOAT fOffsetY, FLOAT fScaleX, FLOAT fScaleY, FLOAT fRotation, DWORD dwAlpha, DWORD dwRed, DWORD dwGreen, DWORD dwBlue)
	{
		pTexture = LPDataManager::Texture::GetTextureSet(tSet)->Get(dwIndex);
		this->fOffsetX = fOffsetX;
		this->fOffsetY = fOffsetY;
		this->fScaleX = fScaleX;
		this->fScaleY = fScaleY;
		this->fRotation = fRotation;
		this->dwColor = D3DCOLOR_ARGB(dwAlpha, dwRed, dwGreen, dwBlue);
	}
	void Draw(LPD3DXSPRITE pSprite);
	void TextureArea(RECT* rect);
	void Color(DWORD dwAlpha, DWORD dwRed, DWORD dwGreen, DWORD dwBlue)
	{
		this->dwColor = D3DCOLOR_ARGB(dwAlpha, dwRed, dwGreen, dwBlue);
	}
	DWORD GetAlpha() { return (dwColor >> 24) & 0xFF; };
	~CFieldTexture();
};

