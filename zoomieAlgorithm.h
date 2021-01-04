#ifndef ZOOMIE_ALGORITHM_H 
#define ZOOMIE_ALGORITHM_H

#include <iostream>
#include <vector>

using namespace std;

enum cellType {unmet, met, recent};

struct Person
{
	string name;
	bool participating = true;
	bool paired = false;
	vector <cellType> met;
};

class zoomieAlgorithm
{
private:
	string names;
	vector<Person> people;
	vector<string> vPrintPairs;
	//vector<bool> recentPairs;
	int expectedEmpty =0;

	vector<cellType> StoVcT(string);
	string PtoS(Person);
	void printPeople();
	void addPeople();
	void notParticipating(string);
	int getBorderIndex();
	void Pair();
	int numNotParticipating();
	void resetPaired();
	bool PossiblePair();
	int peopleNotMet(Person);
	void printPairs();
	int notPaired();
	void printNotPaired();
	void reset();
	Person findPerson(string);
	int findPersonIndex(string);
	void refreshAllMet();
	bool unpairedAllMet();
	void resetRecent();
public:
	void writeIn();
	void writeOut();
	void savePrevious();
	void menu();
};

#endif