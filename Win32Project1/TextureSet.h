#pragma once
#include <boost/unordered_map.hpp>
#include "Texture.h"
class CTextureSet
{
private:
	std::wstring sKey;
	boost::unordered_map<int, CTexture*> aData;

	HANDLE hFile;
	HANDLE hMapHandle;
	void* map_ptr;

	void LoadTextureSet();
public:
	CTexture* Get (int key);
	CTextureSet(std::wstring key);
	~CTextureSet();
};

