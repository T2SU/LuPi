#include "FieldTexture.h"
#include "LPLogger.h"
#include <d3d9.h>
#include <d3dx9.h>

CFieldTexture::~CFieldTexture()
{
	pTexture = 0;
}

void CFieldTexture::Draw(LPD3DXSPRITE pSprite)
{
	if (SUCCEEDED(pSprite->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE)))
	{
		// Draw2D  테스트중
		D3DXMATRIX  mat; // 3d 행렬

		D3DXVECTOR2 pos(fOffsetX, fOffsetY);//화면 좌표
		D3DXVECTOR2 scale(fScaleX, fScaleY);//스케일
		RECT		rect = { 0, 0, (LONG)pTexture->info.Width, (LONG)pTexture->info.Height }; // 그릴 행렬 사이즈

		D3DXVECTOR2* rotateCenter = NULL;

		if ( 0 != fRotation )
			rotateCenter = new D3DXVECTOR2(pTexture->info.Width / 2.0f, pTexture->info.Height / 2.0f);
		
		D3DXMatrixTransformation2D(&mat, NULL, 0, &scale, rotateCenter, fRotation, &pos);

		if (rotateCenter)
			delete rotateCenter;
		
		/*LPLogger::WriteConsoleW(L" %d %d %d %d \n %d %d %d %d \n %d %d %d %d \n %d %d %d %d \n ----------------------------------- \n",
			mat._11, mat._12, mat._13, mat._14,
			mat._21, mat._22, mat._23, mat._24,
			mat._31, mat._32, mat._33, mat._34,
			mat._41, mat._42, mat._43, mat._44);*/

		pSprite->SetTransform(&mat);
		pSprite->Draw(
			pTexture->pTexture, // pTexture
			&rect,
			NULL,
			NULL,
			dwColor // dwColor
			);

		pSprite->End();
	}
}

// 텍스쳐 영역을 구함
void CFieldTexture::TextureArea(RECT* rect)
{
	// Rotation 된 영역을 정확하게 구하진 않음
	rect->top = (LONG) fOffsetY;
	rect->left = (LONG)fOffsetX;
	rect->right = (LONG)(fOffsetX + pTexture->info.Width * fScaleX);
	rect->bottom = (LONG)(fOffsetY + pTexture->info.Height * fScaleY);
}