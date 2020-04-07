#include "StreamReader.h"
#include "LPLogger.h"
#include <algorithm>
#include <memory>
#include <zlib.h>
#include "LPDataManager.h"
#include "SoundSet.h"
#include "ZLibTool.h"


CSoundSet::CSoundSet(std::wstring key)
{
	sKey = key;
	LoadSoundSet();
}


CSoundSet::~CSoundSet()
{
	// ���ε� �ڵ� ��� �ݱ�
	UnmapViewOfFile(map_ptr);
	CloseHandle(hMapHandle);
	CloseHandle(hFile);

	std::for_each(aData.begin(), aData.end(), [](std::pair<int, CSound*> p){ delete p.second; });
}

CSound* CSoundSet::Get(int key)
{
	return aData[key];
}


void CSoundSet::LoadSoundSet()
{
	std::wstring sDataPath = L"Data/Sound/" + sKey + L".lpd";

	hFile = CreateFileW(sDataPath.c_str(), GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_FLAG_RANDOM_ACCESS, NULL);
	hMapHandle = CreateFileMappingW(hFile, NULL, PAGE_READONLY | SEC_RESERVE, 0, 0, 0);
	map_ptr = MapViewOfFile(hMapHandle, FILE_MAP_READ, 0, 0, 0);

	StreamReader reader(map_ptr);        // ��Ʈ�� ����
	DWORD dwFilesCount = reader.Read<DWORD>();

	for (size_t i = 0; i < dwFilesCount; i++)
	{
		/*
		DWORD dwOriginalSize = reader.Read<DWORD>();
		DWORD dwCompressedSize = reader.Read<DWORD>();
		void* buf = reader.ReadBinary(dwCompressedSize);
		void* uncompressedBuf = malloc(dwOriginalSize);

		uLongf UnCompSize;
		UnCompSize = UncompressData((LPBYTE)buf, dwCompressedSize, (LPBYTE)uncompressedBuf, dwOriginalSize);
		//uncompress((Bytef*)uncompressedBuf, &UnCompSize, (const Bytef*)buf, dwCompressedSize); // ���� ����
		LPLogger::WriteConsoleW(L"[%d] ���� ũ�� : %d  ���� ũ�� : %d  ���� ũ�� : %d\n", i, dwOriginalSize, dwCompressedSize, UnCompSize);

		CSound* sound = new CSound(uncompressedBuf, dwOriginalSize); // ���� ���� (�׽�Ʈ)
		aData[i] = sound; // �����̳ʿ� ���� ���� (�׽�Ʈ)
		//free(uncompressedBuf);
		*/
		DWORD dwOriginalSize = reader.Read<DWORD>();
		void* buf = reader.ReadBinary(dwOriginalSize);

		LPLogger::WriteConsoleW(L"[%d] ���� ũ�� : %d\n", i, dwOriginalSize);

		CSound* sound = new CSound(buf, dwOriginalSize); // ���� ���� (�׽�Ʈ)
		aData[i] = sound; // �����̳ʿ� ���� ���� (�׽�Ʈ)
						  //free(uncompressedBuf);
	}

	LPLogger::WriteConsoleW(L"%s ���� ��Ʈ���� %d�� ���� �ε�\n", sKey.c_str(), dwFilesCount);


}