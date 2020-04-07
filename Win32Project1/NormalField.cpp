#include "NormalField.h"
#include "FieldTexture.h"
#include "LPApp.h"
#include "Global.h"
#include <vector>
#include <d3dx9.h>


Field::CNormalField::CNormalField()
{
}


Field::CNormalField::~CNormalField()
{
}


void Field::CNormalField::Begin()
{
	DWORD dwBackColor = D3DCOLOR_XRGB(0, 0, 0);
	D3D9::D3DDevice->Clear(0, NULL, D3DCLEAR_TARGET, dwBackColor, 1.0f, 0);
}


void Field::CNormalField::Render()
{
	std::for_each(__LAYER1_aTexture.begin(), __LAYER1_aTexture.end(), [](CFieldTexture* p){ p->Draw(D3D9::D3DXSprite); });
	std::for_each(__LAYER2_aTexture.begin(), __LAYER2_aTexture.end(), [](CFieldTexture* p){ p->Draw(D3D9::D3DXSprite); });
	std::for_each(__LAYER3_aTexture.begin(), __LAYER3_aTexture.end(), [](CFieldTexture* p){ p->Draw(D3D9::D3DXSprite); });
	std::for_each(__LAYER4_aTexture.begin(), __LAYER4_aTexture.end(), [](CFieldTexture* p){ p->Draw(D3D9::D3DXSprite); });
	std::for_each(__LAYER5_aTexture.begin(), __LAYER5_aTexture.end(), [](CFieldTexture* p){ p->Draw(D3D9::D3DXSprite); });
	std::for_each(__LAYER6_aTexture.begin(), __LAYER6_aTexture.end(), [](CFieldTexture* p){ p->Draw(D3D9::D3DXSprite); });
	std::for_each(__LAYER7_aTexture.begin(), __LAYER7_aTexture.end(), [](CFieldTexture* p){ p->Draw(D3D9::D3DXSprite); });

	// Draw Player
	Instance::Player->DrawObject();
}

DWORD Field::CNormalField::GetFieldType()
{
	return 1;
}