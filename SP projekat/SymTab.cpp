#include "SymTab.h"

using namespace std;

void SymTab::addSym(string sym, bool def, Section *section, STT type, STB binding, int val)
{
	Elf_Sym *old, s = Elf_Sym(sym, def, section, type, binding, val);

	for (resetIterator(); getBoolIt(); iteratorNext())  // ako postoji, azuriraj
	{
		old = getItEntPointer();
		if (sym.compare(old->getName()) == 0)
		{
			old->copyInfo(s);
			//replItEnt(s);
			return;
		}
	}
	endInsert(s);
}

Elf_Sym *SymTab::addUndSym(string sym, int off)
{
	Elf_Sym *s = new Elf_Sym(sym, false, NULL, STT_OBJECT, STB_LOCAL);
	endInsert(*s);
	return getItEntPointer();
}

Elf_Sym *SymTab::getSymRef(string name)
{
	Elf_Sym *tek;
	for (resetIterator(); getBoolIt(); iteratorNext())
	{
		tek = getItEntPointer();
		if (name.compare(tek->getName()) == 0) return tek;
	}
	return NULL;
}