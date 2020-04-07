#include "StreamReader.h"


StreamReader::~StreamReader()
{
}

void* StreamReader::ReadBinary(unsigned int len)
{
	if ( end && ptr + len > end )
		throw 38; // End Of File
	void *ret = ptr;
	ptr += len;
	return ret;
}


void StreamReader::Seek(unsigned int pos)
{
	ptr = initial + pos;
}

unsigned int StreamReader::Remaining()
{
	if (0 == end)
	{
		throw 38;
	}
	return end - ptr;
}