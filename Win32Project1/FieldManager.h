#pragma once

#include "Field.h"

namespace FieldManager
{
	void GetField(DWORD dwFieldId, Field::CField** out);
	void RegisterTransferField(DWORD dwTarget);
	void PostTransferField();
}