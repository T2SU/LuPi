#include "Texture.h"
#include "LPLogger.h"
#include "Global.h"

CTexture::CTexture(WCHAR *sFilePath)
{
	D3DXCreateTextureFromFileExW(D3D9::D3DDevice, sFilePath,
		D3DX_DEFAULT_NONPOW2, // �̹��� ���� ũ��
		D3DX_DEFAULT_NONPOW2, // �̹��� ���� ũ�� // �ڵ����� �̹����� ũ�⿡ ���߾� �ҷ��´�.
		1,      // Miplavels   //3D �󿡼� Ȯ�� ��ҽ� ����Ѵ�.������ ��������� ����..
		0,      // Usage    //��� �뵵 ��� �뵵�δ� 0���� ����
		D3DFMT_A8B8G8R8,  // �̹��� ���� ���� //�� 8bit ��� �̹���
		D3DPOOL_MANAGED,  // pool    //�̹������� ���̷�Ʈ�� ��������
		D3DX_FILTER_NONE,  // Filter    //Ȯ�� ��ҽ� ����ϴ� ����
		D3DX_FILTER_NONE,  // MipFilter   //Ȯ�� ��ҽ� ����ϴ� ����
		0,         // �÷�Ű ����  //����� ���ﶧ.. png�� ������ ������. �׷��� �÷�Ű �����쵵 �ȴ�.
		&info,     // �ҷ��� �̹����� ������ ���� �� ����ü
		NULL,      // �ȷ�Ʈ ����  //8bit �ΰ�� ����ǥ
		&pTexture);
}

CTexture::CTexture(void* buf, UINT len, DWORD dwTransColorKey)
{
	if (FAILED(D3DXCreateTextureFromFileInMemoryEx(D3D9::D3DDevice, buf, len,
		D3DX_DEFAULT_NONPOW2, // �̹��� ���� ũ��
		D3DX_DEFAULT_NONPOW2, // �̹��� ���� ũ�� // �ڵ����� �̹����� ũ�⿡ ���߾� �ҷ��´�.
		1,      // Miplavels   //3D �󿡼� Ȯ�� ��ҽ� ����Ѵ�.������ ��������� ����..
		0,      // Usage    //��� �뵵 ��� �뵵�δ� 0���� ����
		D3DFMT_A8B8G8R8,  // �̹��� ���� ���� //�� 8bit ��� �̹���
		D3DPOOL_MANAGED,  // pool    //�̹������� ���̷�Ʈ�� ��������
		D3DX_FILTER_NONE,  // Filter    //Ȯ�� ��ҽ� ����ϴ� ����
		D3DX_FILTER_NONE,  // MipFilter   //Ȯ�� ��ҽ� ����ϴ� ����
		dwTransColorKey,         // �÷�Ű ����  //����� ���ﶧ.. png�� ������ ������. �׷��� �÷�Ű �����쵵 �ȴ�.
		&info,     // �ҷ��� �̹����� ������ ���� �� ����ü
		NULL,      // �ȷ�Ʈ ����  //8bit �ΰ�� ����ǥ
		&pTexture)))
	{
		LPLogger::WriteConsoleW(L"Texture Load ����!");
	}
	else
		LPLogger::WriteConsoleW(L"Texture Load ����!");
}

CTexture::~CTexture()
{
	pTexture->Release();
}
