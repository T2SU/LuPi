#include "StreamReader.h"
#include "LPLogger.h"
#include <algorithm>
#include <memory>
#include <zlib.h>
#include "LPDataManager.h"
#include "TextureSet.h"
#include "ZLibTool.h"


CTextureSet::CTextureSet(std::wstring key)
{
	sKey = key;
	LoadTextureSet();
}


CTextureSet::~CTextureSet()
{
	// 매핑된 핸들 모두 닫기
	UnmapViewOfFile( map_ptr );
	CloseHandle( hMapHandle );
	CloseHandle(hFile);

	std::for_each(aData.begin(), aData.end(), [](std::pair<int, CTexture*> p){ delete p.second; });
}

CTexture* CTextureSet::Get(int key)
{
	return aData[key];
}


void CTextureSet::LoadTextureSet()
{
	std::wstring sDataPath = L"Data/TextureSet/" + sKey + L".lpd";

	hFile	   = CreateFileW(sDataPath.c_str(), GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_FLAG_RANDOM_ACCESS, NULL);
	hMapHandle = CreateFileMappingW(hFile, NULL, PAGE_READONLY | SEC_RESERVE, 0, 0, 0);
	map_ptr    = MapViewOfFile(hMapHandle, FILE_MAP_READ, 0, 0, 0);

	StreamReader reader(map_ptr);        // 스트림 리더
	DWORD dwFilesCount = reader.Read<DWORD>();

	for (DWORD i = 0; i < dwFilesCount; i++)
	{
		/*
		DWORD dwOriginalSize = reader.Read<DWORD>();
		DWORD dwCompressedSize = reader.Read<DWORD>();
		DWORD dwTransColorKey = reader.Read<DWORD>();
		void* buf = reader.ReadBinary(dwCompressedSize);
		void* uncompressedBuf = malloc(dwOriginalSize);

		uLongf UnCompSize;
		UnCompSize = UncompressData((LPBYTE)buf, dwCompressedSize, (LPBYTE)uncompressedBuf, dwOriginalSize);
		//uncompress((Bytef*)uncompressedBuf, &UnCompSize, (const Bytef*)buf, dwCompressedSize); // 압축 해제
		LPLogger::WriteConsoleW(L"[%d] 원본 크기 : %d  압축 크기 : %d  해제 크기 : %d\n", i, dwOriginalSize, dwCompressedSize, UnCompSize);

		CTexture* texture = new CTexture(uncompressedBuf, dwOriginalSize, dwTransColorKey); // 텍스쳐 생성 (테스트)
		aData[i] = texture; // 컨테이너에 텍스쳐 삽입 (테스트)

		free(uncompressedBuf);
		*/

		DWORD dwOriginalSize = reader.Read<DWORD>();
		DWORD dwTransColorKey = reader.Read<DWORD>();
		void* buf = reader.ReadBinary(dwOriginalSize);

		LPLogger::WriteConsoleW(L"[%d] 원본 크기 : %d  \n", i, dwOriginalSize);

		CTexture* texture = new CTexture(buf, dwOriginalSize, dwTransColorKey); // 텍스쳐 생성 (테스트)
		aData[i] = texture; // 컨테이너에 텍스쳐 삽입 (테스트)


	}

	LPLogger::WriteConsoleW(L"%s 텍스쳐세트에서 %d개 텍스쳐 로드\n", sKey.c_str(), dwFilesCount);
	

	// DWORD filesCount
	// ULONG_PTR[] fileOffset
	// struct Texture {
	//     FLOAT x
	//     FLOAT y
	//     FLOAT scaleX
	//     FLOAT scaleY
	//     FLOAT rotation
	//     DWORD dwColor
	//     DWORD dwTransparentColor
	//     DWORD dwImagelen
	//     char[] image
	// }
}