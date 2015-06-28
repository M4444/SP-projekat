#include "SymTab.h"

using namespace std;

void SymTab::add(string sym, Section *section, STT type, STB binding, int val)
{
	Elf_Sym old, s = Elf_Sym(sym, section, type, binding, val);
	// ~ add sym to simbol table
	// label = sym;

	// s.name = pomeraj u string tabli do imena simbola
	// s.value = pomeraj u trenutnoj sekciji
	// s.size = ?;	// "Sve instrukcije su velicine reci (4 bajta)."
	// s.type = STT_SECTION; ?
	// s.binding = STB_LOCAL; ?
	// s.reserved = 0;
	// s.section = "Indeks sekcije u tabeli zaglavlja sekcija za koji je posmatrani simbol vezan."

	for (resetIterator(); getBoolIt(); iteratorNext())
	{
		old = getItEnt();
		if (sym.compare(old.getName()) == 0)
		{
			s.copyBinding(old);
			replItEnt(s);
			return;
		}
	}
	endInsert(s);
}