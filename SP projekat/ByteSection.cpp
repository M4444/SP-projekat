#include "ByteSection.h"

ByteSection::~ByteSection()
{
}

void ByteSection::insert(char c)
{
	dw[cnt++] = c;
	if (cnt == 4)
	{
		endInsert(dw);
		cnt = 0;
	}
}

void ByteSection::endInsert(dword dw)
{
	for (int i = 4; i >= 0; i--)
	{
		insert(dw[i]);
	}
}

void ByteSection::replace(dword dw, int offset)
{
	dword *tek;

	resetIterator();
	for (int i = 0; i < offset / INS_SIZE; i++)
		iteratorNext();

	tek = getItEntPointer();

	tek->sumOr(dw);
}

void ByteSection::outTab(ofstream *output)
{
	ofstream &out = *output;

	for (int i = cnt; i < 4; i++)
	{
		dw[i] = 0;
	}
	endInsert(dw);
	cnt = 0;

	out << "#" << this->getName() << endl;

	dword *tek;
	int i=0;
	for (resetIterator(); getBoolIt(); iteratorNext())
	{
		tek = getItEntPointer();

		tek->outDword(output);
		out << "  ";
		if ((i % 4) == 3) out << endl;
		i++;
	}
}