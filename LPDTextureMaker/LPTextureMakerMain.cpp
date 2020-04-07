#include <LPData.h>
#include <zlib.h>
#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <string>
#include <map>

#pragma comment(lib, "zlib.lib")
#define ZLIB_WINAPI
#define TRIM_SPACE " \t\n\v"
namespace ospace {
	inline std::string trim(std::string& s, const std::string& drop = TRIM_SPACE)
	{
		std::string r = s.erase(s.find_last_not_of(drop) + 1);
		return r.erase(0, r.find_first_not_of(drop));
	}
	inline std::string rtrim(std::string s, const std::string& drop = TRIM_SPACE)
	{
		return s.erase(s.find_last_not_of(drop) + 1);
	}
	inline std::string ltrim(std::string s, const std::string& drop = TRIM_SPACE)
	{
		return s.erase(0, s.find_first_not_of(drop));
	}
}
DWORD dwFieldSize = 0;
void GetLines(wchar_t* sFileName, std::vector<std::string>* vector);
void GetTagLines(wchar_t* strTag, std::vector<std::string>* input, std::vector<std::string>* output);
void SplitString(__in const std::string& instring, __in const std::string& token, std::vector<std::string>* output);


int GetMaxCompressedLen(int nLenSrc)
{
	int n16kBlocks = (nLenSrc + 16383) / 16384; // round up any fraction of a block
	return (nLenSrc + 6 + (n16kBlocks * 5));
}

int CompressData(const BYTE* abSrc, int nLenSrc, BYTE* abDst, int nLenDst)
{
	z_stream zInfo = { 0 };
	zInfo.total_in = zInfo.avail_in = nLenSrc;
	zInfo.total_out = zInfo.avail_out = nLenDst;
	zInfo.next_in = (BYTE*)abSrc;
	zInfo.next_out = abDst;

	int nErr, nRet = -1;
	nErr = deflateInit(&zInfo, Z_DEFAULT_COMPRESSION); // zlib function
	if (nErr == Z_OK) {
		nErr = deflate(&zInfo, Z_FINISH);              // zlib function
		if (nErr == Z_STREAM_END) {
			nRet = zInfo.total_out;
		}
	}
	deflateEnd(&zInfo);    // zlib function
	return(nRet);
}

void PackingTextures(wchar_t *Dir)
{
	WCHAR sFileOutName[260];
	wsprintfW(sFileOutName, L"Data/TextureSet/%s.lpd", Dir);

	FILE *FileOut;
	_wfopen_s(&FileOut, sFileOutName, L"wb");
	DWORD dwIndex = 0;

	fwrite(&dwIndex, 4, 1, FileOut);

	for (;;)
	{
		WCHAR sFileName[260];
		wsprintfW(sFileName, L"Packages/TextureSet/%s/%d.png", Dir, dwIndex);
		FILE *FileIn;
		if (_wfopen_s(&FileIn, sFileName, L"rb"))
		{
			break;
		}

		//입력 파일의 사이즈를 구한다.
		fseek(FileIn, 0, SEEK_END);
		DWORD FileInSize = ftell(FileIn);

		// raw 데이터와 압축 데이터의 버퍼를 생성한다.
		void *RawDataBuff = malloc(FileInSize);
		
		
		/*
		
		void *CompDataBuff = NULL;

		// zlib 에서는zlib 데이터스트림의 오버헤드를 막기위해 압축 버퍼 사이즈는 최소 원본 버퍼 + 12 바이트보다 0.1 배는 커야한다고 밝힌다. ( 밑에 밑줄은 source -> dest 가 오타가 아닌지 의심)
		int CompBuffSize = GetMaxCompressedLen(FileInSize);
		CompDataBuff = malloc((size_t)(CompBuffSize));

		*/

		//파일의 데이터를 소스 버퍼로 읽어들인다.
		fseek(FileIn, 0, SEEK_SET);
		fread(RawDataBuff, FileInSize, 1, FileIn);

		/*
		//데이터를 압축한다.
		uLongf DestBuffSize;
		DestBuffSize = CompressData((LPBYTE)RawDataBuff, FileInSize, (LPBYTE)CompDataBuff, CompBuffSize);
		wprintf(L"Packages/TextureSet/%s/%d.png [%d] %d -> %d\n", Dir, dwIndex, CompBuffSize, FileInSize, DestBuffSize);
		*/

		wprintf(L"Packages/TextureSet/%s/%d.png [%d bytes]\n", Dir, dwIndex, FileInSize);

		DWORD dwTransColorKey = 0x0;

		// 압축된 데이터를 디스크에 기록
		fwrite(&FileInSize, 4, 1, FileOut);   // 원본 크기
		//fwrite(&DestBuffSize, 4, 1, FileOut); // 압축 후 크기
		fwrite(&dwTransColorKey, 4, 1, FileOut); // 컬러키

		//fwrite(CompDataBuff, DestBuffSize, 1, FileOut);
		fwrite(RawDataBuff, FileInSize, 1, FileOut);

		//free(CompDataBuff);
		free(RawDataBuff);
		fclose(FileIn);

		dwIndex++;
	}

	fseek(FileOut, 0, SEEK_SET);
	fwrite(&dwIndex, 4, 1, FileOut);
	fclose(FileOut);
	wprintf(L"Packages/TextureSet/%s  Packing Complete!!\n\n", Dir);
}

void PackingSound(wchar_t* Dir)
{
	WCHAR sFileOutName[260];
	wsprintfW(sFileOutName, L"Data/Sound/%s.lpd", Dir);

	FILE *FileOut;
	_wfopen_s(&FileOut, sFileOutName, L"wb");
	DWORD dwIndex = 0;

	fwrite(&dwIndex, 4, 1, FileOut);

	for (;;)
	{
		WCHAR sFileName[260];
		wsprintfW(sFileName, L"Packages/Sound/%s/%d.mp3", Dir, dwIndex);
		FILE *FileIn;
		if (_wfopen_s(&FileIn, sFileName, L"rb"))
		{
			break;
		}

		//입력 파일의 사이즈를 구한다.
		fseek(FileIn, 0, SEEK_END);
		DWORD FileInSize = ftell(FileIn);

		// raw 데이터와 압축 데이터의 버퍼를 생성한다.
		void *RawDataBuff = malloc(FileInSize);

		/*
		void *CompDataBuff = NULL;

		// zlib 에서는zlib 데이터스트림의 오버헤드를 막기위해 압축 버퍼 사이즈는 최소 원본 버퍼 + 12 바이트보다 0.1 배는 커야한다고 밝힌다. ( 밑에 밑줄은 source -> dest 가 오타가 아닌지 의심)
		int CompBuffSize = GetMaxCompressedLen(FileInSize);
		CompDataBuff = malloc((size_t)(CompBuffSize));

		*/

		//파일의 데이터를 소스 버퍼로 읽어들인다.
		fseek(FileIn, 0, SEEK_SET);
		fread(RawDataBuff, FileInSize, 1, FileIn);

		/*
		//데이터를 압축한다.
		uLongf DestBuffSize;
		DestBuffSize = CompressData((LPBYTE)RawDataBuff, FileInSize, (LPBYTE)CompDataBuff, CompBuffSize);
		wprintf(L"Packages/Sound/%s/%d.mp3 [%d] %d -> %d\n", Dir, dwIndex, CompBuffSize, FileInSize, DestBuffSize);
		*/
		wprintf(L"Packages/Sound/%s/%d.mp3 [%d bytes]\n", Dir, dwIndex, FileInSize);


		// 압축된 데이터를 디스크에 기록
		fwrite(&FileInSize, 4, 1, FileOut);   // 원본 크기
		//fwrite(&DestBuffSize, 4, 1, FileOut); // 압축 후 크기

		fwrite(RawDataBuff, FileInSize, 1, FileOut);

		//free(CompDataBuff);
		free(RawDataBuff);
		fclose(FileIn);

		dwIndex++;
	}

	fseek(FileOut, 0, SEEK_SET);
	fwrite(&dwIndex, 4, 1, FileOut);
	fclose(FileOut);
	wprintf(L"Packages/Sound/%s  Packing Complete!!\n\n", Dir);
}

void PackingFields(FILE *FileOut, DWORD Dir, std::map<DWORD, long> *filePos)
{
	size_t i;
	long startPos;
	long layerStartPos;

	startPos = ftell(FileOut);
	(*filePos)[Dir] = startPos + 8 + (8 * dwFieldSize);

	// 데이터 파싱
	{

		WCHAR sFileName[260];
		wsprintfW(sFileName, L"Packages/Field/%d/Information.txt", Dir);
		FILE *FileIn;
		if (0 == _wfopen_s(&FileIn, sFileName, L"rb"))
		{
			char Sign = 1;
			fwrite(&Sign, 1, 1, FileOut);

			// 텍스트 파일을 파싱한다.
			std::vector<std::string> aAllLines;
			GetLines(sFileName, &aAllLines);

			LPFieldInfo lpFieldInfo;
			ZeroMemory(&lpFieldInfo, sizeof(LPFieldInfo));


			// 태그들을 로드하고 구조체에 값을 입력한다.
			{
				// Music
				std::vector<std::string> aTextureLines;
				GetTagLines(L"Music", &aAllLines, &aTextureLines);
				size_t szLen = aTextureLines.size();
				if (szLen > 0)
				{
					std::vector<std::string> aSplitted;
					SplitString(aTextureLines[0], " ", &aSplitted);
					memcpy_s(lpFieldInfo.aMusic, sizeof(lpFieldInfo.aMusic), aSplitted[0].c_str(), aSplitted[0].length());
					lpFieldInfo.dwMusicIndex = atoi(aSplitted[1].c_str());

				}
			}
			{
				// FieldType
				std::vector<std::string> aTextureLines;
				GetTagLines(L"FieldType", &aAllLines, &aTextureLines);
				size_t szLen = aTextureLines.size();
				if (szLen > 0)
				{
					std::vector<std::string> aSplitted;
					SplitString(aTextureLines[0], " ", &aSplitted);
					lpFieldInfo.dwFieldType = atoi(aSplitted[0].c_str());

				}
			}

			// 구조체를 바이너리 형태로 파일로 작성한다.
			fwrite(&lpFieldInfo, sizeof(LPFieldInfo), 1, FileOut);

		}
		else
		{
			char Sign = 0;
			fwrite(&Sign, 1, 1, FileOut);
		}
	}

	layerStartPos = ftell(FileOut);
	DWORD dwLayers = 0;
	fwrite(&dwLayers, 4, 1, FileOut);

	for (i = 1; i <= 7; i++)
	{
		WCHAR sFileName[260];
		wsprintfW(sFileName, L"Packages/Field/%d/Layer%d.txt", Dir, i);
		FILE *FileIn;
		if (_wfopen_s(&FileIn, sFileName, L"rb"))
		{
			continue;
		}

		fwrite(&i, 4, 1, FileOut);

		// 텍스트 파일을 파싱한다.
		std::vector<std::string> aAllLines;
		GetLines(sFileName, &aAllLines);

		// 텍스쳐 파트 태그를 로드한다.
		std::vector<std::string> aTextureLines;
		GetTagLines(L"Texture", &aAllLines, &aTextureLines);

		size_t szLen = aTextureLines.size();
		fwrite(&szLen, 4, 1, FileOut);

		// 텍스쳐 파트를 바이너리 포맷으로 변환한다.
		for (std::vector<std::string>::const_iterator it = aTextureLines.begin();
			it != aTextureLines.end(); it++)
		{
			std::vector<std::string> aSplitted;
			
			SplitString(*it, " ", &aSplitted);

			LPTexture lpTexture;
			ZeroMemory(&lpTexture, sizeof(lpTexture));

			lpTexture.uLen = (USHORT)aSplitted[0].length();
			memcpy_s(lpTexture.aBuf, sizeof(lpTexture.aBuf), aSplitted[0].c_str(), aSplitted[0].length());
			lpTexture.dwTextureIndex = atoi(aSplitted[1].c_str());
			lpTexture.dwOffsetX = atoi(aSplitted[2].c_str());
			lpTexture.dwOffsetY = atoi(aSplitted[3].c_str());
			lpTexture.fScaleX = (FLOAT)atof(aSplitted[4].c_str());
			lpTexture.fScaleY = (FLOAT)atof(aSplitted[5].c_str());
			lpTexture.dwRotation = atoi(aSplitted[6].c_str());
			lpTexture.uAlpha = (UCHAR)atoi(aSplitted[7].c_str());
			lpTexture.uRed = (UCHAR)atoi(aSplitted[8].c_str());
			lpTexture.uGreen = (UCHAR)atoi(aSplitted[9].c_str());
			lpTexture.uBlue = (UCHAR)atoi(aSplitted[10].c_str());

			fwrite(&lpTexture, sizeof(lpTexture), 1, FileOut);

			/*
			USHORT uLen = (USHORT)aSplitted[0].length();
			fwrite(&uLen, 2, 1, FileOut);
			fwrite(aSplitted[0].c_str(), aSplitted[0].length(), 1, FileOut);

			DWORD dwTextureIndex = atoi(aSplitted[1].c_str());
			fwrite(&dwTextureIndex, 4, 1, FileOut);

			DWORD dwOffsetX = atoi(aSplitted[2].c_str());
			fwrite(&dwOffsetX, 4, 1, FileOut);

			DWORD dwOffsetY = atoi(aSplitted[3].c_str());
			fwrite(&dwOffsetY, 4, 1, FileOut);

			FLOAT fScaleX = (FLOAT)atof(aSplitted[4].c_str());
			fwrite(&fScaleX, 4, 1, FileOut);

			FLOAT fScaleY = (FLOAT)atof(aSplitted[5].c_str());
			fwrite(&fScaleY, 4, 1, FileOut);

			DWORD dwRotation = atoi(aSplitted[6].c_str());
			fwrite(&dwRotation, 4, 1, FileOut);

			UCHAR uAlpha = (UCHAR)atoi(aSplitted[7].c_str());
			fwrite(&uAlpha, 1, 1, FileOut);

			UCHAR uRed = (UCHAR)atoi(aSplitted[8].c_str());
			fwrite(&uRed, 1, 1, FileOut);

			UCHAR uGreen = (UCHAR)atoi(aSplitted[9].c_str());
			fwrite(&uGreen, 1, 1, FileOut);

			UCHAR uBlue = (UCHAR)atoi(aSplitted[10].c_str());
			fwrite(&uBlue, 1, 1, FileOut);
			*/

		}
		dwLayers++;
	}

	


	long curPos = ftell(FileOut);
	fseek(FileOut, layerStartPos, SEEK_SET);
	fwrite(&dwLayers, 4, 1, FileOut);
	fseek(FileOut, curPos, SEEK_SET);
}


void __cdecl _tmain(int argc, TCHAR *argv[])
{
	WIN32_FIND_DATA FindFileData;
	HANDLE hFind;

	hFind = FindFirstFile(L"Packages/TextureSet/*", &FindFileData);
	if (hFind == INVALID_HANDLE_VALUE)  {
		return;
	}

	do {
		if (wcscmp(FindFileData.cFileName, L".") != 0 &&
			wcscmp(FindFileData.cFileName, L"..") != 0) {
			if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
				PackingTextures(FindFileData.cFileName);
			}
		}
	} while (FindNextFile(hFind, &FindFileData) != 0);

	FILE *FileOut;
	FILE *RealFileOut;
	DWORD dwSignature;
	size_t size;
	char buf[BUFSIZ];
	dwSignature = *(DWORD*) "LUPI";

	_wfopen_s(&FileOut, L"Data/Field.lpd.temp", L"wb");


	hFind = FindFirstFile(L"Packages/Field/*", &FindFileData);
	if (hFind == INVALID_HANDLE_VALUE)  {
		return;
	}

	std::map<DWORD, long> FieldOffset;

	do {
		if (wcscmp(FindFileData.cFileName, L".") != 0 &&
			wcscmp(FindFileData.cFileName, L"..") != 0) {
			if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
				dwFieldSize++;
			}
		}
	} while (FindNextFile(hFind, &FindFileData) != 0);


	hFind = FindFirstFile(L"Packages/Field/*", &FindFileData);
	do {
		if (wcscmp(FindFileData.cFileName, L".") != 0 &&
			wcscmp(FindFileData.cFileName, L"..") != 0) {
			if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
				PackingFields(FileOut, _wtoi(FindFileData.cFileName), &FieldOffset);
				wprintf(L"Packages/Field/%s  Packing Complete!!\n\n", FindFileData.cFileName);
			}
		}
	} while (FindNextFile(hFind, &FindFileData) != 0);

	_wfopen_s(&RealFileOut, L"Data/Field.lpd", L"wb");

	fwrite(&dwSignature, 4, 1, RealFileOut);
	fwrite(&dwFieldSize, 4, 1, RealFileOut);

	fclose(FileOut);
	_wfopen_s(&FileOut, L"Data/Field.lpd.temp", L"rb");

	for (std::map<DWORD, long>::const_iterator it = FieldOffset.begin();
		it != FieldOffset.end(); it++)
	{
		fwrite(&it->first, 4, 1, RealFileOut);
		fwrite(&it->second, sizeof(long), 1, RealFileOut);
	}

	while (size = fread(buf, 1, BUFSIZ, FileOut)) {
		fwrite(buf, 1, size, RealFileOut);
	}

	fclose(FileOut);
	fclose(RealFileOut);
	DeleteFile(L"Data/Field.lpd.temp");



	hFind = FindFirstFile(L"Packages/Sound/*", &FindFileData);
	do {
		if (wcscmp(FindFileData.cFileName, L".") != 0 &&
			wcscmp(FindFileData.cFileName, L"..") != 0) {
			if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
				PackingSound(FindFileData.cFileName);
			}
		}
	} while (FindNextFile(hFind, &FindFileData) != 0);


	getchar();
}

void GetLines(wchar_t* sFileName, std::vector<std::string>* vector)
{
	std::ifstream hFile(sFileName);
	std::string line;
	while (std::getline(hFile, line))
	{
		size_t pos;
		pos = line.find("//");
		if (pos != std::string::npos)
		{
			line = line.substr(0, pos);
		}
		line = ospace::ltrim(line);
		line = ospace::rtrim(line);
		if (line.length() > 0)
			vector->push_back(line);
	}
}

void GetTagLines(wchar_t* strTag, std::vector<std::string>* input, std::vector<std::string>* output)
{
	bool bIsOpened = 0;

	wchar_t strUnicode[256] = { 0, };
	char	strMultibyte[256] = { 0, };
	char    strTagStart[256] = { 0, };
	char    strTagEnd[256] = { 0, };
	wcscpy_s(strUnicode, 256, strTag);
	int len = WideCharToMultiByte(CP_ACP, 0, strUnicode, -1, NULL, 0, NULL, NULL);
	WideCharToMultiByte(CP_ACP, 0, strUnicode, -1, strMultibyte, len, NULL, NULL);
	sprintf_s(strTagStart, "[%s]", strMultibyte);
	sprintf_s(strTagEnd, "[/%s]", strMultibyte);
	std::string __strTagStart = std::string(strTagStart);
	std::string __strTagEnd = std::string(strTagEnd);


	for (std::vector<std::string>::const_iterator it = input->begin();
		it != input->end(); it++)
	{
		if (__strTagStart == *it) 
		{
			bIsOpened = 1;
			continue;
		}
		if (__strTagEnd == *it)
		{
			if (!bIsOpened)
			{
				throw 604;
			}
			bIsOpened = 0;
			return;
		}
		if (bIsOpened)
		{
			output->push_back(*it);
		}
	}

	throw 38;
}

void SplitString(__in const std::string& instring, __in const std::string& token, std::vector<std::string>* output)
{
	int begin = 0, end = 0;

	while (1) {

		end = instring.find(token.c_str(), begin, token.length());

		if (end == 0) { //처음일 경우
			begin = token.length();
			continue;
		}

		if (end < 0) {
			if (begin != instring.length()) {
				output->push_back(instring.substr(begin, instring.length() - begin));
			}
			break;
		}
		else {
			if (begin != end){
				output->push_back(instring.substr(begin, end - begin));
				begin = end + token.length();
			}
			// 끝일 경우
			else
			{
				begin += token.length();
			}
		}
	}
}