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
	// ���ε� �ڵ� ��� �ݱ�
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

	StreamReader reader(map_ptr);        // ��Ʈ�� ����
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
		//uncompress((Bytef*)uncompressedBuf, &UnCompSize, (const Bytef*)buf, dwCompressedSize); // ���� ����
		LPLogger::WriteConsoleW(L"[%d] ���� ũ�� : %d  ���� ũ�� : %d  ���� ũ�� : %d\n", i, dwOriginalSize, dwCompressedSize, UnCompSize);

		CTexture* texture = new CTexture(uncompressedBuf, dwOriginalSize, dwTransColorKey); // �ؽ��� ���� (�׽�Ʈ)
		aData[i] = texture; // �����̳ʿ� �ؽ��� ���� (�׽�Ʈ)

		free(uncompressedBuf);
		*/

		DWORD dwOriginalSize = reader.Read<DWORD>();
		DWORD dwTransColorKey = reader.Read<DWORD>();
		void* buf = reader.ReadBinary(dwOriginalSize);

		LPLogger::WriteConsoleW(L"[%d] ���� ũ�� : %d  \n", i, dwOriginalSize);

		CTexture* texture = new CTexture(buf, dwOriginalSize, dwTransColorKey); // �ؽ��� ���� (�׽�Ʈ)
		aData[i] = texture; // �����̳ʿ� �ؽ��� ���� (�׽�Ʈ)


	}

	LPLogger::WriteConsoleW(L"%s �ؽ��ļ�Ʈ���� %d�� �ؽ��� �ε�\n", sKey.c_str(), dwFilesCount);
	

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