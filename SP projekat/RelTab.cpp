#include "RelTab.h"

RelTab::~RelTab()
{
}

int RelTab::addRel(string name, RT type, int off)
{
	Elf_Sym *sym = symTab->getSymRef(name);

	if (sym != NULL)
	{
		Elf_Rel rel;

		if (sym->getDef() == false) sym->addFRef(off, type);

		if (sym->getBinding() == STB_GLOBAL)
		{
			rel = Elf_Rel(off, type, sym);
			endInsert(rel);
			return (-1)*INS_SIZE;
		}
		if (sym->getBinding() == STB_LOCAL)
		{
			rel = Elf_Rel(off, type, sym->getSection());
			endInsert(rel);
			return (-1)*INS_SIZE + sym->getValue();
		}
		// throw unknown binding?
	}
	else
	{
		sym = symTab->addUndSym(name, off);
		sym->addFRef(off, type);
		sym = NULL;
		return 0;
	}
}