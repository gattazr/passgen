#include "params.h"
#include "passgen.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

/**
 * main is the entry point of the programm. Since this programm accepts params,
 * this function has the two default params.
 *
 * @param aArgc : number of params given to the programm
 * @param aArgv : params given to the params
 *
 * @return int : this function will return 0 and exit the program is no error happened
 */
int main(int aArgc, char** aArgv){
	srand ( time(NULL) );
	Params* wParams = loadParams(aArgc, aArgv);

	makePassword(*wParams);

	unloadParams(wParams);
	return 0;
}


/**
 * makePassword is the funciton that generates the password. The function will generate
 * passwords following the rules defines in the given Param structure.
 *
 * @param aParams Params : structure containing the password generation rules
 */
void makePassword(Params aParams){
	int wNumberofChars = aParams.pLengthOfPasswords - aParams.pNumberOfSpecials - aParams.pNumberOfNumbers;
	char wPassword[aParams.pLengthOfPasswords + 1];
	int wRandIndex;
	int wI, wJ, wK;
	for(wI = 0; wI < aParams.pNumberOfPasswords; wI++){
		// this function will loop to create as many password as required

		if(aParams.pWordsLoaded > 0){
			// if using words, make a wordPassword
			makeWordPassword(aParams, wPassword);
			while (strlen(wPassword) != wNumberofChars) {
				// while the length of the wordPassword is not good, the function will relaunched
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

		// add special chars
		for(wJ = 0 ; wJ < aParams.pNumberOfSpecials; wJ++){
      int wPassLength = strlen(wPassword);
      if(wPassLength > 0){
          wRandIndex = rand() % (wPassLength);
      }else{
        wRandIndex = 0;
      }
			int wRandSpecial = rand() % aParams.pSpecialsLoaded;
			char wSpecial = aParams.pSpecials[wRandSpecial][0];

			for(wK = aParams.pLengthOfPasswords; wK > wRandIndex; wK--){
				wPassword[wK] = wPassword[wK - 1];
			}
			wPassword[wRandIndex] = wSpecial;
		}

		fprintf(stdout, "%s\n", wPassword);
		// end the generation by printing out the password to stdout
	}
}

/**
 * makePassword is a function used to make a password using words. This function
 * will modify a given string to a string composed of words in the given Param structure
 * while following the rules in the rules in the Param struct
 *
 * @param aParams Param : structure containing the generation rules
 * @param aPassword char* : String in which the generator password will be put
 */
void makeWordPassword(Params aParams, char* aPassword){
	int wDepth;
	char* wCurrentWord;
	int wSize;
	int wI;
  int wWordsUsed;

	wDepth = 0;
  wWordsUsed = 0;
	wSize = aParams.pLengthOfPasswords - aParams.pNumberOfSpecials - aParams.pNumberOfNumbers;
	for(wI = 0 ; wI < aParams.pLengthOfPasswords; wI++){
		aPassword[wI] = 0;
	}
	while (wDepth < 1000000 && strlen(aPassword) < wSize ) {
		if(aParams.pNumberOfWords > 0 && wWordsUsed > aParams.pNumberOfWords){
      break;
    }
    wCurrentWord = aParams.pWords[rand() % aParams.pWordsLoaded];
    if (strlen(wCurrentWord) + strlen(aPassword) <= wSize) {
			strcat(aPassword, wCurrentWord);
      wWordsUsed++;
		}
		wDepth++;
	}
  if(aParams.pNumberOfWords > 0 && wWordsUsed != aParams.pNumberOfWords){
    makeWordPassword(aParams, aPassword);
  }
}
