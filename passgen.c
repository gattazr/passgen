#include "params.h"
#include "passgen.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
/*
if used without params
	generate will create a random password of 16 char with 2 numbers and 1 special
	Use ASCII tables
Possible params :
	-n : how many params to generate
	-w : path to file contaning words
	-s : path to file contaning specials
	-l : length of the password
	-ls : number of specials
	-ln : number of numbers to use
	-lc : number of capitals
*/
int main(int aArgc, char** aArgv){
	srand ( time(NULL) );
	Params* wParams = loadParams(aArgc, aArgv);
	
	makePassword(*wParams);
	
	unloadParams(wParams);
	return 0;
}


void makePassword(Params aParams){
	int wNumberofChars = aParams.pLengthOfPasswords - aParams.pNumberOfSpecials - aParams.pNumberOfNumbers;
	char wPassword[aParams.pLengthOfPasswords + 1];
	int wRandIndex;
	int wI, wJ, wK;
	for(wI = 0; wI < aParams.pNumberOfPasswords; wI++){

		// check if using words or randomly generated
		if(aParams.pWordsLoaded > 0){
			makeWordPassword(aParams, wPassword);
			while (strlen(wPassword) != wNumberofChars) {
				makeWordPassword(aParams, wPassword);
			}
		}else{
			for(wJ = 0 ; wJ < aParams.pLengthOfPasswords; wJ++){
				wPassword[wJ] = 0;
			}	
			for(wJ = 0 ; wJ < wNumberofChars; wJ++){
				wPassword[wJ] = 'a' + rand() % 26;
			}
		}

		//transform some of the lowercase char into capitals
		for(wJ = 0 ; wJ < aParams.pNumberOfCapitals; wJ++){
			wRandIndex = rand() % wNumberofChars;
			if( wPassword[wRandIndex] >= 'a' && wPassword[wRandIndex] <= 'z'){
				wPassword[wRandIndex] = 'A' + wPassword[wRandIndex] - 'a';	
			}else{
				wJ--;
			}
		}
		
		// add numbers
		for(wJ = 0 ; wJ < aParams.pNumberOfNumbers; wJ++){
			if(wNumberofChars + wJ == 0){
				wRandIndex = 0;
			}else{
				wRandIndex = rand() % ( wNumberofChars + wJ );	
			}
			char wRandNumber = rand() % 10 + '0';
			for(wK = aParams.pLengthOfPasswords; wK > wRandIndex; wK--){
				wPassword[wK] = wPassword[wK - 1];
			}
			wPassword[wRandIndex] = wRandNumber;
		}

		//add special chars
		char** wSpecials;
		int wNumberOfSpecials;
		if(aParams.pSpecialsLoaded > 0){
			wSpecials = aParams.pSpecials;
			wNumberOfSpecials = aParams.pNumberOfSpecials;
		}else{
			wSpecials = malloc(sizeof(char*)*34);
			int wIndex = 0;
			for(wJ = 0; wJ < 15; wJ++){
				wSpecials[wIndex] = malloc(sizeof(char));
				wSpecials[wIndex][0] = '!' + wJ;
				wIndex++;
			}
			for(wJ = 0; wJ < 7; wJ++){
				wSpecials[wIndex] = malloc(sizeof(char));
				wSpecials[wIndex][0] = ':' + wJ;
				wIndex++;
			}
			for(wJ = 0; wJ < 6; wJ++){
				wSpecials[wIndex] = malloc(sizeof(char));
				wSpecials[wIndex][0] = '[' + wJ;
				wIndex++;
			}
			for(wJ = 0; wJ < 6; wJ++){
				wSpecials[wIndex] = malloc(sizeof(char));
				wSpecials[wIndex][0] = '{' + wJ;
				wIndex++;
			}
			wNumberOfSpecials = 34;
		}

		for(wJ = 0 ; wJ < aParams.pNumberOfSpecials; wJ++){
			wRandIndex = rand() % (aParams.pNumberOfNumbers + wNumberofChars);			
			int wRandSpecial = rand() % wNumberOfSpecials;
			char wSpecial = wSpecials[wRandSpecial][0];

			for(wK = aParams.pLengthOfPasswords; wK > wRandIndex; wK--){
				wPassword[wK] = wPassword[wK - 1];
			}
			wPassword[wRandIndex] = wSpecial;
			
		}

		//free special chars if allocated
		if(aParams.pSpecialsLoaded == 0){
			for(wJ = 0; wJ < wNumberOfSpecials; wJ++){
				free(wSpecials[wJ]);
			}
			free(wSpecials);
		}

		fprintf(stdout, "%s\n", wPassword);
	}
}

void makeWordPassword(Params aParams, char* aPassword){
	int wDepth;
	char* wCurrentWord;
	int wSize;
	int wI;

	wDepth = 0;
	wSize = aParams.pLengthOfPasswords - aParams.pNumberOfSpecials - aParams.pNumberOfNumbers;
	for(wI = 0 ; wI < aParams.pLengthOfPasswords; wI++){
		aPassword[wI] = 0;
	}
	while (wDepth < 1000000 && strlen(aPassword) < wSize ) {
		wCurrentWord = aParams.pWords[rand() % aParams.pWordsLoaded];
		if (strlen(wCurrentWord) + strlen(aPassword) <= wSize) {
			strcat(aPassword, wCurrentWord);
		}
		wDepth++;
	}
}
