#pragma once

template <typename T>
class Table
{
private:
	struct Elem {
		T entry;
		Elem* next;
		Elem(T ent, Elem* n = 0)
		{
			entry = ent;
			next = n;
		}
	};
	Elem *first;

	Elem *iterator;

protected:
	void resetIterator() 
	{
		iterator = first; 
	}
	T getItEnt() { return iterator->entry; }
	T *getItEntPointer() { return &(iterator->entry); }
	bool getBoolIt() { return iterator; }
	void iteratorNext() { iterator = iterator->next; }

	void replItEnt(T newEnt) { iterator->entry = newEnt; }
public:
	Table() { first = 0; iterator = 0/*(Elem *)4*/; }
	Table(T ent)
	{
		first = new Elem(ent);
	}
	~Table() { removeAll(); }
	int length() const
	{
		int n = 0;
		for (Elem* tek = first; tek; tek = tek->next)
			n++;
		return n;
	}
	void endInsert(T ent)      // Dodavanje na kraj.
	{
		Elem* newE = new Elem(ent);
		if (!first) first = newE;
		else {
			Elem* cur = first;
			while (cur->next) cur = cur->next;
			cur->next = newE;
		}
		iterator = newE;
	}
	void removeAll()
	{
		while (first) {
			Elem* stari = first;
			first = first->next;
			delete stari;
		}
	}

	void removeFirst()
	{
		if (first) {
			Elem* stari = first;
			first = first->next;
			delete stari;
		}
	}

	//Table(const Table& lst);     // - kopije.
	//void pisi() const;           // Pisanje liste.
	//void naPocetak(int b) {      // Dodavanje na pocetak.
	//	first = new Elem(b, first);
	//}
	//void citaj1(int n);      // Citanje liste stavljajuci brojeve na pocetak.
	//void citaj2(int n);      // Citanje liste stavljajuci brojeve na kraj.
	//void umetni(int b);      // Umetanje u uredenu listu.
	//void izostavi(int b);    // Izostavljanje svakog pojavljivanja.
};
