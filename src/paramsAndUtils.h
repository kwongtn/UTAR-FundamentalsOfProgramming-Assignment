#pragma once

#include <iostream>
#include <fstream>
#include <cstring>
#include <sstream>

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