
typedef struct {
	char** pWords; // default what is in NULL
	int pWordsLoaded; //defualt is 0
	char** pSpecials; //default what is NULL
	int pSpecialsLoaded; //default is 0
	int pNumberOfPasswords; //default is 1
	int pLengthOfPasswords; // default is 16
	int pNumberOfSpecials; //default 0
	int pNumberOfCapitals; //default 0
	int pNumberOfNumbers; // default 0
	int pNumberOfWords; // default 0
} Params;

Params* loadParams(int aArgc, char** aArgv);
void unloadParams(Params* aParams);
int loadFile(char* aPath, char*** aList);
void printHelp(char* aMessage);
