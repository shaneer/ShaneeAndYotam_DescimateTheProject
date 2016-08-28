#include <stdbool.h>
#include <stdio.h>
#include "SPConfig.h"
#include <assert.h>
#include "SPLogger.h"

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

SPConfig spConfigCreate(const char* filename, SP_CONFIG_MSG* msg){
	assert( msg != NULL );
	FILE *fp;
	SPConfig res;
	char* temp;
	char* value;
	char* paramName;
	int lineNum;

	temp = (char*) malloc(CONFIG_LINE_MAX_SIZE+1);
		if (temp == NULL) {
			*msg = SP_CONFIG_ALLOC_FAIL;
			return NULL;
		}

	if((fp = fopen(filename, "r")) == NULL){
		*msg = SP_CONFIG_CANNOT_OPEN_FILE;
		free(temp);
		if (strcmp(filename, "spcbir.config")==0){			//Tells us program is using default config, and it's faulty
			printf(SP_CONFIG_FAULTY_DEFAULT);
		}
		return NULL;
	}

	res = (SPConfig) malloc(sizeof(*res));
	if (res == NULL) { 			//Allocation Fails
		free(temp);
		close(fp);
		*msg = SP_CONFIG_ALLOC_FAIL;
		return NULL;
	}

	//We have now succesfully allocated memory for our config struct res, opened our config_file
	// and allocated memory for temp. We begin by setting all the defaults.
	int defaults = setDefaults(res);
	if (defaults<0){
		*msg = SP_CONFIG_ALLOC_FAIL;
		free(temp);
		close(fp);
		return NULL;
	}

	//We now have a config object with all defaults set, we initialize line counter and begin to review file.
	int lineNum = 0;

	while(! feof(fp)) {
		fgets(temp, CONFIG_LINE_MAX_SIZE, fp);
		lineNum++;								//Count starts at 1

		//'Skips' over preceding whitespace
		while (isspace(*temp)){
			++temp;
		}
		//Skips comments and 'newline's
		if (*temp == '#'||*temp =='\n'||*temp =='\0') {		//Skips comments and 'newline's
			continue;
		}

		//Creates new string value which holds 'value to be assigned' portion of the line
		value = (char*) realloc(value, strlen(temp));
			if (value == NULL) {
				free(temp);
				spConfigDestroy(res);
				*msg = SP_CONFIG_ALLOC_FAIL;
				close(fp);
				return NULL;
			}
		strcpy(value, strchr(temp, '='));

		/*INVALID LINE since not long enough to be definition and we already skipped # and \n
		OR no '=' meaning it cannot be used as an assignment
		*/
		if (strlen(temp)<MIN_LENGTH_OF_CONFIG_LINE || value==NULL || strlen(value)<2) {	//value must have at least one char other then newline
			free(temp);
			free(value);
			spConfigDestroy(res);
			close(fp);
			*msg = SP_CONFIG_INVALID_LINE;
			return NULL;
		}
		
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

		paramName = (char*) realloc(paramName, strlen(temp)-strlen(value));
			if (paramName == NULL) {
				free(temp);
				free(value);
				spConfigDestroy(res);
				close(fp);
				*msg = SP_CONFIG_ALLOC_FAIL;
				return NULL;
			}
		//Copy param name, until first space or until '='
		int ind=0;
		while (!(isspace(temp[ind])||temp[ind]=='=')){
			paramName[ind]=temp[ind];
			ind++;
		}
		paramName[ind]='\0';

		//TEMPORARY VALUE FOR VALIDITY CHECKS
		int tempVal;

		//Check all paramNames
		if (strcmp(paramName, "spImagesDirectory")==0) {
			res->spImagesDirectory = readStr(value, filename, lineNum, msg);
			if (res->spImagesDirectory == NULL){
				terminateDuringParse(res, temp, paramName, value, fp, msg, *msg);
				return NULL;
			}
		}
		else if (strcmp(paramName, "spImagesPrefix")==0) {
			res->spImagesPrefix = readStr(value, filename, lineNum, msg);
			if (res->spImagesPrefix == NULL){
				terminateDuringParse(res, temp, paramName, value, fp, msg, *msg);
				return NULL;
			}
		}
		else if (strcmp(paramName, "spImagesSuffix")==0) {
			res->spImagesSuffix = readSuffix(value, filename, lineNum, msg);
			if (res->spImagesSuffix == NULL){
				terminateDuringParse(res, temp, paramName, value, fp, msg, *msg);
				return NULL;
			}
		}
		else if (strcmp(paramName, "spNumOfImages")==0) {
			tempVal = readInt(value, INT_MAX, 0, filename, lineNum, msg);
			if (tempVal == -1){
				terminateDuringParse(res, temp, paramName, value, fp, msg, *msg);
				return NULL;
			}else
			res->spNumOfImages = tempVal;
		}
		else if (strcmp(paramName, "spPCADimension")==0) {
			tempVal = readInt(value, 28, 10, filename, lineNum, msg);
			if (tempVal == -1){
				terminateDuringParse(res, temp, paramName, value, fp, msg, *msg);
				return NULL;
			}else
			res->spPCADimension = tempVal;
		}
		else if (strcmp(paramName, "spPCAFilename")==0) {
			res->spPCAFilename = readStr(value, filename, lineNum, msg);
			if (res->spPCAFilename == NULL){
				terminateDuringParse(res, temp, paramName, value, fp, msg, *msg);
				return NULL;
			}
		}
		else if (strcmp(paramName, "spNumOfFeatures")==0) {
			tempVal = readInt(value, INT_MAX, 0, filename, lineNum, msg);
			if (tempVal == -1){
				terminateDuringParse(res, temp, paramName, value, fp, msg, *msg);
				return NULL;
			}else
			res->spNumOfFeatures = tempVal;
		}
		else if (strcmp(paramName, "spExtractionMode")==0) {
			res->spExtractionMode = readBool(value, filename, lineNum, msg);
			if (res->spExtractionMode == NULL){
				terminateDuringParse(res, temp, paramName, value, fp, msg, *msg);
				return NULL;
			}
		}
		else if (strcmp(paramName, "spNumOfSimilarImages")==0) {
			tempVal = readInt(value, INT_MAX, 1, filename, lineNum, msg);
			if (tempVal == -1){
				terminateDuringParse(res, temp, paramName, value, fp, msg, *msg);
				return NULL;
			}else
			res->spNumOfSimilarImages = tempVal;
		}
		else if (strcmp(paramName, "spKDTreeSplitMethod")==0) {
			res->spKDTreeSplitMethod = readEnum(value, filename, lineNum, msg);
			if (res->spKDTreeSplitMethod == INVALID){
				terminateDuringParse(res, temp, paramName, value, fp, msg, *msg);
				return NULL;
			}
		}
		else if (strcmp(paramName, "spKNN")==0) {
			tempVal = readInt(value, INT_MAX, 1, filename, lineNum, msg);
			if (tempVal == -1){
				terminateDuringParse(res, temp, paramName, value, fp, msg, *msg);
				return NULL;
			}else
			res->spKNN = tempVal;
		}
		else if (strcmp(paramName, "spMinimalGUI")==0) {
			res->spMinimalGUI = readBool(value, filename, lineNum, msg);
			if (res->spMinimalGUI == NULL){
				terminateDuringParse(res, temp, paramName, value, fp, msg, *msg);
				return NULL;
			}
		}
		else if (strcmp(paramName, "spLoggerLevel")==0) {
			tempVal = readInt(value, 4, 1, filename, lineNum, msg);
			if (tempVal == -1){
				terminateDuringParse(res, temp, paramName, value, fp, msg, *msg);
				return NULL;
			}else
			res->spLoggerLevel = tempVal;
		}
		else if (strcmp(paramName, "spLoggerFilename")==0) {
			res->spLoggerFilename = readStr(value, filename, lineNum, msg);
			if (res->spLoggerFilename == NULL){
				terminateDuringParse(res, temp, paramName, value, fp, msg, *msg);
				return NULL;
			}
		}
		else {
		terminateDuringParse(res, temp, paramName, value, fp, msg, SP_CONFIG_INVALID_LINE);
		return NULL;
		}
	}

	int check = checkvalid(res, temp, paramName, value, fp, msg, filename, lineNum);
	if (check<0){
		return NULL;
	}
	*msg = SP_CONFIG_SUCCESS;
	fclose(fp);
	return res;
}

void terminateDuringParse(SPConfig res, char* temp, char* paramName, char* value, FILE *fp, SP_CONFIG_MSG* msg, SP_CONFIG_MSG out){
	spConfigDestroy(res);
	free(temp);
	free(paramName);
	free(value);
	fclose(fp);
	*msg = out;
	return;
}

int checkvalid(SPConfig res, char* temp, char* paramName, char* value, FILE *fp,
		SP_CONFIG_MSG* msg, char* filename, int lineNum){
	if (res == NULL){
		return -1;
	}
	if ( res->spImagesDirectory==NULL){
		printParamNotSet(filename, lineNum, "spImagesDirectory");
		terminateDuringParse(res, temp, paramName, value, fp, msg, SP_CONFIG_MISSING_DIR);
		return -1;
	}
	else if ( res->spImagesPrefix==NULL){
		printParamNotSet(filename, lineNum, "spImagesPrefix");
		terminateDuringParse(res, temp, paramName, value, fp, msg, SP_CONFIG_MISSING_PREFIX);
		return -1;
	}
	else if ( res->spImagesSuffix==NULL){
		printParamNotSet(filename, lineNum, "spImagesSuffix");
		terminateDuringParse(res, temp, paramName, value, fp, msg, SP_CONFIG_MISSING_SUFFIX);
		return -1;
	}
	else if ( res->spNumOfImages==NULL){
		printParamNotSet(filename, lineNum, "spNumOfImages");
		terminateDuringParse(res, temp, paramName, value, fp, msg, SP_CONFIG_MISSING_NUM_IMAGES);
		return -1;
	}
	return 0;
}

//PRINTING ERROR MESSAGES TO CONSOLE
void printConstraintsNotMet(char* filename, int lineNum){
	printf("File: %s\nLine: %d\nMessage: Invalid value - constraint not met",filename, lineNum );
}
void printInvalidLine(char* filename, int lineNum){
	printf("File: %s\nLine: %d\nMessage: Invalid configuration line",filename, lineNum );
}
//TODO
void printParamNotSet(char* filename, int lineNum, char* paramName){
	printf("File: %s\nLine: %d\nMessage: Parameter %s is not set",filename, lineNum, paramName );
}

int setDefaults(SPConfig config){
	//ALLOC MEM FOR DEFAULTS
	config->spPCAFilename = (char*) malloc(CONFIG_LINE_MAX_SIZE+1);
	if (config->spPCAFilename == NULL){
		spConfigDestroy(config);
		return -1;
	}
	config->spLoggerFilename = (char*) malloc(CONFIG_LINE_MAX_SIZE+1);
	if (config->spLoggerFilename == NULL){
		spConfigDestroy(config);
		return -1;
	}
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

	return 0;
	}

//HELPER FUNCTIONS TO LOAD DATA FROM STRING FORN IN CONFIG LINE
int readInt(char* value, int maxLength, int minLength, char* filename, int lineNum, SP_CONFIG_MSG* msg){
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

char* readStr(char* val, char* filename, int lineNum, SP_CONFIG_MSG* msg){
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

char* readSuffix(char* val, char* filename, int lineNum, SP_CONFIG_MSG* msg){
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

bool readBool(char* val, char* filename, int lineNum, SP_CONFIG_MSG* msg){				//TODO - can the user define bool as 0/1 does case matter True/true/tRue??
	if (val == NULL){
		printInvalidLine(filename, lineNum);
		*msg = SP_CONFIG_INVALID_ARGUMENT;
		return NULL;
	}
	if (!isValidString(val)){
		printInvalidLine(filename, lineNum);
		*msg = SP_CONFIG_INVALID_LINE;
		return NULL;
	}
	if ( strcmp(val,"true")==0 ){
		return true;
	}
	else if ( strcmp(val,"true")==0 ){
		return false;
	}
	printInvalidLine(filename, lineNum);
	*msg = SP_CONFIG_INVALID_STRING;
	return NULL;
}

SP_SPLIT_METHOD readEnum(char* val, char* filename, int lineNum, SP_CONFIG_MSG* msg){
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
   return true;
}

bool spConfigIsExtractionMode(const SPConfig config, SP_CONFIG_MSG* msg){
	assert( msg != NULL );
	if (config == NULL){
		msg = SP_CONFIG_INVALID_ARGUMENT;
		return NULL;
	}
	msg = SP_CONFIG_SUCCESS;
	if (config->spExtractionMode==1){
		return true;
	}
	else return false;
}


bool spConfigMinimalGui(const SPConfig config, SP_CONFIG_MSG* msg){
	assert( msg != NULL );
	if (config == NULL){
		msg = SP_CONFIG_INVALID_ARGUMENT;
		return NULL;
	}
	msg = SP_CONFIG_SUCCESS;
	if (config->spMinimalGUI==1){
		return true;
	}
	else return false;
}

int spConfigGetNumOfImages(const SPConfig config, SP_CONFIG_MSG* msg){
	assert( msg != NULL );
	if (config == NULL){
		msg = SP_CONFIG_INVALID_ARGUMENT;
		return NULL;
	}

	int num = config->spNumOfImages;
	msg = SP_CONFIG_SUCCESS;

	return num;
}

int spConfigGetNumOfFeatures(const SPConfig config, SP_CONFIG_MSG* msg){
	assert( msg != NULL );
	if (config == NULL){
		msg = SP_CONFIG_INVALID_ARGUMENT;
		return NULL;
	}

	int num = config->spNumOfFeatures;
	msg = SP_CONFIG_SUCCESS;

	return num;
}


int spConfigGetPCADim(const SPConfig config, SP_CONFIG_MSG* msg){
	assert( msg != NULL );
	if (config == NULL){
		msg = SP_CONFIG_INVALID_ARGUMENT;
		return NULL;
	}

	int num = config->spPCADimension;
	msg = SP_CONFIG_SUCCESS;

	return num;
}

//TODO - !
SP_CONFIG_MSG spConfigGetImagePath(char* imagePath, const SPConfig config,int index){
	SP_CONFIG_MSG* msg;
	msg = (SP_CONFIG_MSG*) malloc(sizeof(*msg));

	//TODO - necessary?
	if (msg == NULL){
		return SP_CONFIG_ALLOC_FAIL;
	}
	if (config == NULL || imagePath==NULL){
		*msg = SP_CONFIG_INVALID_ARGUMENT;
		return *msg;
	}
	if(index > spConfigGetNumOfImages(config, msg)){
		*msg = SP_CONFIG_INDEX_OUT_OF_RANGE;
		return *msg;
	}

	//spImagesDirectory = "./images/"
	//spImagesPrefix = "img"
	//spImagesSuffix = ".png"
	//spNumOfImages = 17
	char* directory = config->spImagesDirectory;
	char* prefix = config->spImagesPrefix;
	char* suffix = config->spImagesSuffix;
	char* indexStr = malloc(MAX_INT_LEN);
	//TODO ALLOCATE MEMORY

	snprintf(indexStr, MAX_INT_LEN, "%d", index);
	//CAN index be larger than sizeof() = 4?

	int sizeInMem = sizeof(config->spImagesDirectory)+sizeof(config->spImagesPrefix)+sizeof(indexStr)+10;
	imagePath = realloc(imagePath, sizeInMem);

	//Check Alloc Fail
	if (imagePath==NULL || sizeof(imagePath) < sizeInMem-1){
		*msg = SP_CONFIG_ALLOC_FAIL;
		return *msg;
	}

	imagePath = memcpy(imagePath, directory, strlen(directory)+1);
	imagePath = strncat(imagePath, prefix, strlen(prefix)+1);
	imagePath = strncat(imagePath, indexStr, strlen(indexStr)+1);
	imagePath = strncat(imagePath, suffix, strlen(suffix)+1);

	return SP_CONFIG_SUCCESS;
}

SP_CONFIG_MSG spConfigGetPCAPath(char* pcaPath, const SPConfig config){
	//TODO
}

void spConfigDestroy(SPConfig config){
	if (config == NULL){
		return;
	}
	if (config->spImagesDirectory != NULL){
		free(config->spImagesDirectory);
	}
	if (config->spImagesPrefix != NULL){
		free(config->spImagesPrefix);
	}
	if (config->spImagesSuffix != NULL){
		free(config->spImagesSuffix);
	}
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
