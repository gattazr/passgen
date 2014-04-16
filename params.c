#include "params.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define LINELEN 25

/**
 * lodParams is a function used to create and load a Param structure. this
 * structure define the rules that will be used during the generation process.
 *
 * @param aArgc int : size of the aArgv array of String
 * @param aArgv array of String : params given to the program
 * @return adress to a malloced structure containing the generation rules
 */
Params* loadParams(int aArgc, char** aArgv){
	int wI;
	Params *wParams = malloc(sizeof(Params));

	wParams->pNumberOfPasswords = 1;
	wParams->pWords = NULL;
	wParams->pWordsLoaded = 0;
	wParams->pSpecials = NULL;
	wParams->pSpecialsLoaded = 0;
	wParams->pLengthOfPasswords = 16;
	wParams->pNumberOfSpecials = 0;
	wParams->pNumberOfCapitals = 0;
	wParams->pNumberOfNumbers = 0;

	for(wI = 0; wI < aArgc ; wI++){
		if(strcmp(aArgv[wI], "-h") == 0 || strcmp(aArgv[wI], "--help") == 0) {
      // the -h or --help will print the help and stop the execution of the
      // program
			printHelp(NULL);
			exit(0);
		}
		else if(strcmp(aArgv[wI], "-n") == 0) {
			// the -n flag indicates the next param will be how many passwords needs to be generated
			if(wI + 1 < aArgc){
				wI++;
				int wtoGenerate = atoi(aArgv[wI]);
				if(wtoGenerate > 0){
					wParams->pNumberOfPasswords = wtoGenerate;
				}else{
					printHelp("generator : option -n : requires a number greater than 0");
					exit(-1);
				}
			}else{
				printHelp("generator : option -n : requires a number greater than 0");
				exit(-1);
			}
		}
		else if(strcmp(aArgv[wI], "-w") == 0) {
      // the -w flag indicates the next param will be the path to a text file containing words
			if(wI + 1 < aArgc){
				wI++;
				wParams->pWordsLoaded = loadFile(aArgv[wI], &wParams->pWords);
			}else{
				printHelp("generator : option -w : requires a path to a file");
				exit(-1);
			}
		}
		else if(strcmp(aArgv[wI], "-s") == 0) {
      // the -s flag indicates the next param will be the path to a text file containing special characters
			if(wI + 1 < aArgc){
				wI++;
				wParams->pSpecialsLoaded = loadFile(aArgv[wI], &wParams->pSpecials);
			}else{
				printHelp("generator : option -s : requires a path to a file");
				exit(-1);
			}
		}
		else if(strcmp(aArgv[wI], "-l") == 0) {
      // the -l flagh indicates the next param will be how long the passwords must be
			if(wI + 1 < aArgc){
				wI++;
				int wInt = atoi(aArgv[wI]);
				if(wInt > 0){
					wParams->pLengthOfPasswords = wInt;
				}else{
					printHelp("generator : option -l : requires a number greater than 0");
					exit(-1);
				}
			}else{
				printHelp("generator : option -l : requires a number greater than 0");
				exit(-1);
			}
		}
		else if(strcmp(aArgv[wI], "-ls") == 0) {
      // the -ls flags indicates the next params will be how many special characters the password must have
			if(wI + 1 < aArgc){
				wI++;
				int wInt = atoi(aArgv[wI]);
				if(wInt > 0){
					wParams->pNumberOfSpecials = wInt;
				}else{
					printHelp("generator : option -ls : requires a number greater than 0");
					exit(-1);
				}
			}else{
				printHelp("generator : option -ls : requires a number greater than 0");
				exit(-1);
			}
		}
		else if(strcmp(aArgv[wI], "-ln") == 0) {
      // the -ls flags indicates the next params will be how many numbers the password must have
			if(wI + 1 < aArgc){
				wI++;
				int wInt = atoi(aArgv[wI]);
				if(wInt > 0){
					wParams->pNumberOfNumbers = wInt;
				}else{
					printHelp("generator : option -ln : requires a number greater than 0");
					exit(-1);
				}
			}else{
				printHelp("generator : option -ln : requires a number greater than 0");
				exit(-1);
			}
		}
		else if(strcmp(aArgv[wI], "-lc") == 0) {
      // the -ls flags indicates the next params will be how many capital letters the password must have
			if(wI + 1 < aArgc){
				wI++;
				int wInt = atoi(aArgv[wI]);
				if(wInt > 0){
					wParams->pNumberOfCapitals = wInt;
				}else{
					printHelp("generator : option -ls : requires a number greater than 0");
					exit(-1);
				}
			}else{
				printHelp("generator : option -ls : requires a number greater than 0");
				exit(-1);
			}
		}
	}

  // if no file were loaded for the special chars, load every special in the ASCII table
  if(wParams->pSpecialsLoaded == 0){
    char **wSpecials;
    wSpecials = malloc(sizeof(char*)*34);
    int wIndex = 0;
    int wJ;
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
    wParams->pSpecialsLoaded = 34;
    wParams->pSpecials = wSpecials;
  }

	//check that the given rules can work together
	int wNumberOfLetters = wParams->pLengthOfPasswords - wParams->pNumberOfSpecials - wParams->pNumberOfNumbers;
	if(wParams->pLengthOfPasswords < wParams->pNumberOfSpecials){
		fprintf(stderr, "generator : the password(s) can not contain this many specials\n");
		exit(-1);
	}
	if(wParams->pLengthOfPasswords < wParams->pNumberOfNumbers){
		fprintf(stderr, "generator : the password(s) can not contain this many numbers\n");
		exit(-1);
	}
	if(wNumberOfLetters < wParams->pNumberOfCapitals){
		fprintf(stderr, "generator : the password(s) can not contain this many capitals\n");
		exit(-1);
	}
	if(wNumberOfLetters < 0){
		fprintf(stderr, "generator : the password(s) can not contain this many specials and numbers\n");
		exit(-1);
	}

	return wParams;
}

/**
 * unloadParams is a function used to destroy and free the allocated memory used
 * for a Param structures
 *
 *  @param aParams Param* : pointer to a Param structure to destroy and free
 */
void unloadParams(Params* aParams){
	int wI;
	if(aParams->pWords != NULL){
		for(wI = 0; wI < aParams->pWordsLoaded; wI++){
			free(aParams->pWords[wI]);
		}
		free(aParams->pWords);
	}
	if(aParams->pSpecials != NULL){
		for(wI = 0; wI < aParams->pSpecialsLoaded; wI++){
			free(aParams->pSpecials[wI]);
		}
		free(aParams->pSpecials);
	}
	free(aParams);
}

/**
 * loadFile is a function used to load every line of a given file into a String
 * array. If the file is too big to be loaded into the array, the programm will exit.
 *
 * @param aPath char* : String containing the path to the file to load
 * @param aPath char*** : pointer to a soon to be String array containing every lines
 * of the given file
 *
 *  @return int : the number of line that has been loaded.
 */
int loadFile(char* aPath, char*** aList){
	char **wList;

	if(aPath == NULL){
		aList = NULL;
		return 0;
	}

	int wLines = 128;

	wList = (char **)malloc(sizeof(char*)*wLines);
	if (wList == NULL) {
		printHelp("generator : out of memory.");
		exit(-1);
	}

	FILE *wFile = fopen(aPath, "r");
		if ( wFile == NULL){
		fprintf(stderr, "generator : The file %s could not be opened.\n", aPath);
		printHelp(NULL);
		exit(-1);
	}

	int wI;
	for (wI=0;1;wI++) {
		int wJ;
		/* Have we gone over our line allocation? */
		if (wI >= wLines) {
			int new_size;
			/* Double the allocation and re-allocate */
			new_size = wLines*2;
			wList = (char **)realloc(wList, sizeof(char*)*new_size);
			if (wList==NULL) {
				printHelp("generator : out of memory.");
				exit(-1);
			}
			wLines = new_size;
		}
		/* Allocate space for the next line */
		wList[wI] = malloc(LINELEN);
		if (wList[wI]==NULL) {
			printHelp("generator : out of memory.");
			exit(-1);
		}
		if (fgets(wList[wI],LINELEN-1,wFile)==NULL){
			wList = (char **)realloc(wList, sizeof(char*)*wI);
			break;
		}
		/* Get rid of CR or LF at end of line */
		for (wJ=strlen(wList[wI])-1;wJ>=0 && (wList[wI][wJ]=='\n' || wList[wI][wJ]=='\r');wJ--){
			wList[wI][wJ]=0;
		}
	}

	*aList = wList;
	return wI;
}

/**
 * printHelp is a function printing in stdout the help of the programm. This function
 * will also print a message (error message) that are given to the function
 *
 * @param aMessage char* : String containing a message to print before the help
 */
void printHelp(char* aMessage){

	if(aMessage){
		fprintf(stdout, "%s\n", aMessage);
	}
	fprintf(stdout, "passgen, version 0.1\n\n");

	fprintf(stdout, "usage : passgen [-n number] [-w wordFile] [-s specialFile] [-l length] [-ls specials] [-ln numbers] [-lc capitals]\n\n");
	fprintf(stdout, "  n number : number of passwords to generate\n");
	fprintf(stdout, "  w wordFile : path to a text file containing a word per line\n");
	fprintf(stdout, "  s specialFile : path to a text file containing a special character per line\n");
	fprintf(stdout, "  l length : length of every passwords to generate\n");
	fprintf(stdout, "  ls specials : number of specials to use\n");
	fprintf(stdout, "  ln numbers : number of specials to use\n");
	fprintf(stdout, "  lc capitals : number of capitals to use\n");

	fprintf(stdout, "\nBy default, it generates a 16 character long password with only lower case letters\n");
}
