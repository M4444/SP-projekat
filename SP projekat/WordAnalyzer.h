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
	LONG,
	SKIP,
	SECTION,
	INSTR
};

class BadSyntax {};
class EndOfFile {};
class OutOfRange{};

class WordAnalyzer
{
private:
	SectionTab *secTab;
	SymTab *symtab;
	bool expectNewLine, expectSameLine;
	bool sectionDefinition, stringDefinition, skipNextWord;
	string section;
	string instr, instrOp[4];
	int instrOpNum;

	int loCnt;

	bool trySection(string sym, string dotSecton);
	int checkNum(string numS);
	dword creteMemRep(string type, int instr, int op0, int op1 = 0, int op2 = 0, int op3 = 0);
	dword creteMemRep(int instr);
	bool determineSec(string sym);
	bool determineIns(string sym);
	void creteInstEntry(string instr, string *instrOp, int instrOpNum);

	DP cdp;
	void process(string word, bool newLine);
public:
	WordAnalyzer();
	~WordAnalyzer();
	void end();

	void pass(string word, bool newLine);
};

