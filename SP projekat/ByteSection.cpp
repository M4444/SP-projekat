#include "ByteSection.h"

ByteSection::~ByteSection()
{
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