#pragma once
#include "Table.h"
#include "Elf_Rel.h"

class FRef
{
private:
	int patch;
	RT type;
public:
	FRef(){}
	FRef(int p, RT t) : patch(p), type(t) {}
	~FRef(){}
};

class FRefTab : public Table<FRef>
{
public:
	FRefTab() {}
	FRefTab(int off, RT type) :Table(FRef(off, type)) {}
	~FRefTab() {}

	void addFRef(int off, RT type) { endInsert(FRef(off, type)); }
};

