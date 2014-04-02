
typedef struct {
	char** pWords; // default what is in NULL
	int pWordsLoaded; //defualt is 0
	char** pSpecials; //default what is NULL
	int pSpecialsLoaded; //defualt is 0
	int pNumberOfPasswords; //default is 1
	int pLengthOfPasswords; // default is 16
	int pNumberOfSpecials; //default 1
	int pNumberOfCapitals; //default 2
	int pNumberOfNumbers; // default 2
} Params;

Params* loadParams(int aArgc, char** aArgv);
void unloadParams(Params* aParams);
int loadFile(char* aPath, char*** aList);
void printHelp(char* aMessage);
