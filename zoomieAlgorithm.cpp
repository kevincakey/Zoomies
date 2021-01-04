#include "zoomieAlgorithm.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <ctime>

using namespace std;

void zoomieAlgorithm::writeIn()
{
	ifstream fin("zoomies.csv");
	string line;
	int linecounter = 0;

	getline(fin, names);
	while (fin.good())
	{
		Person row;
		getline(fin, line);
		int n = line.find(",");
		row.name = line.substr(0, n);
		string list = line.substr(n + 1, line.size());
		row.met = StoVcT(list);
		people.push_back(row);
	}
	fin.close();
}

vector<cellType> zoomieAlgorithm::StoVcT(string s)
{
	vector<cellType> line;

	for (unsigned i = 0; i < s.size();i++)
	{
		if ((s.substr(i, 2) == "x,") || (s.substr(i, 2) == "o,"))
		{
			line.push_back(met);
			i++;
		}
		else if (s.at(i) == ',')
		{
			line.push_back(unmet);
		}
	}

	return line;
}

void zoomieAlgorithm::writeOut()
{
	ofstream fout("zoomies.csv");

	if (!fout.is_open())
	{
		cout << "zoomies.csv did not open" << endl;
		exit(1);
	}

	string p1 = PtoS(people.at(0));

	fout << names << endl;
	fout << people.at(0).name << "," << p1;
	for (unsigned i = 1; i < people.size(); i++)
	{
		fout << endl << people.at(i).name << "," << PtoS(people.at(i));
		//cout << endl << people.at(i).name << "," << PtoS(people.at(i));
	}
	fout.close();
}

string zoomieAlgorithm::PtoS(Person p) // parameter was vector<bool> b
{
	string s = "";
	for (unsigned i = 0; i < p.met.size(); i++)
	{
		if (p.met.at(i) == recent)
		{
			s += "o,";
		}
		else
			if (p.met.at(i) == met)
			{
				s += "x,";
			}
			else if (p.met.at(i) == unmet)
			{
				s += ",";
			}
			else
			{
				return "error";
			}
	}
	return s;
}

void zoomieAlgorithm::savePrevious()
{
	ofstream fout("prezoomies.csv");

	if (!fout.is_open())
	{
		cout << "prezoomies.csv did not open" << endl;
		exit(1);
	}

	string p1 = PtoS(people.at(0));

	fout << names << endl;
	fout << people.at(0).name << "," << p1;
	for (unsigned i = 1; i < people.size(); i++)
	{
		fout << endl << people.at(i).name << "," << PtoS(people.at(i));
		//cout << endl << people.at(i).name << "," << PtoS(people.at(i));
	}
	fout.close();
}

void zoomieAlgorithm::menu()
{
	string np;
	string in = "";
	while (in != "q")
	{
		cout << "What would you like to do?" << endl;
		cout << "a: To Add a Person" << endl;
		cout << "n: To remove someone from participating this week (in development, may cause errors)" << endl;
		cout << "p: To start pairing" << endl;
		cout << "r: To reset pairs" << endl;
		cout << "q: To quit" << endl;
		cin >> in;
		if (in == "a")
		{
			addPeople();
			cout << endl;
			writeOut();
		}
		else if (in == "n")
		{
			cout << "Enter the name of the person not participating this week" << endl;
			cin >> np;
			notParticipating(np);
			cout << np << " is not participating anymore" << endl;
			cout << endl;
		}
		else if (in == "p")
		{
			Pair();
			printPairs();

			cout << "Pairing complete" << endl;
			cout << endl;
			in = "q";
		}
		else if (in == "r")
		{
			reset();
			cout << "Reset complete" << endl;
			cout << endl;
			in = "q";
		}
		else if (in == "q")
		{
			cout << "Open the Excel File to see results" << endl;
		}
		else
		{
			cout << "Please choose one of the valid options above" << endl;
			cout << endl;
		}
	}
	return;
}

void zoomieAlgorithm::addPeople()
{
	string n;
	cout << "Enter the name of the person to add in" << endl;
	cin >> n;
	names.insert((names.find("border")), n + ',');

	for (unsigned i = 0; i < people.size() - 1; i++)
	{
		people.at(i).met.push_back(unmet);
	}

	Person p;
	p.name = n;

	for (unsigned j = 0; j < people.size() - 2;j++)
	{
		p.met.push_back(unmet);
	}
	p.met.push_back(met);

	people.insert(people.begin() + people.size() - 2, p);

	cout << n << " was added for pairing!" << endl;
}

void zoomieAlgorithm::printPeople()
{
	cout << "names: " << names << endl;

	for (unsigned i = 0; i < people.size(); i++)
	{
		cout << people.at(i).name << "," << PtoS(people.at(i)) << endl;
	}
	cout << endl;

	for (unsigned j = 0; j < people.size() - 2; j++)
	{
		for (unsigned k = 0; k < people.at(j).met.size();k++)
		{
			cout << people.at(j).met.at(k) << " ";
		}
		cout << endl;
	}
}

int zoomieAlgorithm::getBorderIndex()
{
	return names.find("border") + 6;
}

void zoomieAlgorithm::notParticipating(string p)
{
	expectedEmpty = 0;
	people.at(findPersonIndex(p)).participating = false;
	for (unsigned i = 0;i < findPerson(p).met.size();i++)
	{
		if (!findPerson(p).met.at(i))
		{
			expectedEmpty++;
		}
	}
}

void zoomieAlgorithm::Pair()
{
	int bufferIndex = people.size() - 3;
	if (((people.size() - 2 - numNotParticipating()) % 2) == 1)
	{
		people.at(bufferIndex).participating = false;
	}
	int p1, p2;
	refreshAllMet();
	srand(time(NULL));
	while (PossiblePair())
	{
		p1 = rand() % (people.size() - 2);
		p2 = rand() % (people.size() - 2);

		if (notPaired() <= 1)
		{
			//cout << "Everyone has been paired" << endl;
			return;
		}
		//redo random pairs
		while (p1 == p2 || (!people.at(p1).participating || !people.at(p2).participating)
			|| (people.at(p1).paired || people.at(p2).paired) || (people.at(p1).met.at(p2))
			|| (people.at(p2).met.at(p1)))
		{
			if (!PossiblePair())
			{
				return;
			}

			if (notPaired() <= 1)
			{
				cout << "Everyone has been paired or is not participating" << endl;
				return;
			}

			p1 = rand() % (people.size() - 2);
			p2 = rand() % (people.size() - 2);

			if ((p1 != p2) && unpairedAllMet() && notPaired() != 0)
			{
				resetPaired();

				p1 = rand() % (people.size() - 2);
				p2 = rand() % (people.size() - 2);

				vPrintPairs.clear();
				resetRecent();
			}
		}

		people.at(p1).paired = true;
		people.at(p1).met.at(p2) = recent;
		people.at(p2).paired = true;
		people.at(p2).met.at(p1) = recent;

		vPrintPairs.push_back("Pair: (" + people.at(p1).name + ", " + people.at(p2).name + ")");
	}
}

int zoomieAlgorithm::numNotParticipating()
{
	int p = 0;
	for (unsigned i = 0; i < people.size() - 2;i++)
	{
		if (people.at(i).participating == false)
		{
			p++;
		}
	}
	return p;
}

void zoomieAlgorithm::resetPaired()
{
	for (unsigned i = 0; i < people.size() - 2;i++)
	{
		if (people.at(i).participating)
		{
			people.at(i).paired = false;
		}
	}
}

bool zoomieAlgorithm::PossiblePair()
{
	int y = 0;
	for (unsigned i = 0; i < people.size() - 2;i++)
	{
		for (unsigned j = 0;j < people.at(i).met.size();j++)
		{
			if (people.at(i).met.at(j) == unmet)
			{
				y++;
			}
		}
	}

	y -= expectedEmpty;

	if (y >= 2)
	{
		return true;
	}
	else
	{
		return false;
	}
}

int zoomieAlgorithm::peopleNotMet(Person p)
{
	int n = 0;
	for (unsigned i = 0; i < p.met.size() - 1;i++)
	{
		if (p.met.at(i))
		{
			n++;
		}
	}
	return p.met.size() - 1 - n;
}

void zoomieAlgorithm::printPairs()
{
	for (unsigned i = 0; i < vPrintPairs.size(); i++)
	{
		cout << vPrintPairs.at(i) << endl;
	}
}

int zoomieAlgorithm::notPaired()
{
	int p = 0;
	for (unsigned i = 0; i < people.size() - 2;i++)
	{
		if (people.at(i).paired == false && people.at(i).participating)
		{
			p++;
		}
	}
	return p;
}

void zoomieAlgorithm::printNotPaired()
{
	int p = 0;
	for (unsigned i = 0; i < people.size() - 2;i++)
	{
		if (people.at(i).paired == false && people.at(i).participating)
		{
			cout << people.at(i).name << ", ";
		}
	}
	cout << endl;
}

void zoomieAlgorithm::reset()
{
	for (unsigned i = 0; i < people.size();i++)
	{
		for (unsigned j = 0; j < people.at(i).met.size(); j++)
		{
			if (i == j)
			{
				people.at(i).met.at(j) = met;
			}
			else
			{
				people.at(i).met.at(j) = unmet;
			}
		}
	}
}

Person zoomieAlgorithm::findPerson(string s)
{
	for (unsigned i = 0; i < people.size() - 1;i++)
	{
		if (people.at(i).name == s)
		{
			return people.at(i);
		}
	}
	cout << "No match found" << endl;
	exit(1);
}

int zoomieAlgorithm::findPersonIndex(string s)
{
	for (unsigned i = 0; i < people.size() - 2;i++)
	{
		if (people.at(i).name == s)
		{
			return i;
		}
	}
	cout << "No match found" << endl;
	exit(1);
}

void zoomieAlgorithm::refreshAllMet()
{
	for (unsigned i = 0; i < people.size() - 2;i++)
	{
		if (peopleNotMet(people.at(i)) == 0)
		{
			for (unsigned j = 0; j < people.at(i).met.size();j++)
			{
				people.at(i).met.at(j) = unmet;
			}
			people.at(i).met.at(findPersonIndex(people.at(i).name)) = met;
		}
	}
}

bool zoomieAlgorithm::unpairedAllMet()
{
	vector<Person> unpaired;
	for (unsigned i = 0; i < people.size() - 2; i++)
	{
		if (!people.at(i).paired)
		{
			unpaired.push_back(people.at(i));
		}
	}


	for (unsigned j = 0; j < unpaired.size();j++)
	{
		for (unsigned k = 0; k < unpaired.size();k++)
		{
			if ((!(unpaired.at(j).met.at(findPersonIndex(unpaired.at(k).name)))) && (k != j))
			{
				//cout << "there is a possible unpaired pair" << endl;
				return false;
			}
		}
	}

	//cout << "no possible unpaired pair" << endl;
	return true;
}

void zoomieAlgorithm::resetRecent()
{
	for (unsigned i = 0; i < people.size() - 2;i++)
	{
		for (unsigned j = 0; j < people.at(i).met.size(); j++)
		{
			if (people.at(i).met.at(j) == recent)
			{
				people.at(i).met.at(j) = unmet;
			}
		}
	}
}