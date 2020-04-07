#pragma once
#include <Windows.h>
#include <time.h>
#include <random>

#include "LPLogger.h"

namespace Tools
{
	BOOL InRect(RECT* rect, LONG x, LONG y);
}

static std::mt19937 mt_rand((unsigned int)time(0));

template <typename T>
class CSecType
{
private:
	char* pBuf;
	char* pKey;
public:
	CSecType(T dat)
	{
		size_t sz = sizeof(T);
		pBuf = (char*)malloc( sz );
		pKey = (char*)malloc( sz );
		Fuse(dat);
	}
	~CSecType()
	{
		free(pBuf);
		free(pKey);
	}

	T Tear()
	{
		T ret;
		size_t bytes = sizeof(T);
		char* tmpBuf = (char*)malloc(bytes);
		memset(tmpBuf, 0, bytes);
		memcpy_s(tmpBuf, bytes, pBuf, bytes);

		for (size_t i = 0; i < bytes; i += 4)
		{
			LONG* plToSec = (LONG*)(tmpBuf + i);
			LONG tB = *(LONG*)(pKey + i);
			LONG tA = *plToSec;
			tB = (LONG)_rotl((unsigned int)tB, 3);
			tA = (LONG)_rotr((unsigned int)tA, 11);
			tA ^= tB;
			*plToSec = tA;
		}
		memcpy_s(&ret, bytes, tmpBuf, bytes);
		free(tmpBuf);
		return ret;
	}

	void Fuse(T newValue)
	{
		size_t bytes = sizeof(T);
		for (size_t i = 0; i < bytes; i += 4)
		{
			LONG* ptr = (LONG*)(pKey + i);
			*ptr = (LONG)mt_rand();
		}

		char* tmpBuf = (char*)malloc(bytes);
		memset(tmpBuf, 0, bytes);
		memcpy_s(tmpBuf, bytes, &newValue, bytes);

		for (size_t i = 0; i < bytes; i += 4)
		{
			LONG* plToSec = (LONG*)tmpBuf + i;
			LONG tB = *(LONG*)(pKey + i);
			LONG tA = *plToSec;
			tB = (LONG)_rotl((unsigned int)tB, 3);
			tA ^= tB;
			tA = (LONG)_rotl((unsigned int)tA, 11);
			*plToSec = tA;
		}
		memcpy_s(pBuf, bytes, tmpBuf, bytes);
		free(tmpBuf);
	}

	void operator = (T newValue)
	{
		Fuse(newValue);
	}

	operator T()
	{
		return (T)Tear();
	}
};