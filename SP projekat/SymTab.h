#pragma once

#include <string>
#include "Section.h"
#include "Elf_Sym.h"
#include "Table.h"

using namespace std;

class SymTab : public Section, public Table<Elf_Sym>
{
public:
	//SymTab();
	SymTab(int address = 0, int secSize = 0):
		Section(".symtab", SHT_SYMTAB, 0, address, secSize){}

	void addSym(string sym, bool defined, Section *section, STT type, STB binding, int val = 0);
	Elf_Sym *addUndSym(string sym, int off);
	Elf_Sym *getSymRef(string name);
};