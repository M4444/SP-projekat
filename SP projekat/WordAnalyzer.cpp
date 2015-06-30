#include "WordAnalyzer.h"
#include <math.h>
//#include <cstdint>	// za int32_t

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

int WordAnalyzer::checkNum(string numS)
{
	int numI;
	
	try{
		if (numS != "")
		{
			numI = stoi(numS);
			if ((numI / pow(10, numS.length() - 1)) == 0) throw BadSyntax(); //return -1;
		}
		else numI = 0;
	}
	catch (invalid_argument ia)
	{
		throw BadSyntax();
		//return -2;
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
		//if (secNumI < 0) throw BadSyntax();

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

dword WordAnalyzer::creteMemRep(int op)
{
	dword Rep;
	Rep[3] = (op & 0xff000000) >> 24;

	Rep[2] = (op & 0xff0000) >> 16;

	Rep[1] = (op & 0xff00) >> 8;

	Rep[0] = op & 0xff;
	return Rep;
}
dword WordAnalyzer::creteMemRep(string type, int instr, int op0, int op1, int op2, int op3)
{
	dword Rep;

	if (type.compare("4,4,18") == 0)
	{
		Rep[3] = (instr & 0x3f) << 2;
		Rep[3] |= (op0 & 0xc) >> 2;

		Rep[2] = (op0 & 3) << 6;
		Rep[2] |= (op1 & 0xf) << 2;
		Rep[2] |= (op2 & 0x30000) >> 3;

		Rep[1] = (op2 & 0xff00) >> 8;

		Rep[0] = op2 & 0xff;
	}
	else if (type.compare("5,21") == 0)
	{
		Rep[3] = (instr & 0x3f) << 2;
		Rep[3] |= (op0 & 0x18) >> 3;

		Rep[2] = (op0 & 7) << 5;
		Rep[2] |= (op1 & 0x1f0000) >> 16;

		Rep[1] = (op1 & 0xff00) >> 8;

		Rep[0] = op1 & 0xff;
	}
	else if (type.compare("2,24") == 0)
	{
		Rep[3] = (instr & 0x3f) << 2;
		Rep[3] |= op0 & 0x3;

		Rep[2] = (op1 & 0xff0000) >> 16;

		Rep[1] = (op1 & 0xff00) >> 8;

		Rep[0] = op1 & 0xff;
	}
	else if (type.compare("4,4,5,13") == 0)
	{
		Rep[3] = (instr & 0x3f) << 2;
		Rep[3] |= (op0 & 0xc) >> 2;

		Rep[2] = (op0 & 3) << 6;
		Rep[2] |= (op1 & 0xf) << 2;
		Rep[2] |= (op2 & 0x18) >> 3;

		Rep[1] = (op2 & 7) << 5;
		Rep[1] |= (op3 & 0x1F00) >> 8;

		Rep[0] = op3 & 0xff;
	}
	else if (type.compare("4,22") == 0)
	{
		Rep[3] = (instr & 0x3f) << 2;
		Rep[3] |= (op0 & 0xc) >> 2;

		Rep[2] = (op0 & 3) << 6;
		Rep[2] |= (op1 & 0x3f0000) >> 16;

		Rep[1] = (op1 & 0xff00) >> 8;

		Rep[0] = op1 & 0xff;
	}
	else if (type.compare("4,1,16,5") == 0)
	{
		Rep[3] = (instr & 0x3f) << 2;
		Rep[3] |= (op0 & 0xc) >> 2;

		Rep[2] = (op0 & 3) << 6;
		Rep[2] |= (op1 & 1) << 5;
		Rep[2] |= (op2 & 0xf800) >> 11;

		Rep[1] = (op2 & 0x7f8) >> 3;

		Rep[0] = (op2 & 7) << 5;
		Rep[0] |= op3 & 0x1f;
	}
	return Rep;
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

	else if (sym.compare("ldc") == 0) return true;
	else if (sym.compare("ldch") == 0) return true;
	else if (sym.compare("ldcl") == 0) return true;

	return false;
}

void WordAnalyzer::creteInstEntry(string instr, string *instrOp, int instrOpNum)
{
	if (instr.compare("add") == 0 || instr.compare("sub") == 0 || instr.compare("mul") == 0
		|| instr.compare("div") == 0 || instr.compare("cmp") == 0
		|| instr.compare("ldr") == 0 || instr.compare("str") == 0)
	{
		if (instrOpNum != 3) throw BadSyntax();

		int immI,
			dstI = checkNum(instrOp[0]),
			srcI = checkNum(instrOp[1]);
		string immS = instrOp[2];

		if (dstI<0 || dstI >15) throw OutOfRange();
		if (srcI<0 || srcI >15) throw OutOfRange();

		if (immS[0] == '-')		// da li je negativni broj
		{
			immS.erase(immS.begin());
			immI = checkNum(immS);
			if (immI > pow(2, 18 - 1)) throw OutOfRange();
			if (immI == 0) throw BadSyntax();
			immI *= -1;
		}
		else
		{
			if (immS[0] >= '1' && immS[0] <= '9')		// da li je broj
			{
				immI = checkNum(immS);
				if (immI > pow(2, 18 - 1) - 1) throw OutOfRange();
			}
			else
			{
				immI = secTab->addSymRel(immS, RT_IMM18, loCnt);
			}
		}
		dword insMem;
		if (instr.compare("add") == 0) insMem = creteMemRep("4,4,18", 0, dstI, srcI, immI);
		if (instr.compare("sub") == 0) insMem = creteMemRep("4,4,18", 1, dstI, srcI, immI);
		if (instr.compare("mul") == 0) insMem = creteMemRep("4,4,18", 2, dstI, srcI, immI);
		if (instr.compare("div") == 0) insMem = creteMemRep("4,4,18", 3, dstI, srcI, immI);
		if (instr.compare("cmp") == 0) insMem = creteMemRep("4,4,18", 4, dstI, srcI, immI);

		if (instr.compare("ldr") == 0) insMem = creteMemRep("4,4,18", 9, dstI, srcI, immI);
		if (instr.compare("str") == 0) insMem = creteMemRep("4,4,18", 10, dstI, srcI, immI);
		secTab->addToTextSec(insMem);
	}
	else if (instr.compare("and") == 0 || instr.compare("or") == 0 || instr.compare("not") == 0
		|| instr.compare("test") == 0
		|| instr.compare("mov") == 0 || instr.compare("in") == 0 || instr.compare("out") == 0)
	{
		if (instrOpNum != 2) throw BadSyntax();

		int srcI = checkNum(instrOp[0]),
			dstI = checkNum(instrOp[1]);

		if (dstI<0 || dstI >15) throw OutOfRange();
		if (srcI<0 || srcI >15) throw OutOfRange();
		dword insMem;
		if (instr.compare("and") == 0) insMem = creteMemRep("4,4,18", 5, dstI, srcI);
		if (instr.compare("or") == 0) insMem = creteMemRep("4,4,18", 6, dstI, srcI);
		if (instr.compare("not") == 0) insMem = creteMemRep("4,4,18", 7, dstI, srcI);
		if (instr.compare("test") == 0) insMem = creteMemRep("4,4,18", 8, dstI, srcI);

		if (instr.compare("mov") == 0) insMem = creteMemRep("4,4,18", 25, dstI, srcI);
		if (instr.compare("in") == 0) insMem = creteMemRep("4,4,18", 26, dstI, srcI);
		if (instr.compare("out") == 0) insMem = creteMemRep("4,4,18", 27, dstI, srcI);
		secTab->addToTextSec(insMem);
	}
	else if (instr.compare("je") == 0 || instr.compare("jne") == 0 || instr.compare("jge") == 0
		|| instr.compare("jg") == 0 || instr.compare("jle") == 0 || instr.compare("jl") == 0
		|| instr.compare("jo") == 0 || instr.compare("jno") == 0 || instr.compare("call") == 0)
	{
		if (instrOpNum != 2) throw BadSyntax();

		int immI, 
			dstI = checkNum(instrOp[0]);
		string immS = instrOp[1];

		if (dstI<0 || dstI >16) throw OutOfRange();

		if (immS[0] == '-')
		{
			immS.erase(immS.begin());
			immI = checkNum(immS);
			if (immI > pow(2, 21 - 1)) throw OutOfRange();
			if (immI == 0) throw BadSyntax();
			immI *= -1;
		}
		else
		{
			if (immS[0] >= '1' && immS[0] <= '9')
			{
				immI = checkNum(immS);
				if (immI > pow(2, 21 - 1) - 1) throw OutOfRange();
			}
			else
			{
				immI = secTab->addSymRel(immS, dstI == 16 ? RT_IMM21_PC : RT_IMM21, loCnt);
			}
		}
		dword insMem;
		if (instr.compare("je") == 0) insMem = creteMemRep("5,21", 11, dstI, immI);
		if (instr.compare("jne") == 0) insMem = creteMemRep("5,21", 12, dstI, immI);
		if (instr.compare("jge") == 0) insMem = creteMemRep("5,21", 13, dstI, immI);
		if (instr.compare("jg") == 0) insMem = creteMemRep("5,21", 14, dstI, immI);
		if (instr.compare("jle") == 0) insMem = creteMemRep("5,21", 15, dstI, immI);
		if (instr.compare("jl") == 0) insMem = creteMemRep("5,21", 16, dstI, immI);
		if (instr.compare("jo") == 0) insMem = creteMemRep("5,21", 19, dstI, immI);
		if (instr.compare("jno") == 0) insMem = creteMemRep("5,21", 20, dstI, immI);
		if (instr.compare("call") == 0) insMem = creteMemRep("5,21", 21, dstI, immI);
		secTab->addToTextSec(insMem);
	}
	else if (instr.compare("ret") == 0 || instr.compare("iret") == 0 || instr.compare("jmp") == 0)
	{
		if (instrOpNum != 2) throw BadSyntax();

		int immI,
			type = checkNum(instrOp[0]);
		string immS = instrOp[1];

		if (type<0 || type >3) throw OutOfRange();

		if (immS[0] == '-')
		{
			immS.erase(immS.begin());
			immI = checkNum(immS);
			if (immI > pow(2, 24 - 1)) throw OutOfRange();
			if (immI == 0) throw BadSyntax();
			immI *= -1;
		}
		else
		{
			if (immS[0] >= '1' && immS[0] <= '9')
			{
				immI = checkNum(immS);
				if (immI > pow(2, 24 - 1) - 1) throw OutOfRange();
			}
			else
			{
				immI = secTab->addSymRel(immS, RT_IMM24, loCnt);
			}
		}
		dword insMem = creteMemRep("2,24", 22, type, immI);
		secTab->addToTextSec(insMem);
	}
	else if (instr.compare("push") == 0 || instr.compare("pop") == 0)
	{
		if (instrOpNum != 1) throw BadSyntax();

		int reg = checkNum(instrOp[0]);

		if (reg<0 || reg >17) throw OutOfRange();
		dword insMem;
		if (instr.compare("push") == 0) insMem = creteMemRep("5,21", 23, reg);
		if (instr.compare("pop") == 0) insMem = creteMemRep("5,21", 24, reg);
		secTab->addToTextSec(insMem);
	}
	else if (instr.compare("shr") == 0 || instr.compare("shl") == 0)
	{
		if (instrOpNum != 3) throw BadSyntax();

		int immI, 
			dstI = checkNum(instrOp[0]), 
			srcI = checkNum(instrOp[1]);
		string immS = instrOp[2];

		if (dstI<0 || dstI >15) throw OutOfRange();
		if (srcI<0 || srcI >15) throw OutOfRange();

		if (immS[0] == '-') throw BadSyntax();
		else
		{
			if (immS[0] >= '1' && immS[0] <= '9')
			{
				immI = checkNum(immS);
				if (immI > pow(2, 5) - 1) throw OutOfRange();
			}
			else
			{
				immI = secTab->addSymRel(immS, RT_IMM5, loCnt);
			}
		}
		dword insMem;
		if (instr.compare("shr") == 0) insMem = creteMemRep("4,4,5,13", 28, dstI, srcI, immI);
		if (instr.compare("shl") == 0) insMem = creteMemRep("4,4,5,13", 29, dstI, srcI, immI);
		secTab->addToTextSec(insMem);
	}
	else if (instr.compare("int") == 0)
	{
		if (instrOpNum != 1) throw BadSyntax();

		int src = checkNum(instrOp[0]);

		if (src<0 || src >15) throw OutOfRange();
		dword insMem = creteMemRep("4,22", 30, src);
		secTab->addToTextSec(insMem);
	}
	else if (instr.compare("ldch") == 0 || instr.compare("ldcl") == 0)
	{
		if (instrOpNum != 3) throw BadSyntax();

		int immI,
			dstI = checkNum(instrOp[0]),
			hl = checkNum(instrOp[1]);
		string immS = instrOp[2];

		if (dstI<0 || dstI >15) throw OutOfRange();
		if (hl!=0 && hl!=1) throw OutOfRange();

		if (immS[0] == '-')	throw BadSyntax();
		else
		{
			if (immS[0] >= '1' && immS[0] <= '9')
			{
				immI = checkNum(immS);
				if (immI > pow(2, 16) - 1) throw OutOfRange();
			}
			else
			{
				immI = secTab->addSymRel(immS, RT_IMM16, loCnt);
			}
		}
		dword insMem = creteMemRep("4,1,16,5", 31, dstI, hl, immI);
		secTab->addToTextSec(insMem);
	}
	else if (instr.compare("ldcl1") == 0 || instr.compare("ldch2") == 0)
	{
		if (instrOpNum != 3) throw BadSyntax();

		int immI,
			dstI = checkNum(instrOp[0]),
			hl = checkNum(instrOp[1]);
		string immS = instrOp[2];

		if (dstI<0 || dstI >15) throw OutOfRange();
		if (hl != 0 && hl != 1) throw OutOfRange();

		if (immS[0] == '-')	throw BadSyntax();
		else
		{
			if (immS[0] >= '1' && immS[0] <= '9')
			{
				immI = checkNum(immS);
				if (immI > pow(2, 32) - 1) throw OutOfRange();
			}
			else
			{
				immI = secTab->addSymRel(immS, RT_IMM16, loCnt);
			}
		}
		if (instr.compare("ldcl1") == 0) immI &= 0xffff;
		if (instr.compare("ldch2") == 0) immI = (immI & 0xffff0000) >> 16;
		dword insMem = creteMemRep("4,1,16,5", 31, dstI, hl, immI);
		secTab->addToTextSec(insMem);
	}
}

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
				expectSameLine = true;
				cdp = LONG;
			}
			else if (word.compare(".skip") == 0)
			{
				expectSameLine = true;
				cdp = SKIP;
			}
			else if (word.compare(".end") == 0)
			{
				// jos nesto za kraj (FRefTab)
				// ako se skace u okvuru iste sekcije izbrisati zapis o relokaciji na kraju
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
					instr = word;
					instrOpNum = 0;
					cdp = INSTR;
					expectSameLine = true;
				}
		}
		break;

	case INSTR:
		if (newLine)
		{
			if (instr.compare("ldc") == 0)
			{
				creteInstEntry("ldcl1", instrOp, instrOpNum);
				loCnt += INS_SIZE;
				creteInstEntry("ldch2", instrOp, instrOpNum);
				loCnt += INS_SIZE;
			}
			else
			{
				creteInstEntry(instr, instrOp, instrOpNum);
				loCnt += INS_SIZE;
			}
			cdp = NONE;
			process(word, newLine);
		}
		else
		{
			instrOp[instrOpNum++] = word;
			if (instrOpNum > 2) expectNewLine = true;
		}
		break;

	case GLOBAL: case EXTERN:
		if (word[word.length() - 1] == ',')		// da li postoji jos globalnih/externih simbola
		{
			word.erase(word.end() - 1);
			expectSameLine = true;
		}
		else
		{
			expectNewLine = true;
			cdp = NONE;
		}
		secTab->addToSymTab(word, false, STT_OBJECT, STB_GLOBAL);	// *********dodati proveru za eksterne simbole
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
			secTab->setCurrSecAddr(addr);
			loCnt = addr;
			expectNewLine = 1;
			cdp = NONE;
		}
		break;

	case SKIP:
		int val = checkNum(word);
		loCnt += val;
		cdp = NONE;
		break;
		
	case LONG:	
		int longVal=0;
		if (word[0] == '-')
		{
			word.erase(word.begin());
			longVal = checkNum(word);
			if (longVal > pow(2, 8 - 1)) throw OutOfRange();
			if (longVal == 0) throw BadSyntax();
			longVal *= -1;
		}
		else
		{
			if (word[0] >= '1' && word[0] <= '9')
			{
				longVal = checkNum(word);
				if (longVal > pow(2, 8 - 1) - 1) throw OutOfRange();
			}
			else
			{
				longVal = secTab->addSymRel(word, RT_IMM32, loCnt);
			}
		}
		dword insMem = creteMemRep(longVal);
		secTab->addToTextSec(insMem);

		expectNewLine = true;
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
	else
	{
		process(word, newLine);
	}
}

// drugi prolaz 26:05
// krajnji deo 22:14