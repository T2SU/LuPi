#pragma once
#include "Field.h"
namespace Field
{

	class CNormalField : public CField
	{
	public:
		CNormalField();
		~CNormalField();
		void Begin();
		void Render();
		void AddObject();
		DWORD GetFieldType();
	};


}