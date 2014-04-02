#include "params.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define LINELEN 25

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
			printHelp(NULL);
			exit(0);
		}
		else if(strcmp(aArgv[wI], "-n") == 0) {
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
			if(wI + 1 < aArgc){
				wI++;
				wParams->pWordsLoaded = loadFile(aArgv[wI], &wParams->pWords);
			}else{
				printHelp("generator : option -w : requires a path to a file");
				exit(-1);
			}
		}
		else if(strcmp(aArgv[wI], "-s") == 0) {
			if(wI + 1 < aArgc){
				wI++;
				wParams->pSpecialsLoaded = loadFile(aArgv[wI], &wParams->pSpecials);
			}else{
				printHelp("generator : option -s : requires a path to a file");
				exit(-1);
			}
		}
		else if(strcmp(aArgv[wI], "-l") == 0) {
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

	//check rules can work
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
			/* Double our allocation and re-allocate */
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

void printHelp(char* aMessage){
	
	if(aMessage){
		fprintf(stdout, "%s\n", aMessage);	
	}
	fprintf(stdout, "generator, version 0.1\n\n");

	fprintf(stdout, "usage : generator [-n number] [-w wordFile] [-s specialFile] [-l length] [-ls specials] [-ln numbers] [-lc capitals]\n\n");
	fprintf(stdout, "  n number : number of passwords to generate\n");
	fprintf(stdout, "  w wordFile : path to a text file containing a word per line\n");
	fprintf(stdout, "  s specialFile : path to a text file containing a special character per line\n");
	fprintf(stdout, "  l length : length of every passwords to generate\n");
	fprintf(stdout, "  ls specials : number of specials to use\n");
	fprintf(stdout, "  ln numbers : number of specials to use\n");
	fprintf(stdout, "  lc capitals : number of capitals to use\n");

	fprintf(stdout, "\nBy default, it generates a 16 character long password with only lower case letters\n");
	
}

