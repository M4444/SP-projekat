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

Elf_Sym *SymTab::getUnpSym()
{
	Elf_Sym *tek;
	for (resetIterator(); getBoolIt(); iteratorNext())
	{
		tek = getItEntPointer(); 
		FRefTab *f = tek->getFRefTab();
		if (f != NULL)
			if (f->boolFirst() == true) return tek;
	}
	return NULL;
}

void SymTab::setSymSize(string name, int size)
{
	Elf_Sym *tek;
	for (resetIterator(); getBoolIt(); iteratorNext())
	{
		tek = getItEntPointer();
		if (name.compare(tek->getName()) == 0) tek->setSize(size);
	}
}

void SymTab::setSymVal(string name, int size)
{
	Elf_Sym *tek;
	for (resetIterator(); getBoolIt(); iteratorNext())
	{
		tek = getItEntPointer();
		if (name.compare(tek->getName()) == 0) tek->setAddress(size);
	}
}

void SymTab::outTab(ofstream *output)
{
	ofstream &out = *output;
	
	Elf_Sym *tek;

	out << "#Symbol table" << endl;
	out << "#Num:\tName\tSec\tVal\tSize\tBind" << endl;
	out << "0:\t\tUND\t0\t0\tlocal" << endl;
	int i = 1;
	for (resetIterator(); getBoolIt(); iteratorNext())
	{
		tek = getItEntPointer();
		if (tek->getType() == STT_SECTION)
		{
			tek->outSym(output, i);
			i++;
		}
	}

	for (resetIterator(); getBoolIt(); iteratorNext())
	{
		tek = getItEntPointer();
		if (tek->getType() != STT_SECTION)
		{
			tek->outSym(output, i);
			i++;
		}
	}

}