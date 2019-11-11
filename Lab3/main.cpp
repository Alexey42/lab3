#include "incl.h"

using namespace std;

void main() {
	Program pr;
	string formula = " ";

	while (true) {
		cout << "Enter formula: ";
		cin >> formula;
		if (formula == "q") break;
		pr.Parse(formula);
	}

	//system("pause");
}