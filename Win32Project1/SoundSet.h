#pragma once
#include <boost/unordered_map.hpp>
#include "Sound.h"
class CSoundSet
{
private:
	std::wstring sKey;
	boost::unordered_map<int, CSound*> aData;

	HANDLE hFile;
	HANDLE hMapHandle;
	void* map_ptr;

	void LoadSoundSet();
public:
	CSound* Get(int key);
	CSoundSet(std::wstring key);
	~CSoundSet();
};

