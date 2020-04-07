#include "MainMenuField.h"
#include "NormalField.h"
#include "Player.h"
#include "FieldTexture.h"
#include "FieldManager.h"
#include "LPLogger.h"
#include "LPApp.h"
#include <LPdata.h>

DWORD g_dwTargetField = -1;

void FieldManager::GetField(DWORD dwToLoadField, Field::CField** ppField)
{
	if (*ppField)
	{
		delete *ppField;
	}
	std::wstring sDataPath = L"Data/Field.lpd";

	HANDLE hFile = CreateFileW(sDataPath.c_str(), GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_FLAG_RANDOM_ACCESS, NULL);
	HANDLE hMapHandle = CreateFileMappingW(hFile, NULL, PAGE_READONLY | SEC_RESERVE, 0, 0, 0);
	void* map_ptr = MapViewOfFile(hMapHandle, FILE_MAP_READ, 0, 0, 0);

	StreamReader reader(map_ptr);        // 스트림 리더
	reader.Seek(4);

	DWORD dwFieldCount = reader.Read<DWORD>();
	LPLogger::WriteConsoleW(L"필드 갯수 : %d\n", dwFieldCount);
	for (size_t i = 0; i < dwFieldCount; i++)
	{
		DWORD dwFieldId = reader.Read<DWORD>();
		long pos = reader.Read<long>();
		if (dwFieldId == dwToLoadField)
		{
			LPLogger::WriteConsoleW(L"%d번 필드 찾았당~\n", dwToLoadField);
			reader.Seek((unsigned int)pos);

			Field::CField* out = 0; // 테스트 필드

			LPFieldInfo lpFieldInfo;

			if (1 == reader.Read<char>()) // Sign
			{
				void* tbuf = reader.ReadBinary(sizeof(LPFieldInfo));
				memcpy_s(&lpFieldInfo, sizeof(LPFieldInfo), tbuf, sizeof(LPFieldInfo));

				switch (lpFieldInfo.dwFieldType)
				{
				case 0:
					out = new Field::CMainMenuField;
					LPLogger::WriteConsoleA("FieldType : CMainMenuField\n");
					break;
				case 1:
					out = new Field::CNormalField;
					LPLogger::WriteConsoleA("FieldType : CNormalField\n");
					break;
				}
				
			}

			out->SetFieldId(dwToLoadField);
			out->LoadField(&reader, &lpFieldInfo);
			*ppField = out;
			break;
		}
	}

	UnmapViewOfFile(map_ptr);
	CloseHandle(hMapHandle);
	CloseHandle(hFile);
}

void FieldManager::RegisterTransferField(DWORD dwTarget)
{
	g_dwTargetField = dwTarget;
	LPApp::StartFadeEffect();
}

void FieldManager::PostTransferField()
{
	if (g_dwTargetField != -1)
	{
		FieldManager::GetField(g_dwTargetField, &Instance::Field);
		Instance::Player->InitUI();
		g_dwTargetField = -1;
	}
}