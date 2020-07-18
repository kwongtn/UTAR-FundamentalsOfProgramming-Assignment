// MPN-Project.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "paramsAndUtils.h"

DATA_ROW rows[125];

// Loads file
bool loadFile() {
	string inputFilePath = "";
	cout << "Please input the path of the file you want to load: \n> ";
	getline(cin, inputFilePath);

	cout << endl;

	ifstream inputFile(inputFilePath);
	if (!inputFile.is_open()) {
		cout << "File open error. Please try again." << endl;
		system("pause");
		return false;
	}
	else {
		int rowCounter = 0;
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
			system("pause");
			exit(1);
		}
		catch (...) {
			cout << endl << "ERROR: Unknown file read error on row " << rowCounter + 1 << ". \n\nExiting..." << endl;
			system("pause");
			exit(1);
		}
	}

}


int main() {

	while (loadFile()) {};


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
