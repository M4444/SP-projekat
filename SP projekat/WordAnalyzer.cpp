#include "WordAnalyzer.h"
#include <math.h>

WordAnalyzer::WordAnalyzer()
{
	secTab = new SectionTab();
	//sectionDefinition = false;
	//stringDefinition = false;
	expectNewLine = expectSameLine = false;
	skipNextWord = false;
	loCnt = 0;
	cdp = NONE;
//	symtab = new SymTab();
}


WordAnalyzer::~WordAnalyzer()
{
}

void WordAnalyzer::end()
{
	if (sectionDefinition || stringDefinition) throw BadSyntax();
}

void WordAnalyzer::setDirFlag(string sym)
{
	/* primary directive list
	.global <ime globalnog simbola>,...
	.extern <ime globalnog simbola>,...

	.text(1 | 2 |..) (int adr ucitavanja sekcije)
	.data(1 | 2 |..) (int adr ucitavanja sekcije)
	.bss(1 | 2 |..) (int adr ucitavanja sekcije)

	.end

	.char
	.word
	.long	(rezervise 4 bajta)
	.align
	.skip
	*/

	if (sym.compare(".global") == 0) skipNextWord = true; // nesto ali u drugom prolazu
	else throw BadSyntax();
}

int WordAnalyzer::checkNum(string numS)
{
	int numI;
	
	try{
		if (numS != "")
		{
			numI = stoi(numS);
			if ((numI / pow(10, numS.length() - 1)) == 0) return -1;
		}
		else numI = 0;
	}
	catch (invalid_argument ia)
	{
		return -2;
	}
	return numI;
}

bool WordAnalyzer::trySection(string fullnSecName, string dotSecton)
{
	string secNumS, secName;
	int secNumI;
	int l = dotSecton.length();

	secName = fullnSecName.substr(0, l);
	if (secName.compare(dotSecton) == 0)
	{
		secNumS = fullnSecName.substr(l);
		secNumI = checkNum(secNumS);
		if (secNumI < 0) throw BadSyntax();

		secTab->setCurrSecSize(loCnt);
		//loCnt = 0;

		if (secTab->sectionExists(fullnSecName)) throw SectionRedefinition();
		secTab->createSection(secName, fullnSecName);

		return true;
	}
	else return false;
}

bool WordAnalyzer::determineSec(string sym)
{
	if (trySection(sym, ".text")) return true;
	else if (trySection(sym, ".bss")) return true;
	else if (trySection(sym, ".data")) return true;

	return false;
}

bool WordAnalyzer::determineIns(string sym)
{
	if (sym.compare("add") == 0) return true;
	else if (sym.compare("sub") == 0) return true;
	else if (sym.compare("mul") == 0) return true;
	else if (sym.compare("div") == 0) return true;

	else if (sym.compare("cmp") == 0) return true;

	else if (sym.compare("and") == 0) return true;
	else if (sym.compare("or") == 0) return true;
	else if (sym.compare("not") == 0) return true;
	else if (sym.compare("test") == 0) return true;

	else if (sym.compare("ldr") == 0) return true;
	else if (sym.compare("str") == 0) return true;

	else if (sym.compare("je") == 0) return true;
	else if (sym.compare("jne") == 0) return true;
	else if (sym.compare("jge") == 0) return true;
	else if (sym.compare("jg") == 0) return true;
	else if (sym.compare("jle") == 0) return true;
	else if (sym.compare("jl") == 0) return true;
	else if (sym.compare("jo") == 0) return true;
	else if (sym.compare("jno") == 0) return true;
	else if (sym.compare("call") == 0) return true;

	else if (sym.compare("ret") == 0) return true;
	else if (sym.compare("iret") == 0) return true;
	else if (sym.compare("jmp") == 0) return true;

	else if (sym.compare("push") == 0) return true;
	else if (sym.compare("pop") == 0) return true;

	else if (sym.compare("mov") == 0) return true;

	else if (sym.compare("in") == 0) return true;
	else if (sym.compare("out") == 0) return true;

	else if (sym.compare("shr") == 0) return true;
	else if (sym.compare("shl") == 0) return true;

	else if (sym.compare("int") == 0) return true;

	else if (sym.compare("ldch") == 0) return true;
	else if (sym.compare("ldcl") == 0) return true;

	return false;
}

/*
void WordAnalyzer::defineSection(string word)
{
	if (word[0] == '\"' && word[word.length() - 1] == '\"')
	{
		word.erase(word.begin());
		word.erase(word.end() - 1);
		if (!determineSec(word)) throw BadSyntax();
	}
	else throw BadSyntax();
	sectionDefinition = false;
}

void WordAnalyzer::defineString(string word)
{
	if (word[0] == '\"' && word[word.length() - 1] == '\"')
	{
		loCnt += word.length() - 2;
	}
	else throw BadSyntax();
	stringDefinition = false;
}*/

void WordAnalyzer::process(string word, bool newLine)
{
	switch (cdp)
	{
	case NONE:
		if (word[0] == '.')		// da li je direktiva
		{
			if (word.compare(".global") == 0)
			{
				expectSameLine = true;
				cdp = GLOBAL;
			}
			else if (word.compare(".extern") == 0)
			{
				expectSameLine = true;
				cdp = EXTERN;
			}
			else if (word.compare(".long") == 0)
			{
				loCnt += 4;
				//expectSameLine = true;	xxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
				skipNextWord = true;
			}
			else if (word.compare(".skip") == 0)
			{
				expectSameLine = true;
				cdp = SKIP;
			}
			else if (word.compare(".end") == 0)
			{
				// mozda jos nesto
				throw EndOfFile();
			}
			else if (determineSec(word))
			{
				cdp = SECTION;
			}
		}
		else
		{
			if (word[word.length() - 1] == ':')		// da li je labela
			{
				word.erase(word.end() - 1);
				secTab->addToSymTab(word, true, STT_OBJECT/*?*/, STB_LOCAL, loCnt);
			}
			else if (!determineIns(word)) throw BadSyntax();	// da li je instrukcija
			else
			{
				loCnt += INS_SIZE;
				skipNextWord = true;	/*xxxxxxxxxxxxxxxxxxxxxxxxxxxxxx*/
			}
		}
		break;

	case GLOBAL: case EXTERN:
		if (word[word.length() - 1] == ',')		// da li postoji jos globalnih/externih simbola
		{
			word.erase(word.end() - 1);
			secTab->addToSymTab(word, false, STT_OBJECT, STB_GLOBAL);
			expectSameLine = true;
		}
		else
		{
			secTab->addToSymTab(word, false, STT_OBJECT, STB_GLOBAL);
			expectNewLine = true;
			cdp = NONE;
		}
		break;

	case SECTION:
		if (newLine) 
		{
			loCnt = 0;
			cdp = NONE;
			process(word, newLine);
		}
		else
		{
			int addr = checkNum(word);
			if (addr < 0) throw BadSyntax();
			secTab->setCurrSecAddr(addr);
			loCnt = addr;
			expectNewLine = 1;
			cdp = NONE;
		}
		break;

	case SKIP:
		int val = checkNum(word);
		if (val < 0) throw BadSyntax();
		loCnt += val;
		cdp = NONE;
		break;
	}
}

void WordAnalyzer::pass(string word, bool newLine)
{
	if (expectNewLine && !newLine) throw BadSyntax();
	if (expectSameLine && newLine) throw BadSyntax();
	expectNewLine = expectSameLine = 0;

	if (skipNextWord) { skipNextWord = false; }
//	else if (sectionDefinition) { defineSection(word); return 1; }
//	else if (stringDefinition) { defineString(word); return 1; }
	else
	{
		process(word, newLine);

		/*
		if (word[0] == '.')
		{
			if (!determineSec(word));
			else if (word.compare(".global") == 0)
			{
				expectSameLine = true;
			}
			//setDirFlag(word);
		}*/
		// nesto..
	}
}

// drugi prolaz 26:05