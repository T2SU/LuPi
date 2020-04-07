#pragma once
#include "Field.h"

namespace Field
{
	class CMainMenuField : public CField
	{
	protected:
	public:
		CMainMenuField();
		~CMainMenuField();
		void Begin();
		void Render();
		void AddObject();
		DWORD GetFieldType();
	};


}