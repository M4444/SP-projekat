/*#include "Table.h"

template <typename T>
int Table<typename T>::length() const
{
	int n = 0;
	for (Elem* tek = first; tek; tek = tek->next)
		n++;
	return n;
}

template <typename T> 
void Table<typename T>::endInsert(T ent)  // Dodavanje na kraj.
{
	Elem* newE = new Elem(ent);
	if (!first) first = newE;
	else {
		Elem* cur = first;
		while (cur->next) cur = cur->next;
		cur->next = newE;
	}
}

template <typename T> 
void Table<typename T>::removeAll()
{
	while (first) {
		Elem* stari = first;
		first = first->next;
		delete stari;
	}
}


template <typename T> 
void Table::pisi() const {        // Pisanje liste.
for (Elem* tek = first; tek; tek = tek->next)
cout << tek->broj << ' ';
}

template <typename T> 
void Table::citaj1(int n) { // Citanje stavljajuci brojeve na pocetak.
removeAll();
for (int i = 0; i<n; i++) {
int b; cin >> b;
first = new Elem(b, first);
}
}

template <typename T> 
void Table::citaj2(int n) { // Citanje stavljajuci brojeve na kraj.
removeAll(); Elem* posl = 0;
for (int i = 0; i<n; i++) {
int b; cin >> b;
Elem* novi = new Elem(b);
if (!first) first = novi; else posl->next = novi;
posl = novi;
}
}

template <typename T> 
Table::Table(const Table& lst) {  // Konstruktor kopije.
first = 0;
for (Elem *tek = lst.first, *posl = 0; tek; tek = tek->next) {
Elem* novi = new Elem(tek->broj);
if (!first) first = novi; else posl->next = novi;
posl = novi;
}
}

template <typename T> 
void Table::umetni(int b) {       // Umetanje u uredenu listu.
Elem *tek = first, *pret = 0;
while (tek && tek->broj < b)
{
pret = tek; tek = tek->next;
}
Elem* novi = new Elem(b, tek);
if (!pret) first = novi;
else pret->next = novi;
}

template <typename T> 
void Table::izostavi(int b) {     // Izostavljanje svakog pojavljivanja.
Elem *tek = first, *pret = 0;
while (tek)
if (tek->broj != b) {
pret = tek; tek = tek->next;
}
else {
Elem* stari = tek;
tek = tek->next;
if (!pret) first = tek;
else pret->next = tek;
delete stari;
}
}*/
