// MPN-Project.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "paramsAndUtils.h"

DATA_ROW rows[ARR_SIZE];

/*
	Sort the rows based on the combination of positives
*/
void sortRows() {
	// Place scores for the rows in order to sort them later
	int scores[ARR_SIZE];
	for (int i = 0; i < ARR_SIZE; i++) {
		if (rows[i].isUsed == false) {
			scores[i] = numeric_limits<int>::max();
		}
		else {
			scores[i] =
				(100 * rows[i].combination_of_positives[0]) +
				(10 * rows[i].combination_of_positives[1]) +
				(1 * rows[i].combination_of_positives[2]);

		}
	}

	// Sorting
	int sortCounter = 0;
	do {
		sortCounter = 0;
		for (int i = 0; i < ARR_SIZE - 1; i++) {
			if (scores[i] > scores[i + 1]) {
				swap(scores[i], scores[i + 1]);
				swap(rows[i], rows[i + 1]);
				sortCounter++;
			}
		}


	} while (sortCounter > 0);

}

// Check if the row exist based on postive triplet
bool checkDuplicate(DATA_ROW myRow) {
	for (DATA_ROW row : rows) {
		if (compareCombinationOfPositivesString(row, myRow)) {
			return true;
		}
	}
	return false;
}

// Loads file
bool loadFile() {
	clearScreen();
	string inputFilePath = "";
	cout << "Please input the path of the file you want to load: \n> ";
	getline(cin, inputFilePath);

	cout << endl;

	int rowCounter = 0;
	ifstream inputFile(inputFilePath);
	if (!inputFile.is_open()) {
		cout << "File open error. Please try again." << endl;
		system("pause");
		return false;
	}
	else {
		try {
			// Reads the file sequentially
			while (!inputFile.eof()) {
				cout << "\rImporting row " << rowCounter + 1;
				if (rowCounter < ARR_SIZE) {

					// Reading the combination of positives
					for (int i = 0; i < 3; i++) {
						string tempStr = "";
						try {
							inputFile >> tempStr;
							testEmptyString(tempStr) ?
								throw new string("Empty Input. ") : 0;
							rows[rowCounter].combination_of_positives[i] = toInt(tempStr);

							/*
								To verify if the combination of positives are correct, as they cannot be more than 5 (as defined in the assignment)
							*/
							if (rows[rowCounter].combination_of_positives[i] > EXP_SIZE) {
								std::stringstream ss;
								ss << "Experiment size larger than preset of " << EXP_SIZE;
								throw ss.str();
							}
						}
						catch (string* str) {
							std::stringstream ss;
							ss << "Row " << rowCounter + 1 << " \'Combination of Positives\' Value " << i + 1 << " - " << *str;
							throw ss.str();
						}

					}

					// Check if the entry is a duplicate. If yes abort the program, else write into actual array
					for (int i = 0; i < rowCounter; i++) {
						if (compareCombinationOfPositivesString(rows[i], rows[rowCounter])) {
							std::stringstream ss;
							ss << "Row " << i + 1 << " is a duplicate at the combination of positives. Please check the source file.";
							throw ss.str();
						}

					}


					// Reading the MPN Index per 100 ml
					try {
						string tempStr = "";
						inputFile >> tempStr;
						testEmptyString(tempStr) ?
							throw new string("Empty Input. ") : 0;

						rows[rowCounter].mpn_index_per_100ml = toInt(tempStr);
					}
					catch (string* str) {
						std::stringstream ss;
						ss << "Row " << rowCounter + 1 << " \'MPN Index Per 100 ml\' - " << *str;
						throw new string(ss.str());
					}

					// Reading the 95% Lower Confidence Limit
					try {
						string tempStr = "";
						inputFile >> tempStr;
						testEmptyString(tempStr) ?
							throw new string("Empty Input. ") : 0;

						rows[rowCounter].conf_limit.lower = toInt(tempStr);
					}
					catch (string* str) {
						std::stringstream ss;
						ss << "Row " << rowCounter + 1 << " \'95% Confidence Limit (Lower)\' - " << *str;
						throw new string(ss.str());
					}

					// Reading the 95% Upper Confidence Limit
					try {
						string tempStr = "";
						inputFile >> tempStr;
						testEmptyString(tempStr) ?
							throw new string("Empty Input. ") : 0;

						rows[rowCounter].conf_limit.upper = toInt(tempStr);
					}
					catch (string* str) {
						std::stringstream ss;
						ss << "Row " << rowCounter + 1 << " \'95% Confidence Limit (Upper)\' - " << *str;
						throw new string(ss.str());
					}

					rows[rowCounter].isUsed = true;
					rowCounter++;

				}
			}

		}
		catch (string* str) {
			cout << endl << "ERROR: " << *str << "\n\nExiting..." << endl;
			pause();
			exit(1);
		}
		catch (...) {
			cout << endl << "ERROR: Unknown file read error on row " << rowCounter + 1 << ". \n\nExiting..." << endl;
			pause();
			exit(1);
		}
	}

	sortRows();
	cout << "\rSuccesfully imported " << rowCounter << " rows." << endl;
	pause();
	return true;

}

// List everything
void list() {
	sortRows();
	clearScreen();

	// Output headers
	cout
		<< setw(29) << left << "Combination of Positives" << "|"
		<< setw(24) << left << " MPN Index per 100ml" << "|"
		<< setw(30) << "    95% Confidence Limits    " << "|"
		<< endl;

	cout
		<< setw(29) << left << "" << "|"
		<< setw(24) << left << "" << "|"
		<< setw(15) << "    Lower    " << "|"
		<< setw(14) << "    Upper    " << "|"
		<< endl;

	printLine(29 + 24 + 30 + 4);

	int dataCount = 0;
	// Output data
	for (DATA_ROW row : rows) {
		if (row.isUsed) {
			cout
				<< setw(29) << left << combinationOfPositivesString(row) << "|"
				<< setw(24) << left << row.mpn_index_per_100ml << "|"
				<< setw(15) << left << row.conf_limit.lower << "|"
				<< setw(14) << left << row.conf_limit.upper << "|"
				<< endl;

			dataCount++;
		}
	}

	cout << dataCount << " rows of data outputted." << endl;
	pause();
}

// Search using MPN data
int search() {
	clearScreen();
	sortRows();

	DATA_ROW* compare = new DATA_ROW;

	cout << "Please input the positive triplet number:\n";

	cout << "1st number: ";
	compare->combination_of_positives[0] = inputIntWithLimit(5, false);

	cout << "2nd number: ";
	compare->combination_of_positives[1] = inputIntWithLimit(5, false);

	cout << "3rd number: ";
	compare->combination_of_positives[2] = inputIntWithLimit(5, false);

	int matchedEntry = -1;
	for (int i = 0; i < ARR_SIZE; i++) {
		if (compareCombinationOfPositivesString(rows[i], *compare)) {
			matchedEntry = i;
			break;
		}
	}

	if (matchedEntry == -1) {
		cout << "No entries found with the combination of positive string of " << combinationOfPositivesString(*compare) << ". \n";
		pause();
	}
	else {
		clearScreen();
		cout << "\n\nFor " << combinationOfPositivesString(rows[matchedEntry]) << ", " << endl;
		cout << "MPN = " << rows[matchedEntry].mpn_index_per_100ml << "; " << endl;
		cout << "95% of sampled contain between " << rows[matchedEntry].conf_limit.lower << " and " << rows[matchedEntry].conf_limit.upper << " bacteria/ml." << endl;

		pause();
	}

	return matchedEntry;

}

// Export to file
void exportToFile() {
StartExport:
	clearScreen();
	string outputFilePath = "";
	cout << "Please input the path of the file you want to output to, or input \'d\' for a default name: \n> ";
	getline(cin, outputFilePath);

	if (outputFilePath == "d") {
		outputFilePath == "./" + returnDatetimeString(true) + "_output.txt";
	}

	cout << endl;

	int rowCounter = 0;
	ofstream outputFile;
	if (!outputFile.is_open()) {
		cout << "File open error. Do you want to try again?" << endl;
		if (decider()) {
			goto StartExport;
		}
		else {
			return;
		}
	}
	else {
		for (DATA_ROW row : rows) {
			if (row.isUsed) {
				cout << "\rExporting row " << rowCounter + 1;

				outputFile
					<< row.combination_of_positives[0] << "\t"
					<< row.combination_of_positives[1] << "\t"
					<< row.combination_of_positives[2] << "\t"
					<< row.mpn_index_per_100ml << "\t"
					<< row.conf_limit.lower << "\t"
					<< row.conf_limit.upper << "\t"
					<< "\n";
				rowCounter++;
			}
		}

		cout << "Total " << rowCounter << " rows exported to: \n" << outputFilePath;

	}
}

// Insert
void insert() {
	clearScreen();

	// Check if the array is full
	int size = 0;
	for (DATA_ROW row : rows) {
		row.isUsed ? size++ : 0;
	}
	if (size == ARR_SIZE) {
		cout << "Array is full at " << ARR_SIZE << " entries." << endl;
		pause();
		return;
	}

	// Get combination of positives triplet, with validation
	DATA_ROW userInput;
	while (true) {
		clearScreen();
		cout << "Please input the positive triplet number:\n";

		cout << "1st number: ";
		userInput.combination_of_positives[0] = inputIntWithLimit(5, false);

		cout << "2nd number: ";
		userInput.combination_of_positives[1] = inputIntWithLimit(5, false);

		cout << "3rd number: ";
		userInput.combination_of_positives[2] = inputIntWithLimit(5, false);

		if (checkDuplicate(userInput)) {
			cout << "The positive triplet number is a duplicate, do you want to try again? \n";
			if (decider()) {
				continue;
			}
			else {
				return;
			}
		}
		else {
			break;
		}
	}

	cout << "Please input the MPN Index per 100ml: ";
	userInput.mpn_index_per_100ml = inputInt();

	while (true) {
		cout << "Please input the lower bound of the 95% confidence range: ";
		userInput.conf_limit.lower = inputInt();

		cout << "Please input the upper bound of the 95% confidence range: ";
		userInput.conf_limit.upper = inputInt();

		if (userInput.conf_limit.lower >= userInput.conf_limit.upper) {
			cout << "ERROR: The upper limit must be lesser than the lower limit. Try again? \n";
			if (decider()) {
				clearScreen();
				continue;
			}
			else {
				return;
			}
		}
		break;
	}

	try {
		rows[findFirstUnused(rows)] = userInput;
	}
	catch (...) {
		cout << "Unknown error occured." << endl;
		pause();
		return;
	}

	sortRows();
}

// Update
void update() {
StartUpdate:
	cout << "We will search using the positive triplet number to narrow down the entry you want to edit. " << endl;
	pause();

	int target = search();
	if (target == -1) {
		cout << "Do you want to try again? " << endl;
		if (decider()) {
			goto StartUpdate;
		}
		else {
			return;
		}
	}
	else {
		cout << "Are you sure you want to edit the entry shown above? " << endl;
		if (!decider()) {
			return;
		}
		else {
			DATA_ROW userInput = rows[target]; 
			cout << "Please input the MPN Index per 100ml: ";
			userInput.mpn_index_per_100ml = inputInt();

			while (true) {
				cout << "Please input the lower bound of the 95% confidence range: ";
				userInput.conf_limit.lower = inputInt();

				cout << "Please input the upper bound of the 95% confidence range: ";
				userInput.conf_limit.upper = inputInt();

				if (userInput.conf_limit.lower >= userInput.conf_limit.upper) {
					cout << "ERROR: The upper limit must be lesser than the lower limit. Try again? \n";
					if (decider()) {
						clearScreen();
						continue;
					}
					else {
						return;
					}
				}
				break;
			}

			try {
				rows[target] = userInput;
			}
			catch (...) {
				cout << "An unknown error occured." << endl;
				return;
			}

			cout << "Succesfully updated row." << endl;
			pause();
		}



	}
}


int main() {

	while (!loadFile()) {};

	while (true) {
		// Main menu
		clearScreen();
		const vector<string> menu = {
			"List", "Search", "Insert", "Update", "Query"
		};

		for (int i = 0; i < menu.size(); i++) {
			cout
				<< setw(5) << left << i + 1
				<< menu[i]
				<< endl;
		}

		cout << setw(5) << left << to_string(10) << "Exit" << endl;

		cout << "Please select an option from the menu above.";
		int selection = inputInt();

		if ((selection > menu.size() && selection != 10) || selection < 1) {
			cout << "Please input a valid option. ";
			pause();
			continue;
		}
		else {
			switch (selection) {

			case 1:
				list();
				break;

			case 2:
				search();
				break;

			case 3:
				insert();
				break;

			case 4:
				update();
				break;

			case 5:
				// Query data
				break;

			case 10:
				// Need to program exit sequence here that exports all data into a file
				exportToFile();
				cout << "Goodbye. " << endl;
				exit(0);
			}
		}



	}

	return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
