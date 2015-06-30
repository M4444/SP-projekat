#pragma once

#include <iostream>
#include <fstream>
#include <iomanip>
#include "Section.h"
#include "Table.h"
#include "Elf_Sym.h"

typedef char dword1122[4];

typedef struct DW 
{
	unsigned char a[4];

	unsigned char &operator[](int n) { return a[n]; }
	DW() { for (int i = 0; i < 4; i++) a[i] = 0; }
	//
	void sumOr(DW dw) 
	{ 
		for (int i = 0; i < 4; i++) 
			a[i] |= dw[i]; 
	}
	//
	void outDword(ofstream *output)
	{
		ofstream &out = *output;
		for (int i = 0; i < 4; i++) out << setfill('0') << setw(2) << std::hex << (int)a[i] << " ";
		//out << std::hex << (int)a[0] << ' ' << a[1] << ' ' << a[2] << ' ' << a[3];
	}
} dword;

class ByteSection : public Section, public Table<dword>
{
public:
	ByteSection(string secName, SHT	secType, int attributes, int address = 0, int secSize = 0):
		Section(secName, secType, attributes, address, secSize){}

	~ByteSection();

	void replace(dword dw, int offset);
	void outTab(ofstream *output);
};

