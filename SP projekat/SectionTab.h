#pragma once

//#include "Elf_Shdr.h"
#include <string>
#include "Table.h"
#include "Section.h"
#include "SymTab.h"

using namespace std;

class SectionRedefinition {};

class SectionTab : public Table<Section*>
{
private:
	//void trySection(string fullnSecName, string dotSecton);
	Section *current;

	SymTab *symTab;
public:
	SectionTab() { symTab = new SymTab(); current = NULL; }
	~SectionTab();


	bool sectionExists(string name);

	//bool setSection(string sec);

	//Section *createSec(string name);


	void createSection(string secTypeName, string secName);
	void createSection(string secTypeName) { createSection(secTypeName, secTypeName); }

	void setCurrSecAddr(int addr) { if (current) current->setAddress(addr); }
	void setCurrSecSize(int size) { if (current) current->setSize(size); }

	//SymTab *getSymTab() { return symTab; }

	void addToSymTab(string sym, bool def, STT type, STB binding, int val = 0) 
	{ symTab->add(sym, def ? current : 0, type, binding, val); }
};