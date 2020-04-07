#include <Windows.h>
#include <stdarg.h>
#include "LPLogger.h"

// 일단 임시로 OutputDebugMessage를 통해 출력됨

void LPLogger::MBA(const char* sText, ...)
{
#ifdef _DEBUG
	char sOutput[260] = { 0, };
	va_list valist;

	va_start(valist, sText);

	wvsprintfA(sOutput, sText, valist);

	va_end(valist);

	MessageBoxA(NULL, sOutput, "", 0);
#endif
}

void LPLogger::WriteConsoleA(const char* sText, ...)
{
#ifdef _DEBUG
	char sOutput[260] = { 0, };
	va_list valist;

	va_start(valist, sText);
	
	wvsprintfA(sOutput, sText, valist);
	
	va_end(valist);

	OutputDebugStringA(sOutput);
#endif
}

void LPLogger::WriteConsoleW(const wchar_t* sText, ...)
{
#ifdef _DEBUG
	wchar_t sOutput[260] = { 0, };
	va_list valist;

	va_start(valist, sText);

	wvsprintfW(sOutput, sText, valist);

	va_end(valist);

	OutputDebugStringW(sOutput);
#endif
}