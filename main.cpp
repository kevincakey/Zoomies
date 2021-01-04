#include <iostream>
#include "zoomieAlgorithm.h"

using namespace std;

int main()
{
	zoomieAlgorithm t;
	t.writeIn();
	t.savePrevious();
	t.menu();
	t.writeOut();

	return 0;
}