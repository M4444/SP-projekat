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
			if (sym->getSection() == NULL) rel = Elf_Rel(off, type, NULL);	// dodati fref na mesto
			else
			{
				string sec = sym->getSection()->getName();
				rel = Elf_Rel(off, type, symTab->getSymRef(sec));
			}
			//rel = Elf_Rel(off, type, );
			endInsert(rel);
			return (-1)*INS_SIZE + sym->getValue();
		}
		// throw unknown binding?
	}
	else
	{
		sym = symTab->addUndSym(name, off);
		sym->addFRef(off, type);
		return 0;
	}
}

void RelTab::outTab(ofstream *output)
{
	ofstream &out = *output;

	string name = bTab->getName();
	name = name.substr(0, 4);
	if (name.compare(".bss") == 0) return;

	out << "#.rel" << bTab->getName() << endl;
	out << "offset\t\tType\t\t\Val[" << bTab->getName() << "]:" << endl;

	Elf_Rel *tek;
	for (resetIterator(); getBoolIt(); iteratorNext())
	{
		tek = getItEntPointer();

		tek->outRel(output);
	}
}