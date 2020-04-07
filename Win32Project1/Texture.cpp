#include "Texture.h"
#include "LPLogger.h"
#include "Global.h"

CTexture::CTexture(WCHAR *sFilePath)
{
	D3DXCreateTextureFromFileExW(D3D9::D3DDevice, sFilePath,
		D3DX_DEFAULT_NONPOW2, // 이미지 가로 크기
		D3DX_DEFAULT_NONPOW2, // 이미지 세로 크기 // 자동으로 이미지의 크기에 맞추어 불러온다.
		1,      // Miplavels   //3D 상에서 확대 축소시 사용한다.깨지는 현상방지를 위해..
		0,      // Usage    //사용 용도 출력 용도로는 0으로 지정
		D3DFMT_A8B8G8R8,  // 이미지 색상 포멧 //각 8bit 사용 이미지
		D3DPOOL_MANAGED,  // pool    //이미지관리 다이렉트가 직접관리
		D3DX_FILTER_NONE,  // Filter    //확대 축소시 사용하는 필터
		D3DX_FILTER_NONE,  // MipFilter   //확대 축소시 사용하는 필터
		0,         // 컬러키 설정  //배경을 지울때.. png는 투명을 가진다. 그래서 컬러키 사용안헤도 된다.
		&info,     // 불러온 이미지의 정보를 저장 할 구조체
		NULL,      // 팔레트 설정  //8bit 인경우 색상표
		&pTexture);
}

CTexture::CTexture(void* buf, UINT len, DWORD dwTransColorKey)
{
	if (FAILED(D3DXCreateTextureFromFileInMemoryEx(D3D9::D3DDevice, buf, len,
		D3DX_DEFAULT_NONPOW2, // 이미지 가로 크기
		D3DX_DEFAULT_NONPOW2, // 이미지 세로 크기 // 자동으로 이미지의 크기에 맞추어 불러온다.
		1,      // Miplavels   //3D 상에서 확대 축소시 사용한다.깨지는 현상방지를 위해..
		0,      // Usage    //사용 용도 출력 용도로는 0으로 지정
		D3DFMT_A8B8G8R8,  // 이미지 색상 포멧 //각 8bit 사용 이미지
		D3DPOOL_MANAGED,  // pool    //이미지관리 다이렉트가 직접관리
		D3DX_FILTER_NONE,  // Filter    //확대 축소시 사용하는 필터
		D3DX_FILTER_NONE,  // MipFilter   //확대 축소시 사용하는 필터
		dwTransColorKey,         // 컬러키 설정  //배경을 지울때.. png는 투명을 가진다. 그래서 컬러키 사용안헤도 된다.
		&info,     // 불러온 이미지의 정보를 저장 할 구조체
		NULL,      // 팔레트 설정  //8bit 인경우 색상표
		&pTexture)))
	{
		LPLogger::WriteConsoleW(L"Texture Load 실패!");
	}
	else
		LPLogger::WriteConsoleW(L"Texture Load 성공!");
}

CTexture::~CTexture()
{
	pTexture->Release();
}
