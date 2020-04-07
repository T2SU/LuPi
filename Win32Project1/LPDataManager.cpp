#include "LPDataManager.h"
#include "LPLogger.h"
#include "Global.h"
#include <algorithm>
#include <Windows.h>

void	   LoadFonts();
LPD3DXFONT LPDataManager::Font::NanumBarunpen;
LPD3DXFONT LPDataManager::Font::NanumGothic;
LPD3DXFONT LPDataManager::Font::Dodum9pt; 
boost::unordered_map<std::wstring, CTextureSet*> Manager;
boost::unordered_map<std::wstring, CSoundSet*> SoundManager;

LPDIRECT3DTEXTURE9 LPDataManager::g_pBlackTexture = 0;


void LPDataManager::LoadData()
{
	LoadFonts();
	CreateDummyTexture();
}

// 폰트 로딩
void LoadFonts()
{

	LPLogger::WriteConsoleW(L"폰트 로딩중");
	int count = 0;
	
	count += AddFontResourceExW(L"Data/Fonts/NanumBarunpen.ttf", FR_PRIVATE, 0);
	count += AddFontResourceExW(L"Data/Fonts/NanumGothic.ttf", FR_PRIVATE, 0);
	LPLogger::WriteConsoleW(L"%d개의 폰트를 로딩하였음!!", count);

	D3DXCreateFontW(D3D9::D3DDevice,            // D3D device
		16,									    // Height
		0,									    // Width
		FW_NORMAL,                              // Weight
		0,                                      // MipLevels, 0 = autogen mipmaps
		FALSE,									// Italic
		DEFAULT_CHARSET,						// CharSet
		OUT_DEFAULT_PRECIS,						// OutputPrecision
		CLEARTYPE_QUALITY,						// Quality
		DEFAULT_PITCH | FF_DONTCARE,			// PitchAndFamily
		L"나눔바른펜",							// pFaceName
		&LPDataManager::Font::NanumBarunpen);	// ppFont
	
	D3DXCreateFontW(D3D9::D3DDevice,            // D3D device
		20,									    // Height
		0,									    // Width
		FW_NORMAL,                              // Weight
		0,                                      // MipLevels, 0 = autogen mipmaps
		FALSE,									// Italic
		DEFAULT_CHARSET,						// CharSet
		OUT_DEFAULT_PRECIS,						// OutputPrecision
		CLEARTYPE_QUALITY,						// Quality
		DEFAULT_PITCH | FF_DONTCARE,			// PitchAndFamily
		L"나눔고딕",							// pFaceName
		&LPDataManager::Font::NanumGothic);		// ppFont

	D3DXCreateFontW(D3D9::D3DDevice,            // D3D device
		11,									    // Height
		0,									    // Width
		FW_NORMAL,                              // Weight
		0,                                      // MipLevels, 0 = autogen mipmaps
		FALSE,									// Italic
		DEFAULT_CHARSET,						// CharSet
		OUT_DEFAULT_PRECIS,						// OutputPrecision
		DEFAULT_QUALITY,						// Quality
		DEFAULT_PITCH | FF_DONTCARE,			// PitchAndFamily
		L"돋움",								// pFaceName
		&LPDataManager::Font::Dodum9pt);		// ppFont

}

void LPDataManager::CreateDummyTexture()
{
	D3DXCreateTexture(
		D3D9::D3DDevice,
		1024,
		768,
		0,
		0,
		D3DFMT_A8B8G8R8,
		D3DPOOL_MANAGED,
		&LPDataManager::g_pBlackTexture);
	D3DLOCKED_RECT area;

	LPDataManager::g_pBlackTexture->LockRect(0, &area, 0, D3DLOCK_DISCARD);
	DWORD *bytePointer = (DWORD*)area.pBits;

	for (DWORD i = 0; i<768; i++)
	{
		for (DWORD j = 0; j<1024; j++)
		{
			DWORD index = i * area.Pitch / 4 + j;
			bytePointer[index] = D3DCOLOR_XRGB(0, 0, 0); //make it black
		}
	}
	LPDataManager::g_pBlackTexture->UnlockRect(0);
}


CSoundSet* LPDataManager::Sound::GetSoundSet(std::wstring key)
{
	if (NULL == SoundManager[key])
	{
		SoundManager[key] = new CSoundSet(key);
	}
	return SoundManager[key];
}


void LPDataManager::Sound::Clear()
{
	std::for_each(SoundManager.begin(), SoundManager.end(), [](std::pair<std::wstring, CSoundSet*> p){ delete p.second; });
	SoundManager.clear();
}

CTextureSet* LPDataManager::Texture::GetTextureSet(std::wstring key)
{
	if (NULL == Manager[key])
	{
		Manager[key] = new CTextureSet(key);
	}
	return Manager[key];
}


void LPDataManager::Texture::Clear()
{
	std::for_each(Manager.begin(), Manager.end(), [](std::pair<std::wstring, CTextureSet*> p){ delete p.second; });
	Manager.clear();
}


void LPDataManager::LostDevice()
{
	LPDataManager::Font::Dodum9pt->OnLostDevice();
	LPDataManager::Font::NanumBarunpen->OnLostDevice();
	LPDataManager::Font::NanumGothic->OnLostDevice();
}
void LPDataManager::ResetDevice()
{
	LPDataManager::Font::Dodum9pt->OnResetDevice();
	LPDataManager::Font::NanumBarunpen->OnResetDevice();
	LPDataManager::Font::NanumGothic->OnResetDevice();
}