#pragma once

#include <iostream>
#include <fstream>
#include <cstring>
#include <sstream>
#include <iomanip>
#include <vector>
#include <chrono>
#include <limits>

using namespace std;

const int ARR_SIZE = 125;
const int EXP_SIZE = 5;

struct DATA_ROW {
	int combination_of_positives[3] = { 0 };
	int mpn_index_per_100ml = 0;

	struct CONF_LIMIT {
		int lower = 0;
		int upper = 0;

	} conf_limit;

	bool isUsed = false;
};

inline int toInt(std::string myString) {
	string errStr = "";
	try
	{
		int i = std::stoi(myString);
		return i;
	}
	catch (std::invalid_argument const& e)
	{
		throw new string("Bad input: std::invalid_argument thrown. Input not a number.");
	}
	catch (std::out_of_range const& e)
	{
		throw new string("Integer overflow: std::out_of_range thrown. ");
	}
	throw new string("Unknown Integer Error. ");
}

inline bool testEmptyString(string str) {
	if (str != "") {
		return false;
	}
	else {
		return true;
	}
}

inline void clearScreen() {
	system("cls");
}

inline void pause() {
	system("pause");
}

inline void printLine(int num = 30, char myChar = '=') {
	for (int i = 0; i < num; i++) {
		cout << myChar;
	}
	cout << endl;
}

inline string combinationOfPositivesString(DATA_ROW row) {
	return to_string(row.combination_of_positives[0]) + "-" +
		to_string(row.combination_of_positives[1]) + "-" +
		to_string(row.combination_of_positives[2]);
}

inline bool compareCombinationOfPositivesString(DATA_ROW row1, DATA_ROW row2, bool respectUsedUnused = true) {
	if (respectUsedUnused) {
		if (!row1.isUsed) {
			return false;
		}
	}
	if (combinationOfPositivesString(row1) == combinationOfPositivesString(row2)) {
		return true;
	}
	else {
		return false;
	}
}

inline int inputInt(bool prompter = true, bool forceInput = true) {
	std::string myString = "";
	int i;
	while (true) {
		cin.clear();
		if (prompter) {
			cout << "\n> ";
		}
		getline(cin, myString);

		if (myString == "" && !forceInput) {
			return -1000;
		}

		try
		{
			i = std::stoi(myString);
			break;
		}
		catch (std::invalid_argument const& e)
		{
			cout << "Bad input: std::invalid_argument thrown. Please re-input." << '\n';
			!prompter ? prompter = !prompter : prompter;
			continue;
		}
		catch (std::out_of_range const& e)
		{
			cout << "Integer overflow: std::out_of_range thrown. Please re-input." << '\n';
			!prompter ? prompter = !prompter : prompter;
			continue;
		}

	}

	return i;

}

inline int inputIntWithLimit(int limit, bool prompter = true, bool forceInput = true) {
	while (true) {
		int myInt = inputInt(prompter, forceInput);

		if ((myInt < 0) || (myInt > limit)) {
			cout << "Invalid input, please try again. The number should be not more than " << limit;
			pause();
			continue;
		}
		else {
			return myInt;
		}
	}
}

inline bool decider(std::string custString = "Your selection (y / n): ") {
	bool x = true;
	std::string selection;
	while (true) {
		cout << custString;
		try {
			getline(cin, selection);
			if (selection == "y" || selection == "Y") {
				x = true;
				break;
			}
			else if (selection == "n" || selection == "N") {
				x = false;
				break;
			}
			else {
				throw "Error";
			}
			if (cin.fail() || selection != "y" || selection != "Y" || selection != "n" || selection != "N") {
				throw "Error";
			}
		}
		catch (...) {
			cout << "Please enter a valid character." << endl;
			pause();
		}
	}
	return x;

}

inline std::string returnDatetimeString(bool includeDashes = false)
{
	time_t t = time(nullptr);
	struct tm nowTime;
	localtime_s(&nowTime, &t);

	std::stringstream ss;
	ss << nowTime.tm_year + 1900;
	if (includeDashes) { ss << "-"; }
	if (nowTime.tm_mon < 10) { ss << "0"; }
	ss << nowTime.tm_mon;
	if (includeDashes) { ss << "-"; }
	if (nowTime.tm_mday < 10) { ss << "0"; }
	ss << nowTime.tm_mday;
	if (includeDashes) { ss << " "; }
	if (nowTime.tm_hour < 10) { ss << "0"; }
	ss << nowTime.tm_hour;
	if (includeDashes) { ss << ":"; }
	if (nowTime.tm_min < 10) { ss << "0"; }
	ss << nowTime.tm_min;
	if (includeDashes) { ss << ":"; }
	if (nowTime.tm_sec < 10) { ss << "0"; }
	ss << nowTime.tm_sec;

	return ss.str();
}

/*
	Finds the index number of the first unused row.
*/
inline int findFirstUnused(DATA_ROW rows[]) {
	for (int i = 0; i < ARR_SIZE; i++) {
		if (!rows[i].isUsed) {
			return i;
		}
	}

	return -1;
}
