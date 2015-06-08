/*
Author:

    Dr. Booth

Assignment Title:

    CSI 1430/1440 Comment Parser

Assignment Description: This program checks for proper documentation
                        in CSI 1430 programs.  TRUE == 1 and FALSE == 0
Due Date: 5:23:2015

Date Created: 5/15/2015

Date Last Modified: 5/16/2015

Usage:  commentChecker <flag> <file...>

Flags:  -h - header comments
        -a - apio comments
        -b - function block comments
        Note: -a and -b are mututally exclusive
 */


/*
 *
 * 	Data Abstraction:
 *		string array to hold comment tags
 *		bool array to hold state of tag T-present F-missing
 *		int rv - return value 1-Pass 0-Fail
 *
 * 	Input:
 *		file containing source code
 *
 * 	Process:
 *		parse source code looking for comment tags
 *
 * 	Output:
 *		return value 0/1 (fail/pass)
 *		also, stdout - first tag violation ONLY
 *
 * 	Assumptions:
 *		students will be using published documentaiton standard.
 *
 */



#include <iostream>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "parseDoc.h"

using namespace std;

int main(int argc, char* argv[]) {
	string token, line, lastLine;
	stringstream report;
	fstream file;
	int rv = TRUE;

	bool pass = false;
	bool stopChecking 		  = false;
	bool HEADER_COMMENTS 	  = false;
	bool APIO_COMMENTS 		  = false;
	bool BLOCK_COMMENTS 	  = false;
	bool HEADER_COMMENTS_PASS = false;
	bool BLOCK_COMMENTS_PASS  = false;
	bool APIO_COMMENTS_PASS   = false;


    if (argc < 3){
    	cout << "Usage Error: too few arguments" << endl;
    	cout << "Usage: -h<a | b> <file...>" << endl;
    	exit(-1);
    }

    //check for flags
    if(argv[1][0] != '-'){
    	cout << "Usage Error: Missing flags" << endl;
    	exit(1);
    }
    else{
    	for(int i = 1 ; i < strlen(argv[1]); i++){
    		if(argv[1][i] == 'h') HEADER_COMMENTS = true;
    		if(argv[1][i] == 'a') APIO_COMMENTS   = true;
    		if(argv[1][i] == 'b') BLOCK_COMMENTS  = true;
    	}
    }

    if(APIO_COMMENTS && BLOCK_COMMENTS){
    	cout << "Usage Error: Cannot have both -a and -b flags." << endl;
    	exit(1);
    }


    //Open each source file and look for header comments
    for(int i = 2; i < argc; i++){

		file.clear();
		file.open(argv[i],ios::in);
		if(!file){
			report << "Error: Missing source code file." << endl;
			pass = false;
			rv = FALSE;
		}
		// new file, reset all flags
		for(int i = 0; i < SIZE; i++){
			keyFlag[i] = false;
		}

		HEADER_COMMENTS_PASS = false;
	    BLOCK_COMMENTS_PASS  = false;
	    APIO_COMMENTS_PASS   = false;
	    stopChecking = false;

		//check for header and apio comments
		while(getline(file, line) && !stopChecking){


			int value = findKey(key, line, SIZE);
            //cout << value << "  " << line << endl;
			switch (value){

			    //first line with # sign, probably from a #include <file>
				case 0:  //found a #include
					   if(!BLOCK_COMMENTS){
						   stopChecking = true;
					   }
				       break;

				//Author:
				case 1:
					   if(HEADER_COMMENTS){
						   keyFlag[value] = compareUserNameToDirectory(line);
						   if(!keyFlag[value]){
							   cout << "FAIL: Invalit Author." << endl;
							   stopChecking = true;
						   }
					   }
				       break;

				//Assignment Title:
				case 2:
						if(HEADER_COMMENTS && !keyFlag[value]){
							   keyFlag[value] = verifyDocumentationLength(line, file, MIN_LENGTH_OF_TITLE);
							   if(!keyFlag[value]){
								   cout << "FAIL: Title is too short." << endl;
								   stopChecking = true;
							   }
						}
				       break;

				//Assignment Description:
				case 3:
						if(HEADER_COMMENTS && !keyFlag[value]){
							keyFlag[value] = verifyDocumentationLength(line, file, MIN_LENGTH_OF_DESCRIPTION);
							   if(!keyFlag[value]){
								   cout << "FAIL: Descritpion is too short." << endl;
								   stopChecking = true;
							   }
						}
			       	   break;


				//Due Date:
				case 4:
						if(HEADER_COMMENTS && !keyFlag[value]){
						   keyFlag[value] = verifyDate(line, file);
						   if(!keyFlag[value]){
							   cout << "FAIL: Bad or Missing Date." << endl;
							   stopChecking = true;
						   }
						}
			         break;


				//Date Created:
				case 5:
						if(HEADER_COMMENTS && !keyFlag[value]){
							keyFlag[value] = verifyDate(line, file);
							if(!keyFlag[value]){
								cout << "FAIL: Bad or Missing Date." << endl;
								stopChecking = true;
							}
						}
			       	   break;


				//Date Last Modified:
				case 6:
						if(HEADER_COMMENTS && !keyFlag[value]){
							keyFlag[value] = verifyDate(line, file);
							if(!keyFlag[value]){
							  cout << "FAIL: Bad or Missing Date." << endl;
							  stopChecking = true;
							}
						}
			       	   break;


				//Data Abstraction:
				case 7:
					if(APIO_COMMENTS && !keyFlag[value]){
						keyFlag[value] = verifyDocumentationLength(line, file, MIN_LENGTH_OF_ABSTRACTION);
					   if(!keyFlag[value]){
						   cout << "FAIL: Abstraction section is too short." << endl;
						   stopChecking = true;
					   }
					}
		       	   break;


				//Input:
				case 8:
					if(APIO_COMMENTS && !keyFlag[value]){
						keyFlag[value] = verifyDocumentationLength(line, file, MIN_LENGTH_OF_INPUT);
						if(!keyFlag[value]){
						   cout << "FAIL: Input section is too short." << endl;
						   stopChecking = true;
						}
					}
		       	    break;


				//Process:
				case 9:
					if(APIO_COMMENTS && !keyFlag[value]){
						keyFlag[value] = verifyDocumentationLength(line, file, MIN_LENGTH_OF_PROCESS);
						if(!keyFlag[value]){
						   cout << "FAIL: Process section is too short." << endl;
						   stopChecking = true;
						}
					}
					break;


				//Output:
				case 10:
					if(APIO_COMMENTS && !keyFlag[value]){
						keyFlag[value] = verifyDocumentationLength(line, file, MIN_LENGTH_OF_OUTPUT);
						if(!keyFlag[value]){
						   cout << "FAIL: Output section is too short." << endl;
						   stopChecking = true;
						}
					}
					break;


				//Assumptions
				case 11:
					if(APIO_COMMENTS && !keyFlag[value]){
						keyFlag[value] = verifyDocumentationLength(line, file, MIN_LENGTH_OF_ASSUMPTIONS);
						if(!keyFlag[value]){
						   cout << "FAIL: Assumptions section is too short." << endl;
						   stopChecking = true;
						}
					}
					break;


				//description:
				case 12:
					if(BLOCK_COMMENTS){
						keyFlag[value] = verifyDocumentationLength(line, file, MIN_LENGTH_OF_DESCRIPTION);
						if(!keyFlag[value]){
						   //cout << "FAIL: Function Description section is too short." << endl;
						   //stopChecking = true;
						}
					}
					break;


				//return:
				case 13:
					if(BLOCK_COMMENTS){
						keyFlag[value] = verifyDocumentationLength(line, file, MIN_LENGTH_OF_RETURN);
						if(!keyFlag[value]){
						   //cout << "FAIL: Function Return section is too short." << endl;
						   //stopChecking = true;
						}
					}
					break;


				//pre:
				case 14:
					if(BLOCK_COMMENTS){
						keyFlag[value] = verifyDocumentationLength(line, file, MIN_LENGTH_OF_PRE);
						if(!keyFlag[value]){
						   //cout << "FAIL: Function PreCondition section is too short." << endl;
						   //stopChecking = true;
						}
					}
					break;


				//post
				case 15:
					if(BLOCK_COMMENTS){
						keyFlag[value] = verifyDocumentationLength(line, file, MIN_LENGTH_OF_POST);
						if(!keyFlag[value]){
						   //cout << "FAIL: Function PostCondition section is too short." << endl;
						   //stopChecking = true;
						}
					}
					break;

				case NOT_FOUNT:
					// no key value, but it could be a function
					if(functionHeader(line)){

						if(BLOCK_COMMENTS && BLOCK_COMMENTS_PASS){
							for(int i = MIN_BLOCK_KEY; i <= MAX_BLOCK_KEY; i++){
								keyFlag[i] = false;
							}
							BLOCK_COMMENTS_PASS = false;
						}
						else if(BLOCK_COMMENTS && !BLOCK_COMMENTS_PASS){
							cout << "Missing or ILL formed Block Comments" << endl;
							stopChecking = true;
							rv = false;
						}
					}
					break;
				//default
				default: break;

			}

			// see if all block comments have been found
		    if(!BLOCK_COMMENTS_PASS){  // if HEADER COMMENTS is still true check all active keyFlags
		    	BLOCK_COMMENTS_PASS = true;
		    	for(int i = MIN_BLOCK_KEY; i <= MAX_BLOCK_KEY && BLOCK_COMMENTS_PASS; i++){ //check header keyFlags
		    		BLOCK_COMMENTS_PASS = keyFlag[i];
		    	}
		    }

			// see if all APIO comments have been found
		    if(!APIO_COMMENTS_PASS){  // if HEADER COMMENTS is still true check all active keyFlags
		    	APIO_COMMENTS_PASS = true;
		    	for(int i = MIN_APIO_KEY; i <= MAX_APIO_KEY && APIO_COMMENTS_PASS; i++){ //check header keyFlags
		    		APIO_COMMENTS_PASS = keyFlag[i];
		    	}
		    }

			// see if all Header comments have been found
		    if(!HEADER_COMMENTS_PASS){  // if HEADER COMMENTS is still true check all active keyFlags
		    	HEADER_COMMENTS_PASS = true;
		    	for(int i = MIN_HEADER_KEY; i <= MAX_HEADER_KEY && HEADER_COMMENTS_PASS; i++){ //check header keyFlags
		    		HEADER_COMMENTS_PASS = keyFlag[i];
		    	}

		    	//if header is finished, reset all other flags
		    	if(HEADER_COMMENTS_PASS){
		    		for(int i = MIN_APIO_KEY; i <= MAX_APIO_KEY && APIO_COMMENTS_PASS; i++){
		    			keyFlag[i] = false;
		    		}
		    		for(int i = MIN_BLOCK_KEY; i <= MAX_BLOCK_KEY && BLOCK_COMMENTS_PASS; i++){
		    			keyFlag[i] = false;
		    		}
		    	}
		    }


		}

		file.close();
	    if(rv && HEADER_COMMENTS){  // if rv is still true check all active keyFlags
	    	rv = HEADER_COMMENTS_PASS;
	    }
	    if(rv && APIO_COMMENTS){  // if rv is still true check all active keyFlags
	    	rv = APIO_COMMENTS_PASS;
	    }
	    if(rv && BLOCK_COMMENTS){  // if rv is still true check all active keyFlags
	    	rv = BLOCK_COMMENTS_PASS;
	    }

	    //cout << rv << "  " << argv[i] << endl;
    }

	return rv;
}
