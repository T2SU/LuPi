
#include "Field.h"
#include "StreamReader.h"
#include "LPDataManager.h"
#include "LPLogger.h"
#include "FieldTexture.h"
#include "Global.h"
#include "LPApp.h"
#include <fmod.hpp>
#include <fmod_errors.h>
#include <LPData.h>
#include <d3dx9.h>
#include <vector>
#include <Windows.h>




Field::CField::CField()
{
	// 필드 데이터 로드
}
Field::CField::~CField()
{
	// 필드 데이터 소멸
	std::for_each(__LAYER1_aTexture.begin(), __LAYER1_aTexture.end(), [](CFieldTexture* p){ delete p; });
	std::for_each(__LAYER2_aTexture.begin(), __LAYER2_aTexture.end(), [](CFieldTexture* p){ delete p; });
	std::for_each(__LAYER3_aTexture.begin(), __LAYER3_aTexture.end(), [](CFieldTexture* p){ delete p; });
	std::for_each(__LAYER4_aTexture.begin(), __LAYER4_aTexture.end(), [](CFieldTexture* p){ delete p; });
	std::for_each(__LAYER5_aTexture.begin(), __LAYER5_aTexture.end(), [](CFieldTexture* p){ delete p; });
	std::for_each(__LAYER6_aTexture.begin(), __LAYER6_aTexture.end(), [](CFieldTexture* p){ delete p; });
	std::for_each(__LAYER7_aTexture.begin(), __LAYER7_aTexture.end(), [](CFieldTexture* p){ delete p; });
	__LAYER1_aTexture.clear();
	__LAYER2_aTexture.clear();
	__LAYER3_aTexture.clear();
	__LAYER4_aTexture.clear();
	__LAYER5_aTexture.clear();
	__LAYER6_aTexture.clear();
	__LAYER7_aTexture.clear();

	// 소멸 후 텍스쳐 모두 삭제
	LPDataManager::Texture::Clear();

	// 소멸 후 사운드 모두 삭제
	if (LPApp::FMOD__BGMusicChannel)
	{
		bool playing = 0;
		LPApp::FMOD__BGMusicChannel->isPlaying(&playing);
		if (playing)
		{
			LPApp::FMOD__BGMusicChannel->stop();
		}
	}

	LPDataManager::Sound::Clear();
	BackgroundMusic = 0;
}

void Field::CField::LoadField(StreamReader* reader, LPFieldInfo* lpFieldInfo)
{
	LPLogger::WriteConsoleW(L"[필드%d] 로드 시작\n", GetFieldId());

	if (strlen(lpFieldInfo->aMusic) > 0)
	{
		wchar_t strUnicode[260] = { 0, };
		char	strMultibyte[260] = { 0, };
		strcpy_s(strMultibyte, 260, lpFieldInfo->aMusic);
		int nLen = MultiByteToWideChar(CP_ACP, 0, strMultibyte, strlen(strMultibyte), NULL, NULL);
		MultiByteToWideChar(CP_ACP, 0, strMultibyte, strlen(strMultibyte), strUnicode, nLen);

		FMOD_RESULT result;

		BackgroundMusic = LPDataManager::Sound::GetSoundSet(strUnicode)->Get(lpFieldInfo->dwMusicIndex);

		FMOD_MODE fmodMode;
		BackgroundMusic->sound->getMode(&fmodMode);
		LPLogger::WriteConsoleA("GetMode Result : %08X\n", fmodMode);
		result = BackgroundMusic->sound->setMode(FMOD_LOOP_NORMAL);
		LPLogger::WriteConsoleA("SetMode Result : %s(%d)\n", FMOD_ErrorString(result), result);
		result = BackgroundMusic->sound->setLoopCount(-1);
		LPLogger::WriteConsoleA("SetLoopCount Result : %s(%d)\n", FMOD_ErrorString(result), result);

		LPApp::FMOD__System->playSound(BackgroundMusic->sound, 0, false, &LPApp::FMOD__BGMusicChannel);
	}

	DWORD dwLayersCount = reader->Read<DWORD>();

	for (DWORD i = 0; i < dwLayersCount; i++)
	{
		DWORD dwLayer = reader->Read<DWORD>();
		DWORD dwTexturesCount = reader->Read<DWORD>();
		for (size_t t = 0; t < dwTexturesCount; t++)
		{
			LPTexture lpTexture;
			void* buf = reader->ReadBinary(sizeof(lpTexture));
			memcpy_s(&lpTexture, sizeof(lpTexture), buf, sizeof(lpTexture));

			wchar_t strUnicode[256] = { 0, };
			char	strMultibyte[256] = { 0, };
			strcpy_s(strMultibyte, 256, lpTexture.aBuf);
			int nLen = MultiByteToWideChar(CP_ACP, 0, strMultibyte, strlen(strMultibyte), NULL, NULL);
			MultiByteToWideChar(CP_ACP, 0, strMultibyte, strlen(strMultibyte), strUnicode, nLen);

			CFieldTexture* pTexture = new CFieldTexture(
				strUnicode,
				lpTexture.dwTextureIndex,
				(FLOAT)lpTexture.dwOffsetX,
				(FLOAT)lpTexture.dwOffsetY,
				lpTexture.fScaleX,
				lpTexture.fScaleY,
				(FLOAT)lpTexture.dwRotation,
				lpTexture.uAlpha,
				lpTexture.uRed,
				lpTexture.uGreen,
				lpTexture.uBlue
			);
			AddTexture(dwLayer, pTexture);
		}
		LPLogger::WriteConsoleW(L"[레이어%d] 텍스쳐 %d개\n", dwLayer, dwTexturesCount);
	}




}

void Field::CField::AddTexture(DWORD Layer, CFieldTexture* Texture)
{
	switch (Layer)
	{
		case 1:
			__LAYER1_aTexture.push_back(Texture);
			break;
		case 2:
			__LAYER2_aTexture.push_back(Texture);
			break;
		case 3:
			__LAYER3_aTexture.push_back(Texture);
			break;
		case 4:
			__LAYER4_aTexture.push_back(Texture);
			break;
		case 5:
			__LAYER5_aTexture.push_back(Texture);
			break;
		case 6:
			__LAYER6_aTexture.push_back(Texture);
			break;
		case 7:
			__LAYER7_aTexture.push_back(Texture);
			break;
	}
}

void Field::CField::Begin()
{
	DWORD dwBackColor = D3DCOLOR_XRGB(0, 0, 0);
	D3D9::D3DDevice->Clear(0, NULL, D3DCLEAR_TARGET, dwBackColor, 1.0f, 0);
}