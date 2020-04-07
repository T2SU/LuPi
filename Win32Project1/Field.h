#pragma once
#include <vector>
#include <LPData.h>
#include "Sound.h"
#include "StreamReader.h"
#include "FieldTexture.h"

namespace Field
{
	class CField
	{
	protected:
		std::vector<CFieldTexture*> __LAYER1_aTexture;
		std::vector<CFieldTexture*> __LAYER2_aTexture;
		std::vector<CFieldTexture*> __LAYER3_aTexture;
		std::vector<CFieldTexture*> __LAYER4_aTexture;
		std::vector<CFieldTexture*> __LAYER5_aTexture;
		std::vector<CFieldTexture*> __LAYER6_aTexture;
		std::vector<CFieldTexture*> __LAYER7_aTexture;
		CSound* BackgroundMusic;
	private:
		DWORD dwFieldId;
	public:
		CField();
		~CField();
		void SetFieldId(DWORD id) { dwFieldId = id; }
		DWORD GetFieldId() { return dwFieldId; }
		virtual DWORD GetFieldType() = 0;
		void LoadField(StreamReader* reader, LPFieldInfo* lpFieldInfo);
		void AddTexture(DWORD Layer, CFieldTexture* Texture);
		virtual void Begin();
		virtual void Render() = 0;
	};
}

