#pragma once

namespace LPLogger
{
	void MBA(const char* text, ...);
	void WriteConsoleA(const char* text, ...);
	void WriteConsoleW(const wchar_t* text, ...);
}