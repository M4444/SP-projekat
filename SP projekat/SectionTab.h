#pragma once

//#include "Elf_Shdr.h"
#include <string>
#include <iostream>
#include <fstream>
#include "Table.h"
#include "Section.h"
#include "SymTab.h"
#include "RelTab.h"
#include "ByteSection.h"
#include "FRefTab.h"

using namespace std;

class SectionRedefinition {};
class BSSwrite {};

class SectionTab : public Table<Section*>
{
private:
	//void trySection(string fullnSecName, string dotSecton);
	ByteSection *current;

	SymTab *symTab;
	FRefTab *freftab;
public:
	SectionTab() 
	{ 
		current = NULL; 
		symTab = new SymTab(); 
		freftab = new FRefTab();
	}
	~SectionTab();


	bool sectionExists(string name);

	//bool setSection(string sec);
	//Section *createSec(string name);

	void createSection(string secTypeName, string secName);
	void createSection(string secTypeName) { createSection(secTypeName, secTypeName); }

	void setCurrSecAddr(int addr) 
	{
		if (current)
		{
			current->setAddress(addr);
			symTab->setSymVal(current->getName(), current->getAddress());
		}
	}
	void setCurrSecSize(int size) 
	{
		if (current)
		{
			current->setSize(size);
			symTab->setSymSize(current->getName(), current->getSize());
		}
	}

	bool checkBssCur()
	{
		string name = current->getName();
		name = name.substr(0, 4);
		if (name.compare(".bss") == 0) return true;
		else return false;
	}
	//SymTab *getSymTab() { return symTab; }

	void addToSymTab(string sym, bool def, STT type, STB binding, int val = 0) 
	{ symTab->addSym(sym, def, def ? current : NULL, type, binding, val); }
	void addMemToCurSec(dword insMem)
	{
		if (checkBssCur()) throw BSSwrite();
		else current->endInsert(insMem); 
	}
	int addSymRel(string name, RT type, int off);

	void backpatching();

	void outSymTab(ofstream *output)
	{
		//*output << "nesto, test" << endl;
		//ofstream &out = *output;
		//out << "jos nesto \\o//" << endl;
		symTab->outTab(output);
	}
	void outSecs(ofstream *output);
};