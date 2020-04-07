#pragma once
#include <Windows.h>
#include <zlib.h>

int UncompressData(const BYTE* abSrc, int nLenSrc, BYTE* abDst, int nLenDst);