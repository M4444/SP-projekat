#pragma once

#include <string>
#include "SectionTab.h"
#include "SymTab.h"

using namespace std;

enum DP		// Directive Process
{
	NONE = 0,
	GLOBAL,
	EXTERN,
	SECTION,
	SKIP
};

class BadSyntax {};
class EndOfFile {};

class WordAnalyzer
{
private:
	SectionTab *secTab;
	SymTab *symtab;
	bool expectNewLine, expectSameLine;
	bool sectionDefinition, stringDefinition, skipNextWord;
	string section;

	int loCnt;

	bool trySection(string sym, string dotSecton);
	int checkNum(string numS);

	void setDirFlag(string sym);
	bool determineSec(string sym);
	bool determineIns(string sym);
	void defineSection(string word);
	void defineString(string word);

	DP cdp;
	void process(string word, bool newLine);
public:
	WordAnalyzer();
	~WordAnalyzer();
	void end();

	void pass(string word, bool newLine);

};

