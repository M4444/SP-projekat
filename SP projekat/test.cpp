#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "WordAnalyzer.h"

using namespace std;

int main(int argc, char *argv[])
{
	ifstream input;
	ofstream output;
	string line;
	string word;

	WordAnalyzer WA;
	try
	{
		WA = WordAnalyzer(argv[3]);
	}
	catch (BadArg)
	{
		cout << "*Invalid third argument*";
		return -1;
	}

	input.open(argv[1]);
	output.open(argv[2]);

	/*
	cout << (char)181;
	dword d = WA.creteMemRep("2,24",45, 1, 4);
	cout << d[0];
	//
	string str = "123456";
	int len, integ = stoi(str);
	try{
		string info;
		str = ".data123";
		//info.erase(info.begin() + 5-1, info.end() - 1);
		info = str.substr(0, 5);
		//str.erase(str.begin(), str.begin() + 5);
		str = str.substr(5);
		if (str!="") integ = stoi(str);
		str = "0123";
		integ = stoi(str);
		//str = "1234";
		integ = stoi(str);
		len = integ / pow(10, str.length()-1);
		//len > 0 - good
	}
	catch (invalid_argument ia)
	{
		integ = -1;
	}*/

	if (input.is_open())
	{
		bool fileEnd = false;
		bool firstLine = true;
		bool newLine;
		while (getline(input, line) && !fileEnd)
		{
			newLine = true;
			if (!firstLine) output << endl;
			else firstLine = false;

			istringstream inLine(line);
			bool firstWord = true;
			while (inLine >> word)
			{
				try 
				{
					WA.pass(word, newLine);
				}
				catch (BadSyntax)
				{
					cout << "*Syntax error*" << endl;
					return -2;
				}
				catch (OutOfRange)
				{
					cout << "*Number out of range*" << endl;
					return -3;
				}
				catch (SectionRedefinition)
				{
					cout << "*Section already defined*" << endl;
					return -4;
				}
				catch (BSSwrite)
				{
					cout << "*Can't write to BSS*" << endl;
					return -5;
				}
				catch (EndOfFile)
				{
					fileEnd = true;
					break;	// nece ispisati .end u izlazu
				}

				if (!firstWord) output << ' ';
				else firstWord = false;
				output << word;

				newLine = false;
			}
		}
		input.close();
		output.close();
	}
	else
	{
		cout << "*Unable to open input file*" << endl;
		return -1;
	}

	return 0;
}