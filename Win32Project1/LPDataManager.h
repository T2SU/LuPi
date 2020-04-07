#pragma once

#include "TextureSet.h"
#include "SoundSet.h"
#include <d3dx9.h>
#include <boost/unordered_map.hpp>


namespace LPDataManager
{
	extern LPDIRECT3DTEXTURE9 g_pBlackTexture;
	void LoadData();
	void LostDevice();
	void ResetDevice();
	void CreateDummyTexture();
	namespace Font
	{
		extern LPD3DXFONT NanumBarunpen;
		extern LPD3DXFONT NanumGothic;
		extern LPD3DXFONT Dodum9pt;
	}
	namespace Texture
	{
		CTextureSet* GetTextureSet(std::wstring key);
		void Clear();
	}
	namespace Sound
	{
		CSoundSet* GetSoundSet(std::wstring key);
		void Clear();
	}

}