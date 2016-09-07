#ifndef SPCONFIG_H_
#define SPCONFIG_H_

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "SPLogger.h"

#define CONFIG_LINE_MAX_SIZE 1024
#define MAX_SUFFIX_LEN 5
#define MIN_NUM_OF_IMGS 0
#define MIN_NUM_OF_FEATURES 0
#define MIN_DIM 10
#define MAX_DIM 28
#define MIN_SIMILAR_IMGS 0
#define MIN_KNN 0
#define MIN_LENGTH_OF_CONFIG_LINE 8
#define MAX_INT_LEN 12

/**
*Enum Type to define a messag returned to the user, allowing us to glean more information about
*the program's executiong, especially in case of failure.
*
 */
typedef enum sp_config_msg_t {
	SP_CONFIG_MISSING_DIR,				//The Non-Default field Image Directory is missing
	SP_CONFIG_MISSING_PREFIX,			//The Non-Default field Prefix is missing
	SP_CONFIG_MISSING_SUFFIX,			//The Non-Default field Suffix is missing
	SP_CONFIG_MISSING_NUM_IMAGES,		//The Non-Default field numOfImages is missing
	SP_CONFIG_CANNOT_OPEN_FILE,			//The Filepath given for the config file cannot be opened
	SP_CONFIG_INVALID_FILE,				//Invalid config file given
	SP_CONFIG_INVALID_LINE,				//Invalid line in config file: not a comment, assignment or newline
	SP_CONFIG_ALLOC_FAIL,
	SP_CONFIG_INVALID_INTEGER,
	SP_CONFIG_INVALID_STRING,
	SP_CONFIG_INVALID_VALUE,			//Invalid value given to variable - doesn't match param constraints
	SP_CONFIG_INVALID_ARGUMENT,		//Invalid argument type
	SP_CONFIG_INDEX_OUT_OF_RANGE,
	SP_CONFIG_SUCCESS
} SP_CONFIG_MSG;


/**
 * Enum Type to define the Spread by which the tree will be built
 *
 * MAX_SPREAD : Spread of the ith dimension is the difference between the max and min of the 𝑖𝑡ℎ coordinate of all points
 * RANDOM : A random dim is chosen
 * INCREMENTAL : Each time spread increments by one, circuling back to 0 when it reaches highest dimension
 * INVALID : Set when the value given cannot be read as one of the valid spreads
 *
 */
typedef enum sp_split_method_t{
	MAX_SPREAD, RANDOM, INCREMENTAL, INVALID
}SP_SPLIT_METHOD;

typedef struct sp_config_t* SPConfig;

/**
 * Creates a new system configuration struct. The configuration struct
 * is initialized based on the configuration file given by 'filename'.
 *
 * @param filename - the name of the configuration file
 * @assert msg != NULL
 * @param msg - pointer in which the msg returned by the function is stored
 * @return NULL in case an error occurs. Otherwise, a pointer to a struct which
 * 		   contains all system configuration.
 *
 * The resulting value stored in msg is as follow:
 * - SP_CONFIG_INVALID_ARGUMENT - if filename == NULL
 * - SP_CONFIG_CANNOT_OPEN_FILE - if the configuration file given by filename cannot be open
 * - SP_CONFIG_ALLOC_FAIL - if an allocation failure occurred
 * - SP_CONFIG_INVALID_INTEGER - if a line in the config file contains invalid integer
 * - SP_CONFIG_INVALID_STRING - if a line in the config file contains invalid string
 * - SP_CONFIG_MISSING_DIR - if spImagesDirectory is missing
 * - SP_CONFIG_MISSING_PREFIX - if spImagesPrefix is missing
 * - SP_CONFIG_MISSING_SUFFIX - if spImagesSuffix is missing
 * - SP_CONFIG_MISSING_NUM_IMAGES - if spNumOfImages is missing
 * - SP_CONFIG_SUCCESS - in case of success
 * - SP_CONFIG_INVALID_LINE -  in case a line with a mistake is given
 *
 *
 */
SPConfig spConfigCreate(const char* filename, SP_CONFIG_MSG* msg);


 // PRINT FUNCTIONS since logger is not yet initiialized

void printConstraintsNotMet(const char* filename, int numOfValue);

void printInvalidLine(const char* filename, int lineNum);

void printParamNotSet(const char* filename, int lineNum, char* paramName);

/**
 * A function to set the default fields of the config struct
 * @param config - the config struct for which we set default fields
 * @assert msg != NULL
 * @param msg - pointer in which the msg is stored used to indicate success
 * @return viod
 */
void setDefaults(SPConfig config);
/**
 * A Helper function for the loading of data. Used to read an argument given to check its validity and convert it
 * from a string to the correct int, checking it meets the proper constraints
 *
 * @param value -  the part of the line (string) that contains the value to be assigned to a param expecting an int
 * @param - maxLength / minLength - the constraints that need to be met for the  param to be assigned
 * @param - filename, lineNum, and numOfVale are used for printing error or warning messages to the user in case of failure or
 *					in case constraints are not met (invvalid value given)
 * @param msg - pointer in which the msg is stored used to indicate success
 * @return - int to be stored as value of a certain field of the struct
 */
int readInt(char* value, int maxLength, int minLength, const char* filename, int lineNum, SP_CONFIG_MSG* msg, int numOfValue);
/**
 * A Helper function for the loading of data. Used to read an argument given to check its validity
 *
 * @param value -  the part of the line (string) that contains the value to be assigned to a param expecting a string, needs to be without spaces.
 * @param - filename, lineNum, and numOfVale are used for printing error or warning messages to the user in case of failure or
 *					in case constraints are not met (invvalid value given)
 * @param msg - pointer in which the msg is stored used to indicate success
 * @return char* (string value) to be assigned to a certain field of the struct
 */
char* readStr(char* val, const char* filename, int lineNum, SP_CONFIG_MSG* msg);


/**
 * A Helper function for the loading of data. Used to read an argument given to check
 * its validity as one of the four optional suffixes
 *
 * @param value -  the part of the line (string) that contains the value to be assigned to a param expecting a string, needs to be without spaces.
 * @param - filename, lineNum, and numOfVale are used for printing error or warning messages to the user in case of failure or
 *					in case constraints are not met (invvalid value given)
 * @param msg - pointer in which the msg is stored used to indicate success
 * @return char* (string value) to be assigned to a the suffix field of the struct
 */
char* readSuffix(char* val, const char* filename, int lineNum, SP_CONFIG_MSG* msg);

/**
 * A Helper function for the loading of data. Used to read an argument given to check its validity as a boolean
 * Only "true" or "false" are accepted.
 *
 * @param value -  the part of the line (string) that contains the value to be assigned to a param expecting a string, needs to be without spaces.
 * @param - filename, lineNum, and numOfVale are used for printing error or warning messages to the user in case of failure or
 *					in case constraints are not met (invvalid value given)
 * @param msg - pointer in which the msg is stored used to indicate success
 * @return - a boolean value to be assigned to a certain field of the struct
 *
 */
bool readBool(char* val, const char* filename, int lineNum, SP_CONFIG_MSG* msg);
/**
 * A Helper function for the loading of data. Used to read an argument given to check its validity as SPLIT_METHOD enum
 * Only one of the 4 predefined enum values defined above are accepted.
 *
 * @param value -  the part of the line (string) that contains the value to be assigned to a param expecting a string, needs to be without spaces.
 * @param - filename, lineNum, and numOfVale are used for printing error or warning messages to the user in case of failure or
 *					in case constraints are not met (invvalid value given)
 * @param msg - pointer in which the msg is stored used to indicate success
 * @return - a SP_SPLIT_METHOD value to be assigned to a certain field of the struct
 *
 */
SP_SPLIT_METHOD readEnum(char* val, const char* filename, int lineNum, SP_CONFIG_MSG* msg);

/*
* Helper Functions meant to check the validity of either an int or a string.
* Ensure there are no spaces and that the value returned is one that could be a potential value for our param's assignment.
* @param -  char* : a string to be checked
* @return - true if the value can be used,
*						false otherwise.
*/
bool isValidInt(char *str);
bool isValidString(char *str);

/*
 * Returns true if spExtractionMode = true, false otherwise.
 *
 * @param config - the configuration structure
 * @assert msg != NULL
 * @param msg - pointer in which the msg returned by the function is stored
 * @return true if spExtractionMode = true, false otherwise.
 *
 * - SP_CONFIG_INVALID_ARGUMENT - if config == NULL
 * - SP_CONFIG_SUCCESS - in case of success
 */
bool spConfigIsExtractionMode(const SPConfig config, SP_CONFIG_MSG* msg);

/*
 * Returns true if spMinimalGUI = true, false otherwise.
 *
 * @param config - the configuration structure
 * @assert msg != NULL
 * @param msg - pointer in which the msg returned by the function is stored
 * @return true if spExtractionMode = true, false otherwise.
 *
 * - SP_CONFIG_INVALID_ARGUMENT - if config == NULL
 * - SP_CONFIG_SUCCESS - in case of success
 */
bool spConfigMinimalGui(const SPConfig config, SP_CONFIG_MSG* msg);

/*
 * Returns the number of images set in the configuration file, i.e the value
 * of spNumOfImages.
 *
 * @param config - the configuration structure
 * @assert msg != NULL
 * @param msg - pointer in which the msg returned by the function is stored
 * @return positive integer in success, negative integer otherwise.
 *
 * - SP_CONFIG_INVALID_ARGUMENT - if config == NULL
 * - SP_CONFIG_SUCCESS - in case of success
 */
int spConfigGetNumOfImages(const SPConfig config, SP_CONFIG_MSG* msg);

/*
 * Returns the number of features to be extracted. i.e the value
 * of spNumOfFeatures.
 *
 * @param config - the configuration structure
 * @assert msg != NULL
 * @param msg - pointer in which the msg returned by the function is stored
 * @return positive integer in success, negative integer otherwise.
 *
 * - SP_CONFIG_INVALID_ARGUMENT - if config == NULL
 * - SP_CONFIG_SUCCESS - in case of success
 */
int spConfigGetNumOfFeatures(const SPConfig config, SP_CONFIG_MSG* msg);

/**
 * Returns the dimension of the PCA. i.e the value of spPCADimension.
 *
 * @param config - the configuration structure
 * @assert msg != NULL
 * @param msg - pointer in which the msg returned by the function is stored
 * @return positive integer in success, negative integer otherwise.
 *
 * - SP_CONFIG_INVALID_ARGUMENT - if config == NULL
 * - SP_CONFIG_SUCCESS - in case of success
 */
int spConfigGetPCADim(const SPConfig config, SP_CONFIG_MSG* msg);


/*
* GETTER for Image Directory
* @param  -the config from which we want the field.
* @return a string giving us the field ImageDirectory of our struct
*/
char* spConfigGetImageDirectory(const SPConfig config, SP_CONFIG_MSG* msg);

/**
 * Given an index 'index' the function stores in imagePath the full path of the
 * ith image.
 *
 * For example:
 * Given that the value of:
 *  spImagesDirectory = "./images/"
 *  spImagesPrefix = "img"
 *  spImagesSuffix = ".png"
 *  spNumOfImages = 17
 *  index = 10
 *
 * The functions stores "./images/img10.png" to the address given by imagePath.
 * Thus the address given by imagePath must contain enough space to
 * store the resulting string.
 *
 * @param imagePath - an address to store the result in, it must contain enough space.
 * @param config - the configuration structure
 * @param index - the index of the image.
 *
 * @return
 * - SP_CONFIG_INVALID_ARGUMENT - if imagePath == NULL or config == NULL
 * - SP_CONFIG_INDEX_OUT_OF_RANGE - if index >= spNumOfImages
 * - SP_CONFIG_SUCCESS - in case of success
 */
SP_CONFIG_MSG spConfigGetImagePath(char* imagePath, const SPConfig config,int index);

/**
 * The function stores in pcaPath the full path of the pca file.
 * For example given the values of:
 *  spImagesDirectory = "./images/"
 *  spPcaFilename = "pca.yml"
 *
 * The functions stores "./images/pca.yml" to the address given by pcaPath.
 * Thus the address given by pcaPath must contain enough space to
 * store the resulting string.
 *
 * @param imagePath - an address to store the result in, it must contain enough space.
 * @param config - the configuration structure
 * @return
 *  - SP_CONFIG_INVALID_ARGUMENT - if imagePath == NULL or config == NULL
 *  - SP_CONFIG_SUCCESS - in case of success
 */
SP_CONFIG_MSG spConfigGetPCAPath(char* pcaPath, const SPConfig config);

/**
 * Frees all memory resources associate with config.
 * If config == NULL nothig is done.
 */
void spConfigDestroy(SPConfig config);

#endif /* SPCONFIG_H_ */
