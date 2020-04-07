
#include "Tools.h"
#include "LPApp.h"
#include <iostream>
#include <Windows.h>

INT WINAPI wWinMain(HINSTANCE hInst, HINSTANCE, LPWSTR, INT)
{
	UNREFERENCED_PARAMETER(hInst);

	BOOL fullscreen = GetFileAttributes(L"HellGuy.txt") != INVALID_FILE_ATTRIBUTES;

	WNDCLASSEX wc;

	ZeroMemory(&wc, sizeof(WNDCLASSEX));

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = LPApp::MsgProc;
	wc.hInstance = hInst;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;    // not needed any more
	wc.lpszClassName = L"LuPiClass";

	RECT wr = { 0, 0, 1024, 768 };    // set the size, but not the position
	if (!fullscreen)
		AdjustWindowRect(&wr, WS_CAPTION | WS_SYSMENU, FALSE);    // adjust the size

	RegisterClassEx(&wc);
	HWND hWnd = CreateWindowEx(NULL, L"LuPiClass", L"LuPi",
		fullscreen ? (WS_EX_TOPMOST | WS_POPUP) : WS_CAPTION | WS_SYSMENU,
		fullscreen ? 0 : 100,
		fullscreen ? 0 : 100,
		wr.right - wr.left, wr.bottom - wr.top,
		NULL, NULL, wc.hInstance, NULL);

	ShowWindow(hWnd, SW_SHOWDEFAULT);
	UpdateWindow(hWnd);


	CSecType<int> test(1);


	LPLogger::WriteConsoleA("\n");
	LPLogger::WriteConsoleA("Test: %d\n", (int)test);
	
	test = 1234;

	LPLogger::WriteConsoleA("Test: %d\n", (int)test);

	int test2 = test;
	LPLogger::WriteConsoleA("Test2: %d\n", test2);

	test2 += 1234;
	LPLogger::WriteConsoleA("Test2: %d\n", test2);


	CSecType<long double> test3(247.247);
	LPLogger::WriteConsoleA("\n");
	LPLogger::WriteConsoleA("Test3: %%F %%F\n", (long double)test3, 247.247);
	test3 = 247.247247;
	LPLogger::WriteConsoleA("Test3: %f\n", (long double)test3);

	
	
	


	if ( LPApp::Initialize( hWnd ) ) 
		LPApp::EventLoop();

	UnregisterClass(L"LuPiClass", wc.hInstance);
	return 0;
}
