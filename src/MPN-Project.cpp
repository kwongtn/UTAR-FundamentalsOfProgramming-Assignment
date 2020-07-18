// MPN-Project.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "paramsAndUtils.h"

DATA_ROW rows[125];

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

	cout << "\rSuccesfully imported " << rowCounter << " rows." << endl;
	pause();
	return true;

}

// List everything
void list() {
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
				// Search by Combination of Positives
				break;

			case 3:
				// Insert data
				break;

			case 4:
				// Update existing data
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
