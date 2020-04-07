#include "Tools.h"
BOOL Tools::InRect(RECT* rect, LONG x, LONG y) {
	return rect->left <= x && rect->right >= x && rect->top <= y && rect->bottom >= y;
}