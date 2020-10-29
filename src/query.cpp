#include "paramsAndUtils.h"

const vector<string> menu = {
	"Tubes receiving 10ml show growth",
	"Tubes receiving 1ml show growth",
	"Tubes receiving 0.1ml show growth"
};

void queryOutputByCombinationOfPositives(DATA_ROW rows[], int selection, int criteria) {
	heading("Query by Combination of Positives -> Results");
	printLine();

	cout << menu[selection] << " with " << criteria << " positives are:" << endl;
	outputListingHeaders();

	int counter = 0;
	for (int i = 0; i < ARR_SIZE; i++) {
		if (rows[i].combination_of_positives[selection] == criteria && rows[i].isUsed) {
			outputRow(rows[i]);
			counter++;
		}
	}

	cout << "\nTotal " << counter << " rows returned." << endl;
	pause();
}


void queryByCombinationOfPositives(DATA_ROW rows[]) {
	while (true) {
		heading("Query by Combination of Positives -> Sub-Menu");
		printLine();

		for (int i = 0; i < menu.size(); i++) {
			cout
				<< setw(5) << left << i + 1
				<< menu[i]
				<< endl;
		}

		cout << setw(5) << left << to_string(10) << "Back to main menu" << endl;

		cout << "\nPlease select an option from the menu above.";
		int selection = inputInt();

		if ((selection > menu.size() && selection != 10) || selection < 1) {
			cout << "Please input a valid option. ";
			pause();
			continue;
		}

		heading("Query by Combination of Positives -> Criteration");
		printLine();

		cout << "How many positives should the " << menu[selection - 1] << "?";
		int criteria = inputIntWithLimit(5);

		queryOutputByCombinationOfPositives(rows, selection - 1, criteria);
		cout << "Do you want to search again?" << endl;

		if (decider()) {
			continue;
		}
		else {
			break;
		}
	}
}