#include <fmod.h>
#include <fmod_errors.h>
#include "Sound.h"
#include "LPApp.h"
#include "LPLogger.h"


CSound::CSound(WCHAR *sFilePath)
{
	wchar_t strUnicode[256] = { 0, };
	char	strMultibyte[256] = { 0, };
	wcscpy_s(strUnicode, 256, sFilePath);
	int len = WideCharToMultiByte(CP_ACP, 0, strUnicode, -1, NULL, 0, NULL, NULL);
	WideCharToMultiByte(CP_ACP, 0, strUnicode, -1, strMultibyte, len, NULL, NULL);

	LPApp::FMOD__System->createSound(strMultibyte, FMOD_DEFAULT, 0, &sound);
}

CSound::CSound(void* buf, UINT len)
{
	this->buf = buf;
	FMOD_RESULT       result;
	FMOD_CREATESOUNDEXINFO exinfo;
	ZeroMemory(&exinfo, sizeof(FMOD_CREATESOUNDEXINFO));
	exinfo.cbsize = sizeof(FMOD_CREATESOUNDEXINFO);
	exinfo.length = len;
	result = LPApp::FMOD__System->createSound((const char *)buf, FMOD_OPENMEMORY | FMOD_DEFAULT, &exinfo, &sound);
	LPLogger::WriteConsoleA("Sound Load Result : %s (%d) / Len : %d\n", FMOD_ErrorString(result), result, len);
}

CSound::~CSound()
{
	sound->release();
	//free(buf);
	//reader로 생성된, MapOfView 버퍼이므로 지울 필요가 없음.
}
