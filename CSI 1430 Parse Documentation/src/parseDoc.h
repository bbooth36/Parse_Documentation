/*
Author: Dr. Booth

Assignment Title: CSI 1430/1440 Comment Parser

Assignment Description: This program checks for proper documentation
                        in CSI 1430 programs.  TRUE == 1 and FALSE == 0
Due Date: 5/23/2015

Date Created: 5/15/2015

Date Last Modified: 5/16/2015

*/

#ifndef CSI_1430_PARSE_DOCUMENTATION_H_
#define CSI_1430_PARSE_DOCUMENTATION_H_

#include <iostream>
#include <string>
//#include <regex>  c++11 not available on upload site
#include <sstream>
#include <string.h>

using namespace std;

const int TRUE  = 1;
const int FALSE = 0;
const int NOT_FOUNT = -1;
const int SIZE  = 16;  //total number of required comments
const int MIN_HEADER_KEY = 1;
const int MAX_HEADER_KEY = 6;
const int MIN_APIO_KEY   = 7;
const int MAX_APIO_KEY   = 11;
const int MIN_BLOCK_KEY  = 12;
const int MAX_BLOCK_KEY  = 15;

const int MIN_LENGTH_OF_TITLE 		= 22;
const int MIN_LENGTH_OF_DESCRIPTION = 11;
const int MIN_LENGTH_OF_ABSTRACTION = 22;
const int MIN_LENGTH_OF_INPUT 		= 22;
const int MIN_LENGTH_OF_PROCESS 	= 22;
const int MIN_LENGTH_OF_OUTPUT 		= 22;
const int MIN_LENGTH_OF_ASSUMPTIONS = 22;
const int MIN_LENGTH_OF_PRE 		= 11;
const int MIN_LENGTH_OF_POST 		= 11;
const int MIN_LENGTH_OF_RETURN 		= 11;

bool keyFlag[SIZE] = {FALSE};
string key[] ={
				"#include",
				"Author:",
				"Assignment Title:",
				"Assignment Description:",
				"Due Date:",
				"Date Created:",
				"Date Last Modified:",
				"Data Abstraction:",
				"Input:",
				"Process:",
				"Output:",
				"Assumptions:",
				"description:",
				"return:",
				"pre:",
				"post:"
              };

//************************************************************
// description: Find match for key value
// return: type
// pre: key value pairs
// post: returns type id
//
//************************************************************


int findKey(string key[], string item, int size);

//************************************************************
// description: calculate total price of item with tax       *
// return: double                                            *
// pre: price and tax are valid                              *
// post: returns sum of price and tax                        *
//                                                           *
//************************************************************

bool compareUserNameToDirectory(string line);

//************************************************************
// description: calculate total price of item with tax       *
// return: double                                            *
// pre: price and tax are valid                              *
// post: returns sum of price and tax                        *
//                                                           *
//************************************************************

bool verifyDocumentationLength(string line, fstream& file, int length);

//************************************************************
// description: calculate total price of item with tax       *
// return: double                                            *
// pre: price and tax are valid                              *
// post: returns sum of price and tax                        *
//                                                           *
//************************************************************

bool verifyDate(string line, fstream& file);

//************************************************************
// description: calculate total price of item with tax       *
// return: double                                            *
// pre: price and tax are valid                              *
// post: returns sum of price and tax                        *
//                                                           *
//************************************************************

char findDelimiter(string);

//************************************************************
// description: calculate total price of item with tax       *
// return: double                                            *
// pre: price and tax are valid                              *
// post: returns sum of price and tax                        *
//                                                           *
//************************************************************

bool functionHeader(string);


// Function Implementation

int findKey(string key[ ], string item, int size){
	int rv = NOT_FOUNT;
	for(int i = 0; i < size && rv == -1; i++){
		if(item.find(key[i]) != string::npos){
			rv = i;
		}
	}
	return rv;
}

bool compareUserNameToDirectory(string line){
	return true;
}

bool verifyDocumentationLength(string line, fstream& file, int min_length){
	bool flag = TRUE;
	long start = file.tellg();
	int totalLength = line.size();

    while(getline(file,line) && findKey(key, line, SIZE) == NOT_FOUNT){
       totalLength += line.size();
    }
	file.seekg(start,ios::beg);

	if(totalLength < min_length) flag = false;
	return flag;
}


bool verifyDate(string line, fstream& file){
	bool flag = TRUE;
	string dateLine = line;
	char delimiter;

	long start = file.tellg();
	int totalLength = line.size();

    while(getline(file,line) && findKey(key, line, SIZE) == NOT_FOUNT){
       totalLength += line.size();
       dateLine += " ";
       dateLine += line;
    }
	file.seekg(start,ios::beg);

	// OK - all text read.  No look for date
	// Possible date formats
	// mm:dd:yy<yy> , mm/dd/yy<yy> , mm-dd-yy<yy> , Month dd, yyyy

    //dateLine = dateLine.substr(dateLine.find(':')+1, dateLine.size()-dateLine.find(':'));

	delimiter = findDelimiter(dateLine);

	if(delimiter == '\0') {
		flag = FALSE;
	}
	else{
	    /*
	    string pattern;
	    pattern = "\\d{1,2}[-";
	    pattern += delimiter;
	    pattern += ". ]\\d{1,2}[-";
	    pattern += delimiter;
	    pattern += ". ]\\d{1,4}";
	    regex e(pattern);
	    flag = regex_search(dateLine,e);
	    */

		stringstream data(dateLine.substr(dateLine.find(':')+1,dateLine.size()-dateLine.find(':')));
		int day, month, year;
		char delim1, delim2;

		data >> month;
		data >> delim1;
		data >> day;
		data >> delim2;
		data >> year;
		if(flag) flag = day <=31 && day >= 1;
		if(flag) flag = month <=12 && month >= 1;
		if(flag) flag = year <=2020 && year >= 15;
		if(flag) flag = delim1 == delimiter && delim2 == delimiter;

	}
	return flag;
}

char findDelimiter(string line){
	char needle [4] = ":/-";
	char rv = '\0';
	int  delimCount[3] = {0};

	for(int i = 0; i < line.size() && !rv; i++){
		for(int j = 0; j < strlen(needle) && !rv; j++){
			if(line[i] == needle[j]){
				delimCount[j]++;
				if(delimCount[j] >= 2){
					rv = needle[j];
				}
			}
		}
	}
	return rv;
}


bool functionHeader(string line){
	/*
	//string pattern = "^\\s*(unsigned|signed)?\\s*(void|int|char|short|long|float|double)\\s+(\\w+)\\s*\\([^)]*\\)\\s*;";
	string pattern = "^\\s*(unsigned|signed)?\\s*(void|bool|int|char|short|long|float|double|string|fstream)\\s+(\\w+)\\s*\\([^)]*\\)\\s*;";

	regex e(pattern);

	return regex_search(line, e);
	*/
	int paren1, paren2, semiColon;
	bool flag = true;

	paren1    = line.find('(');
	paren2    = line.find(')');
	semiColon = line.find(';');

    if(flag) flag = paren1 > 0;
    if(flag) flag = paren2 > 0;
    if(flag) flag = semiColon > 0;
	if(flag) flag = (paren1 < paren2) && (paren2 < semiColon);

	return flag;
}

#endif /* CSI_1430_PARSE_DOCUMENTATION_H_ */

