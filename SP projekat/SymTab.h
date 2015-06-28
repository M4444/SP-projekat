#pragma once

#include <string>
#include "Section.h"
#include "Elf_Sym.h"
#include "Table.h"

using namespace std;

class SymTab : public Section, public Table<Elf_Sym>
{
public:
	void add(string sym, Section *section, STT type, STB binding, int val = 0);
	//SymTab();
	SymTab(int address = 0, int secSize = 0):
		Section(".symtab", SHT_SYMTAB, 0/*?*/, address, secSize){}
};