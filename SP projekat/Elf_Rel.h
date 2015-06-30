#pragma once

//#include "Elf_Sym.h"

enum RT	// Relocation Type
{
	RT_IMM32 = 32,
	RT_IMM16 = 16,
	RT_IMM18 = 18,
	RT_IMM21 = 21,
	RT_IMM21_PC,
	RT_IMM24 = 24,
	RT_IMM24_PC,
	RT_IMM5	 = 5
};

class Elf_Rel
{
private:
	int		offest;			// Beginning of instruction location offset
	RT		info;			// Relocation type
	void	*sym;			// Pointer to the symbol to be relocated
public:
	Elf_Rel() {}
	Elf_Rel(int off, RT inf, void *s) :offest(off), info(inf), sym(s) {}

	//string getName() { return name; }
};