#pragma once

//#include "Elf_Shdr.h"
#include <string>
#include "Table.h"
#include "Section.h"
#include "SymTab.h"
#include "RelTab.h"
#include "ByteSection.h"
#include "FRefTab.h"

using namespace std;

class SectionRedefinition {};

class SectionTab : public Table<Section*>
{
private:
	//void trySection(string fullnSecName, string dotSecton);
	ByteSection *current;

	SymTab *symTab;
	FRefTab *freftab;
	/*
	ByteSection	*textSec;
	ByteSection *dataSec;
	ByteSection	*bssSec;
	RelTab *textRelTab;
	RelTab *dataRelTab;
	RelTab *bssRelTab;*/
public:
	SectionTab() 
	{ 
		current = NULL; 
		symTab = new SymTab(); 
		freftab = new FRefTab();
		/*
		textSec = NULL;
		dataSec = NULL;
		bssSec = NULL;*/
	}
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
	{ symTab->addSym(sym, def, def ? current : NULL, type, binding, val); }
	void addToTextSec(dword insMem) { current->endInsert(insMem); }
	int addSymRel(string name, RT type, int off);
};