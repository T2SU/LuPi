#pragma once

class StreamReader
{
private:
	unsigned char *initial;
	unsigned char *ptr;
	unsigned char *end;
public:
	StreamReader(void* init) {
		this->initial = (unsigned char*)init;
		this->ptr = this->initial;
		this->end = 0;
	}
	StreamReader(void* init, void* end) : StreamReader( init ) { this->end = (unsigned char*) end; };
	StreamReader(void* init, int len)   : StreamReader( init, (char*) init + len ) {};
	~StreamReader();
	template <typename T>
	T Read()
	{
		return *reinterpret_cast<T *>( ReadBinary( sizeof( T ) ) );
	}
	void* ReadBinary(unsigned int len);
	void Seek(unsigned int pos);

	unsigned int Remaining();
};

