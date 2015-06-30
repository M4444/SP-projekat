#include "SectionTab.h"
#include "SymTab.h"
#include "ByteSection.h"

//SectionTab::SectionTab()
//{
//}


SectionTab::~SectionTab()
{
}

bool SectionTab::sectionExists(string name)
{
	Section *sec;

	for (resetIterator(); getBoolIt(); iteratorNext())
	{
		sec = getItEnt();
		if (name.compare(sec->getName()) == 0) return true;
	}
	return false;
}

void SectionTab::createSection(string secTypeName, string name)
{
	//Section sec = Section();

	if (secTypeName.compare(".text") == 0)
	{
		current = new ByteSection(name, SHT_PROGBITS, SHF_ALLOC | SHF_EXECINSTR);
	}
	else if (secTypeName.compare(".bss") == 0)
	{
		current = new ByteSection(name, SHT_NOBITS, SHF_ALLOC | SHF_WRITE);
	}
	else if (secTypeName.compare(".data") == 0)
	{
		current = new ByteSection(name, SHT_PROGBITS, SHF_ALLOC | SHF_WRITE);
	}
	RelTab *relTab = new RelTab(symTab, current);

	endInsert(current);
	endInsert(relTab);
}

int SectionTab::addSymRel(string name, RT type, int off)
{
	Section *sec;
	for (resetIterator(); getBoolIt(); iteratorNext())
	{
		sec = getItEnt();
		if ((sec->getName()).compare(".rel") == 0)
		{
			RelTab *relTab = (RelTab*) sec;
			if (relTab->getBSPointer() == current)
			{
				return relTab->addRel(name, type, off);
			}
		}
	}
}

/*
void SectionTab::trySection(string fullnSecName, string dotSecton)
{
	string secNumS, secName;
	int secNumI;
	int l = dotSecton.length();

	secName = fullnSecName.substr(0, l);
	if (secName.compare(dotSecton) == 0)
	{
		secNumS = fullnSecName.substr(l);
		if (secNumS != "") secNumI = stoi(secNumS);
		else secNumI = 0;
		if (sectionExists(secName, secNumI)) throw SectionRedefinition();
		// secTab->defineSection(dotSecton, secNumI)..
	}
}

bool SectionTab::setSection(string sec)
{
	trySection(sec, ".bss");

	string secNumS, secName;
	int secNumI;
	
	int l = string(".bss").length();

	secName = sec.substr(0, 4);
	if (secName.compare(".bss") == 0)
	{
		secNumS = sec.substr(4);
		if (secNumS != "") secNumI = stoi(secNumS);
		else secNumI = 0;
		if (sectionExists(secName, secNumI)) throw SectionRedefinition();
		// define section (.bss, secNumI)..
	}

	secName = sec.substr(0, 5);
	if (secName.compare(".text") == 0)
	{
		secNumS = sec.substr(5);
		if (secNumS != "") secNumI = stoi(secNumS);
		else secNumI = 0;
		if (sectionExists(secName, secNumI)) throw SectionRedefinition();
		// define section (.bss, secNumI)..
	}

}*/