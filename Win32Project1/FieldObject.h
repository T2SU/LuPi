#pragma once
#include <Windows.h>
#include "Tools.h"

namespace Field
{
	class CFieldObject
	{
	private:
		CSecType<LONG> secX;
		CSecType<LONG> secY;
		CSecType<LONG> secAnimationIndex;
		CSecType<LONG> secDirection;
	protected:
		DWORD dwAnimationDelay;
		DWORD dwAnimationLastUpdated;
	public:
		void SetX(LONG X) { secX = X; }
		void SetY(LONG Y) { secY = Y; }
		LONG GetX() { return secX; }
		LONG GetY() { return secY; }
		void SetAnimationIndex(LONG AnimationIndex) { secAnimationIndex = AnimationIndex; }
		LONG GetAnimationIndex() { return secAnimationIndex; }
		void SetDirection(LONG Direction) { secDirection = Direction; }
		LONG GetDirection() { return secDirection; }

		virtual void DrawObject() = 0;

		CFieldObject();
		~CFieldObject();
	};
}