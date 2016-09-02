#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <ctype.h>
#include "SPConfig.h"
#include <assert.h>
//#include "SPLogger.h"

struct sp_config_t{
	char* spImagesDirectory;
	char* spImagesPrefix;
	char* spImagesSuffix;
	int spNumOfImages;
	int spPCADimension;
	char* spPCAFilename;
	int spNumOfFeatures;
	bool spExtractionMode;
	int spNumOfSimilarImages;
	SP_SPLIT_METHOD spKDTreeSplitMethod;
	int spKNN;
	bool spMinimalGUI;
	int spLoggerLevel;
	char* spLoggerFilename;
};

//FUNCTION DECLARATIONS
bool loadData(SPConfig res, char* paramName, char* value, const char* filename, int lineNum, SP_CONFIG_MSG* msg);

SPConfig spConfigCreate(const char* filename, SP_CONFIG_MSG* msg){
	assert( msg != NULL );
	FILE *fp;
	SPConfig res;
	char* temp;
	char* value;
	char* paramName;
	int lineNum;
	bool successfulLoad;


	res = (SPConfig) malloc(sizeof(*res));
	if (res == NULL) {
		return NULL;
	}

	temp = (char*) malloc(CONFIG_LINE_MAX_SIZE+1);
	value = (char*) malloc(CONFIG_LINE_MAX_SIZE+1);
	res->spPCAFilename = (char*) malloc(CONFIG_LINE_MAX_SIZE+1);
	res->spLoggerFilename = (char*) malloc(CONFIG_LINE_MAX_SIZE+1);
	res->spImagesSuffix = (char*) malloc(CONFIG_LINE_MAX_SIZE+1);
	res->spImagesPrefix = (char*) malloc(CONFIG_LINE_MAX_SIZE+1);
	res->spImagesDirectory = (char*) malloc(CONFIG_LINE_MAX_SIZE+1);

		if (temp == NULL || value==NULL ||  !res->spPCAFilename ||
	!res->spPCAFilename || !res->spLoggerFilename || !res->spImagesSuffix || !res->spImagesPrefix || !res->spImagesDirectory) {
			free(temp);
			free(value);
			spConfigDestroy(res);
			*msg = SP_CONFIG_ALLOC_FAIL;
			return NULL;
		}
// all allocs ok

printf("ALLOC OK \n");
	if((fp = fopen(filename, "r")) == NULL){
		*msg = SP_CONFIG_CANNOT_OPEN_FILE;
		free(temp);
		free(value);
		spConfigDestroy(res);
		if (strcmp(filename, "spcbir.config")==0){			//Tells us program is using default config, and it's faulty
			printf("The default config file spcbir is faulty");
		}
		return NULL;
	}
	//We have now succesfully allocated memory for our config struct res, opened our config_file
	// and allocated memory for temp. We begin by setting all the defaults.
	setDefaults(res);

	//We now have a config object with all defaults set, we initialize line counter and begin to review file.
	lineNum = 0;
	//ASSUME success
	//*msg = SP_CONFIG_SUCCESS;

successfulLoad = true;
printf("FileOpenedSuccessfuly\n");
	while(fgets(temp, CONFIG_LINE_MAX_SIZE, fp) != NULL && feof(fp) == 0  && successfulLoad) {
				lineNum++;															//Count starts at 1

				//Copys param name skipping preceeding whitespace, until first space or until '='
				int ind=0;
				paramName = temp;
				while (isspace(*paramName)){
					++paramName;
					++ind;
				}
				if (*paramName == '#'||*paramName =='\n'||*paramName =='\0') {		//Skips comments and 'newline's
					continue;
				}
				while (!(isspace(paramName[ind])||paramName[ind]=='=')){
					ind++;
				}
				temp[ind]='\0';

				strcpy(value, strchr(temp, '='));
				//We now have temp which we will use to discover which param to use, and value which starts at =
				++value;
				while (isspace(*value)){
					++value;
				}
				//Remove spaces at end
				int end = strlen(value)-1;
				while (isspace(value[end])){
					--end;
				}
				value[end+1]='\0';


				//FUNCTION THAT LOADS DATA INTO PARAM NAMES
				successfulLoad = loadData(res, paramName, value, filename, lineNum, msg);
 		}//END OF WHILE LOOP

 int check = checkvalid(res, msg, filename, lineNum);
 if (!*msg == SP_CONFIG_SUCCESS || check<0){
	 spConfigDestroy(res);
	 res = NULL;
 }
 printf("ALL insrt ok\n");
	free(temp);
	free(value);
	fclose(fp);
	return res;
}

//void terminateDuringParse(SPConfig res, char* temp, char* paramName, char* value, FILE *fp, SP_CONFIG_MSG* msg, SP_CONFIG_MSG out){
//	spConfigDestroy(res);
//	free(temp);
//	free(value);
//	fclose(fp);
//	*msg = out;
//	return;
//}

//Check the paramNames to see if the name fits any and if so assign proper value
bool loadData(SPConfig res, char* paramName, char* value, const char* filename, int lineNum, SP_CONFIG_MSG* msg){
	//TEMPORARY VALUE FOR VALIDITY CHECKS
	int tempVal;
	if (strcmp(paramName, "spImagesDirectory")==0) {
		res->spImagesDirectory = readStr(value, filename, lineNum, msg);
		if (res->spImagesDirectory == NULL){
			return false;
		}
	}
	else if (strcmp(paramName, "spImagesPrefix")==0) {
		res->spImagesPrefix = readStr(value, filename, lineNum, msg);
		if (res->spImagesPrefix == NULL){
			return false;
		}
	}
	else if (strcmp(paramName, "spImagesSuffix")==0) {
		res->spImagesSuffix = readSuffix(value, filename, lineNum, msg);
		if (res->spImagesSuffix == NULL){
			return false;
		}
	}
	else if (strcmp(paramName, "spNumOfImages")==0) {
		tempVal = readInt(value, INT_MAX, 0, filename, lineNum, msg);
		if (tempVal == -1){
			return false;
		}
		res->spNumOfImages = tempVal;
	}
	else if (strcmp(paramName, "spPCADimension")==0) {
		tempVal = readInt(value, 28, 10, filename, lineNum, msg);
		if (tempVal == -1){
			return false;
		}
		res->spPCADimension = tempVal;
	}
	else if (strcmp(paramName, "spPCAFilename")==0) {
		res->spPCAFilename = readStr(value, filename, lineNum, msg);
		if (res->spPCAFilename == NULL){
			return false;
		}
	}
	else if (strcmp(paramName, "spNumOfFeatures")==0) {
		tempVal = readInt(value, INT_MAX, 0, filename, lineNum, msg);
		if (tempVal == -1){
			return false;
		}
		res->spNumOfFeatures = tempVal;
	}
	else if (strcmp(paramName, "spExtractionMode")==0) {
		res->spExtractionMode = readBool(value, filename, lineNum, msg);
		if (res->spExtractionMode == -1){
			return false;
		}
	}
	else if (strcmp(paramName, "spNumOfSimilarImages")==0) {
		tempVal = readInt(value, INT_MAX, 1, filename, lineNum, msg);
		if (tempVal == -1){
			return false;
		}
		res->spNumOfSimilarImages = tempVal;
	}
	else if (strcmp(paramName, "spKDTreeSplitMethod")==0) {
		res->spKDTreeSplitMethod = readEnum(value, filename, lineNum, msg);
		if (res->spKDTreeSplitMethod == INVALID){
			return false;
		}
	}
	else if (strcmp(paramName, "spKNN")==0) {
		tempVal = readInt(value, INT_MAX, 1, filename, lineNum, msg);
		if (tempVal == -1){
			return false;
		}
		res->spKNN = tempVal;
	}
	else if (strcmp(paramName, "spMinimalGUI")==0) {
		res->spMinimalGUI = readBool(value, filename, lineNum, msg);
		if (res->spMinimalGUI == -1){
			return false;
		}
	}
	else if (strcmp(paramName, "spLoggerLevel")==0) {
		tempVal = readInt(value, 4, 1, filename, lineNum, msg);
		if (tempVal == -1){
			return false;
		}
		res->spLoggerLevel = tempVal;
	}
	else if (strcmp(paramName, "spLoggerFilename")==0) {
		res->spLoggerFilename = readStr(value, filename, lineNum, msg);
		if (res->spLoggerFilename == NULL){
			return false;
		}
	}
	else {
	*msg = SP_CONFIG_INVALID_LINE;
	return false;
	}
	return true;
}


int checkvalid(SPConfig res, SP_CONFIG_MSG* msg, const char* filename, int lineNum){
	printf(">> >> inside checkvalid\n");
	if (res == NULL){
		return -1;
	}
	if (strcmp(res->spImagesDirectory, " ")==0){
		printParamNotSet(filename, lineNum, "spImagesDirectory");
		//terminateDuringParse(res, temp, paramName, value, fp, msg, SP_CONFIG_MISSING_DIR);
		*msg = SP_CONFIG_MISSING_DIR;
		return -1;
	}
	else if (strcmp(res->spImagesPrefix, " ")==0){
		printParamNotSet(filename, lineNum, "spImagesPrefix");
		//terminateDuringParse(res, temp, paramName, value, fp, msg, SP_CONFIG_MISSING_PREFIX);
		*msg = SP_CONFIG_MISSING_PREFIX;
		return -1;
	}
	else if (strcmp(res->spImagesSuffix, " ")==0){
		printParamNotSet(filename, lineNum, "spImagesSuffix");
		//terminateDuringParse(res, temp, paramName, value, fp, msg, SP_CONFIG_MISSING_SUFFIX);
		*msg = SP_CONFIG_MISSING_SUFFIX;
		return -1;
	}
	else if ( res->spNumOfImages==-1){
		printParamNotSet(filename, lineNum, "spNumOfImages");
		//terminateDuringParse(res, temp, paramName, value, fp, msg, SP_CONFIG_MISSING_NUM_IMAGES);
		*msg = SP_CONFIG_MISSING_NUM_IMAGES;
		return -1;
	}
	return 0;
}

//PRINTING ERROR MESSAGES TO CONSOLE
void printConstraintsNotMet(const char* filename, int lineNum){
	printf("File: %s\nLine: %d\nMessage: Invalid value - constraint not met",filename, lineNum );
}
void printInvalidLine(const char* filename, int lineNum){
	printf("File: %s\nLine: %d\nMessage: Invalid configuration line",filename, lineNum );
}
//TODO
void printParamNotSet(const char* filename, int lineNum, char* paramName){
	printf("File: %s\nLine: %d\nMessage: Parameter %s is not set",filename, lineNum, paramName );
}

void setDefaults(SPConfig config){
	//ALLOC MEM FOR ALL STRING VALUES
	// config->spPCAFilename = (char*) malloc(CONFIG_LINE_MAX_SIZE+1);
	// config->spLoggerFilename = (char*) malloc(CONFIG_LINE_MAX_SIZE+1);
	// config->spImagesSuffix = (char*) malloc(CONFIG_LINE_MAX_SIZE+1);
	// config->spImagesPrefix = (char*) malloc(CONFIG_LINE_MAX_SIZE+1);
	// config->spImagesDirectory = (char*) malloc(CONFIG_LINE_MAX_SIZE+1);

	// if (config->spPCAFilename == NULL || config->spLoggerFilename == NULL || config->spImagesSuffix == NULL ||
	// config->spImagesPrefix == NULL || config->spImagesDirectory == NULL ){
	// 	spConfigDestroy(config);
	// 	return -1;
	// }

	config->spPCADimension = 20;
	strcpy(config->spPCAFilename, "pca.yml");
	config->spNumOfFeatures = 100;
	config->spExtractionMode = 1;
	config->spNumOfSimilarImages = 1;
	config->spKDTreeSplitMethod = MAX_SPREAD;
	config->spKNN = 1;
	config->spMinimalGUI = 0;
	config->spLoggerLevel = 3;
	strcpy(config->spLoggerFilename, "stdout");

	//SET NON-Default values to NULL or -1
	config->spNumOfImages =  -1;
	strcpy(config->spImagesSuffix, " ");
	strcpy(config->spImagesPrefix, " ");
	strcpy(config->spImagesDirectory, " ");

	//return 0;
	}

//HELPER FUNCTIONS TO LOAD DATA FROM STRING FORN IN CONFIG LINE
int readInt(char* value, int maxLength, int minLength, const char* filename, int lineNum, SP_CONFIG_MSG* msg){
	int num;
	if (isValidInt(value)){
		num = atoi(value);
	}else{
		printInvalidLine(filename, lineNum);
		*msg = SP_CONFIG_INVALID_ARGUMENT;
		return -1;
	}
	if( minLength <= num && num<=maxLength ){
		return num;
	}else {
		printConstraintsNotMet(filename, lineNum);
		*msg = SP_CONFIG_INVALID_INTEGER;
		return -1;
	}
}

char* readStr(char* val, const char* filename, int lineNum, SP_CONFIG_MSG* msg){
	if (val == NULL){
		printInvalidLine(filename, lineNum);
		*msg = SP_CONFIG_INVALID_STRING;
		return NULL;
	}
	if (isValidString(val)){
		return val;
	}else{
		printInvalidLine(filename, lineNum);
		*msg =SP_CONFIG_INVALID_STRING;
		return NULL;
	}
}

char* readSuffix(char* val, const char* filename, int lineNum, SP_CONFIG_MSG* msg){
	if (val == NULL){
		printInvalidLine(filename, lineNum);
		*msg = SP_CONFIG_INVALID_STRING;
		return NULL;
	}
	if (!isValidString(val)){
		printInvalidLine(filename, lineNum);
		*msg = SP_CONFIG_INVALID_STRING;
		return NULL;
	}
	if ( (strcmp(val,".png")==0) || (strcmp(val,".bmp")==0) || (strcmp(val,".jpg")==0) || (strcmp(val,".gif")==0) ){
		return val;
	}
	printInvalidLine(filename, lineNum);
	*msg =  SP_CONFIG_INVALID_STRING;
	return NULL;
}

bool readBool(char* val, const char* filename, int lineNum, SP_CONFIG_MSG* msg){				//TODO - can the user define bool as 0/1 does case matter True/true/tRue??
	if (val == NULL){
		printInvalidLine(filename, lineNum);
		*msg = SP_CONFIG_INVALID_ARGUMENT;
		return -1;
	}
	if (!isValidString(val)){
		printInvalidLine(filename, lineNum);
		*msg = SP_CONFIG_INVALID_LINE;
		return -1;
	}
	if ( strcmp(val,"true")==0 ){
		return true;
	}
	else if ( strcmp(val,"true")==0 ){
		return false;
	}
	printInvalidLine(filename, lineNum);
	*msg = SP_CONFIG_INVALID_STRING;
	return -1;
}

SP_SPLIT_METHOD readEnum(char* val, const char* filename, int lineNum, SP_CONFIG_MSG* msg){
	if (val == NULL){
		printInvalidLine(filename, lineNum);
		*msg = SP_CONFIG_INVALID_ARGUMENT;
		return INVALID;
	}
	if (!isValidString(val)){
		printInvalidLine(filename, lineNum);
		*msg = SP_CONFIG_INVALID_STRING;
		return INVALID;
	}
	SP_SPLIT_METHOD method;
	if (strcmp(val,"MAX_SPREAD")==0){
		method = MAX_SPREAD;
		return method;
	}
	else if (strcmp(val,"RANDOM")==0){
		method = RANDOM;
		return method;
	}
	else if (strcmp(val,"INCREMENTAL")==0){
		method = INCREMENTAL;
		return method;
	}
	printInvalidLine(filename, lineNum);
	*msg = SP_CONFIG_INVALID_LINE;
	return INVALID;
}

bool isValidInt(char *str){
   // Handle empty string or negative "-"
   if (!*str || *str == '-'){
      return false;
		}
   //Check for non-digit chars in the rest of the string
   while (*str){
      if (!isdigit(*str)){
         return false;
      }
      else
         ++str;
   }
   return true;
}

bool isValidString(char *str){
   if (!*str){
      return false;
	}
   //Check for any spaces
   while (*str){
      if (isspace(*str) || (*str)=='#'){
         return false;
			 }
			 else{
         ++str;
   		}
	}
	return true;
}

bool spConfigIsExtractionMode(const SPConfig config, SP_CONFIG_MSG* msg){
	assert( msg != NULL );
	if (config == NULL){
		*msg = SP_CONFIG_INVALID_ARGUMENT;
		return NULL;
	}
	*msg = SP_CONFIG_SUCCESS;
	if (config->spExtractionMode==1){
		return true;
	}
	else return false;
}


bool spConfigMinimalGui(const SPConfig config, SP_CONFIG_MSG* msg){
	assert( msg != NULL );
	if (config == NULL){
		*msg = SP_CONFIG_INVALID_ARGUMENT;
		return NULL;
	}
	*msg = SP_CONFIG_SUCCESS;
	if (config->spMinimalGUI==1){
		return true;
	}
	else return false;
}

int spConfigGetNumOfImages(const SPConfig config, SP_CONFIG_MSG* msg){
	assert( msg != NULL );
	if (config == NULL){
		*msg = SP_CONFIG_INVALID_ARGUMENT;
		return -1;
	}

	int num = config->spNumOfImages;
	*msg = SP_CONFIG_SUCCESS;

	return num;
}

int spConfigGetNumOfFeatures(const SPConfig config, SP_CONFIG_MSG* msg){
	assert( msg != NULL );
	if (config == NULL){
		*msg = SP_CONFIG_INVALID_ARGUMENT;
		return -1;
	}

	int num = config->spNumOfFeatures;
  *msg = SP_CONFIG_SUCCESS;

	return num;
}


int spConfigGetPCADim(const SPConfig config, SP_CONFIG_MSG* msg){
	assert( msg != NULL );
	if (config == NULL){
		*msg = SP_CONFIG_INVALID_ARGUMENT;
		return -1;
	}

	int num = config->spPCADimension;
	*msg = SP_CONFIG_SUCCESS;

	return num;
}

//TODO - !
// SP_CONFIG_MSG spConfigGetImagePath(char* imagePath, const SPConfig config,int index){
// 	return SP_CONFIG_SUCCESS;
// }

//SP_CONFIG_MSG spConfigGetPCAPath(char* pcaPath, const SPConfig config){
//	return SP_CONFIG_SUCCESS;
//	//TODO
//}

void spConfigDestroy(SPConfig config){
	if (config == NULL){
		return;
	}
	free(config->spImagesDirectory);
	free(config->spImagesPrefix);
	free(config->spImagesSuffix);
	free(config->spPCAFilename);
	/*numOfImages is int
	*spPCADimension is int
	*spNumOfFeatures is int
	*spExtractionMode is bool and so Integer value - freed automatically
	*spNumOfSimilarImages is int
	*free(config->spKDTreeSplitMethod);
	*spKNN is int
	*spMinimalGUI is bool and so Integer value - freed automatically
	*spLoggerLevel is int
	*
	*/
	free(config->spLoggerFilename);
	free(config);
	return;
}
