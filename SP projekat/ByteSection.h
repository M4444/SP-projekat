#pragma once

#include "Section.h"
#include "Table.h"

typedef char dword1122[4];

typedef struct DW 
{
	unsigned char a[4];

	unsigned char &operator[](int n) { return a[n]; }
	DW() { for (int i = 0; i < 4; i++) a[i] = 0; }
} dword;

class ByteSection : public Section, public Table<dword>
{
public:
	ByteSection(string secName, SHT	secType, int attributes, int address = 0, int secSize = 0):
		Section(secName, secType, attributes, address, secSize){}

	~ByteSection();
};

