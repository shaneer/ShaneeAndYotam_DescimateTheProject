#include <stdbool.h>
#include <stdio.h>
#include "SPConfig.h"
#include "SPLogger.h"

typedef struct sp_config_t{
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
	int lineCount;

	temp = (char*) malloc(CONFIG_LINE_MAX_SIZE);
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
	if (res == NULL) { //Allocation Fails
		free(temp);
		free(value);
		*msg = SP_CONFIG_ALLOC_FAIL;
		return NULL;
	}

	setDefaults(res);
	lineCount = 0;

	while(! feof(fp)) {
		fgets(temp, CONFIG_LINE_MAX_SIZE, fp);
		lineCount++;

		//'Skips' over preceding whitespace
		while ((*temp == ' ')||(*temp == '\t')||(*temp == '\v')){
			++temp;
		}
		//Skips comments and 'newline's
		if (temp[0] == '#'||*temp =='\n') {		//Skips comments and 'newline's
			continue;
		}
		//Creates new string value which holds value
		value = (char*) realloc(value, strlen(temp));
			if (value == NULL) {
				free(temp);
				free(res);
				*msg = SP_CONFIG_ALLOC_FAIL;
				return NULL;
			}
		value = strchr(temp, '=');

		/*INVALID LINE since not long enough to be definition and we already skipped # and \n
		OR no '=' meaning it cannot be used as an assignment
		*/
		if (strlen(temp)<MIN_LENGTH_OF_CONFIG_LINE || value==NULL || strlen(value)<2) {	//value must have at least one char other then newline
			free(temp);
			free(res);
			*msg = SP_CONFIG_INVALID_LINE;
			return NULL;
		}
		//We now have temp which we will use to discover which param to use, and value which starts at =
		++value;
		while ((*value == ' ')||(*value == '\t')||(*value == '\v')){
			++value;
		}
		//Remove spaces at end
		int end = strlen(value)-1;
		while (isspace(value[end])||(value[end] == '\n')){
			--end;
		}
		value[end+1]='\0';

		paramName = (char*) realloc(paramName, strlen(temp)-strlen(value));
			if (paramName == NULL) {
				free(temp);
				free(value);
				free(res);
				*msg = SP_CONFIG_ALLOC_FAIL;
				return NULL;
			}

		char curr = temp[0];
		int i=0;
		while (!(curr == ' '||curr == '\n'||curr == '\t'||curr == '\v'||curr=='=')){
			paramName[i]=curr;
			curr=temp[i+1];
			i++;
		}
		//TEMPORARY VALUE FOR VALIDITY CHECKS
		int tempVal;

		if (strstr(paramName, "spImagesDirectory")) {

		}
		elif (strstr(paramName, "spImagesPrefix")) {

		}
		elif (strstr(paramName, "spImagesSuffix")) {

		}
		elif (strstr(paramName, "spNumOfImages")) {
			tempVal = readInt(value, INT_MAX, 0);
			if (tempVal == -1){
				return NULL;
			}else
			config->spNumOfImages = tempVal;
		}
		elif (strstr(paramName, "spPCADimension")) {
			tempVal = readInt(value, 28, 10);
			if (tempVal == -1){
				return NULL;
			}else
			config->spPCADimension = tempVal;
		}
		elif (strstr(paramName, "spPCAFilename")) {
		}
		elif (strstr(paramName, "spNumOfFeatures")) {
			tempVal = readInt(value, INT_MAX, 0);
			if (tempVal == -1){
				return NULL;
			}else
			config->spNumOfFeatures = tempVal;
		}
		elif (strstr(paramName, "spExtractionMode")) {

		}
		elif (strstr(paramName, "spNumOfSimilarImages")) {
			temp = readInt(value, INT_MAX, 1);
			if (temp == -1){
				return NULL;
			}else
			config->spNumOfSimilarImages = temp;
		}
		elif (strstr(paramName, "spKDTreeSplitMethod")) {
		}
		elif (strstr(paramName, "spKNN")) {
			temp = readInt(value, INT_MAX, 1);
			if (temp == -1){
				return NULL;
			}else
			config->spKNN = tempVal;
		}
		elif (strstr(paramName, "spMinimalGUI")) {
		}
		elif (strstr(paramName, "spLoggerLevel")) {
			temp = readInt(value, 4, 1);
			if (temp == -1){
				return NULL;
			}else
			config->spLoggerLevel = tempVal;
		}
		elif (strstr(paramName, "spLoggerFilename")) {
		}
		else {
		terminateDuringParse(msg, SP_CONFIG_INVALID_ARGUMENT);
		return NULL;
		}
	}

	int check = checkvalid(res);
	if (check<0){
		return NULL;
	}

	*msg = SP_CONFIG_SUCCESS;
	fclose(fp);
	free(fp);
	return res;
}

void terminateDuringParse(SP_CONFIG_MSG* msg, SP_CONFIG_MSG print){
	destroy(res);
	free (res);
	free(temp);
	free(paramName);
	free(value);
	fclose(fp);
	free(fp);
	*msg = print;
	return;
}

int checkvalid(SP_CONFIG_MSG* msg, SPConfig res){
	if (res == NULL){
		return -1;
	}
	if ( res->spImagesDirectory==NULL){
		printParamNotSet(filename, lineNum, "spImagesDirectory");
		terminateDuringParse(msg, SP_CONFIG_MISSING_DIR);
		return -1;
	}
	elif ( res->spImagesPrefix==NULL){
		printParamNotSet(filename, lineNum, "spImagesPrefix");
		terminateDuringParse(msg, SP_CONFIG_MISSING_PREFIX);
		return -1;
	}
	elif ( res->spImagesSuffix==NULL){
		printParamNotSet(filename, lineNum, "spImagesSuffix");
		terminateDuringParse(msg, SP_CONFIG_MISSING_SUFFIX);
		return -1;
	}
	elif ( res->spNumOfImages==NULL){
		printParamNotSet(filename, lineNum, "spNumOfImages");
		terminateDuringParse(msg, SP_CONFIG_MISSING_NUM_IMAGES);
		return -1;
	}
}

printConstraintsNotMet(char* filename, int lineNum){
	printf(“File: %s\nLine: %d\nMessage: Invalid value - constraint not met”, filename, lineNum);
}
printInvalidLine(char* filename, int lineNum){
	printf(“File: %s\nLine: %d\nMessage: Invalid configuration line” , filename, lineNum);
}
printParamNotSet(char* filename, int lineNum, char* paramName){
	printf(“File: %s\nLine: %d\nMessage: Parameter %s is not set” , filename, lineNum, paramName);
}

int setDefaults(SPConfig config){
	//ALLOC MEM FOR DEFAULTS
	config->spPCAFilename = (char*) malloc(sizeof(char)*8);
	if (config->spPCAFilename == NULL){
		free(config);
		return NULL;
	}
	config->spLoggerFilename = (char*) malloc(sizeof(char)*7);
	if (config->spLoggerFilename == NULL){
		free(config);
		return NULL;
	}
	config->spPCADimension = 20;
	config->spPCAFilename = "pca.yml";
	config->spNumOfFeatures = 100;
	config->spExtractionMode = 1;
	config->spNumOfSimilarImages = 1;
	config->spKDTreeSplitMethod = MAX_SPREAD;
	config->spKNN = 1;
	config->spMinimalGUI = 0;
	config->spLoggerLevel = 3;
	config->spLoggerFilename = "stdout";

	return 1;
	}

int readInt(char* config_line, int maxLength, int minLength){
	if (isValidInt(config_line)){
		int num = atoi(config_line);
	}else{
		printInvalidLine(filename, lineCount);
		terminateDuringParse(msg, SP_CONFIG_INVALID_ARGUMENT);
		return -1;
	}
	if( minLength<=num && num<=maxLength ){
		return num;
	}else {
		printConstraintsNotMet(filename, lineCount);
		terminateDuringParse(msg, SP_CONFIG_INVALID_INTEGER);
		return -1;
	}
}
//TODO
void readStr(char* config_line, char* val){
    char param[maxLength];
    sscanf(config_line, "%s %s\n", param, val);
}
//TODO
void readSuffix(char* config_line, char* val){
    char param[maxLength];
    sscanf(config_line, "%s %s\n", param, val);
}

bool readBool(char* config_line){}
int readEnum(char* config_line){}

bool isValidInt(char *str){
   // Handle empty string or negative "-"
   if (!*str || *str == '-'){
      return false;
		}
   //Check for non-digit chars in the rest of the string
   while (*str){
      if (!isdigit(*str))
         return false;
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
      if ((*str)==' ' || (*str)=='#'){
         return false;
			}else
         ++str;
   }
   return true;
}

/*
 * Returns true if spExtractionMode = true, false otherwise.
 * @param config - the configuration structure
 * @assert msg != NULL
 * @param msg - pointer in which the msg returned by the function is stored
 * @return true if spExtractionMode = true, false otherwise.
 *
 * - SP_CONFIG_INVALID_ARGUMENT - if config == NULL
 * - SP_CONFIG_SUCCESS - in case of success
 */
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
	if (!config->spImagesDirectory == NULL){
		free(config->spImagesDirectory);
	}
	if (!config->spImagesPrefix == NULL){
		free(config->spImagesPrefix);
	}
	if (!config->spImagesSuffix == NULL){
		free(config->spImagesSuffix);
	}
	//numOfImages is int
	//spPCADimension is int
	free(config->spPCAFilename);
	//spNumOfFeatures is int
	//spExtractionMode is bool and so Integer value - freed automatically
	//spNumOfSimilarImages is int
	//free(config->spKDTreeSplitMethod);

	//spKNN is int
	//spMinimalGUI is bool and so Integer value - freed automatically
	//spLoggerLevel is int

	free(config->spLoggerFilename);

	free(config);
}
