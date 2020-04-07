#pragma once

#include <Windows.h>

struct LPTexture
{
	USHORT uLen;
	char aBuf[36];
	DWORD dwTextureIndex;
	DWORD dwOffsetX;
	DWORD dwOffsetY;
	FLOAT fScaleX;
	FLOAT fScaleY;
	DWORD dwRotation;
	UCHAR uAlpha;
	UCHAR uRed;
	UCHAR uGreen;
	UCHAR uBlue;
};

struct LPFieldInfo
{
	char aMusic[256];
	DWORD dwMusicIndex;
	DWORD dwFieldType;
};