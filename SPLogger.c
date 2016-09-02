#include "SPLogger.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

//File open mode
#define SP_LOGGER_OPEN_MODE "w"

// Global variable holding the logger
SPLogger logger = NULL;

struct sp_logger_t {
    FILE* outputChannel; //The logger file
    bool isStdOut; //Indicates if the logger is stdout
    SP_LOGGER_LEVEL level; //Indicates the level
};

SP_LOGGER_MSG spLoggerCreate(const char* filename, SP_LOGGER_LEVEL level) {
    if (logger != NULL) { //Already defined
        return SP_LOGGER_DEFINED;
    }
    logger = (SPLogger) malloc(sizeof(*logger));
    if (logger == NULL) { //Allocation failure
        return SP_LOGGER_OUT_OF_MEMORY;
    }
    logger->level = level; //Set the level of the logger
    if (filename == NULL) { //In case the filename is not set use stdout
        logger->outputChannel = stdout;
        logger->isStdOut = true;
    } else { //Otherwise open the file in write mode
        logger->outputChannel = fopen(filename, SP_LOGGER_OPEN_MODE);
        if (logger->outputChannel == NULL) { //Open failed
            free(logger);
            logger = NULL;
            return SP_LOGGER_CANNOT_OPEN_FILE;
        }
        logger->isStdOut = false;
    }
    return SP_LOGGER_SUCCESS;
}

void spLoggerDestroy() {
    if (!logger) {// If logger is NULL -> nothing to destroy
        return;
    }
    if (!logger->isStdOut) {//Close file only if not stdout
        fclose(logger->outputChannel);
    }
    free(logger);//free allocation
    logger = NULL;
}

/**
 * Writes to the destination the Warning/Error/Debug type message.
 *
 * @param msg       - The message to printed
 * @param file      - A string representing the filename in which spLoggerPrintWarning call occurred
 * @param function  - A string representing the function name in which spLoggerPrintWarning call ocurred
 * @param line		- A string representing the line in which the spLoggerPrintWarning call occurred
 * @return
 * SP_LOGGER_UNDIFINED          - If the logger is undefined
 * SP_LOGGER_INVAlID_ARGUMENT	- If any of msg or file or function are null or line is negative
 * SP_LOGGER_WRITE_FAIL			- If write failure occurred
 * SP_LOGGER_SUCCESS			- otherwise
 */
SP_LOGGER_MSG prntMessageAtCorrectLevel(const char* msg, const char* file,
                                        const char* function, const int line, const char* type, const SP_LOGGER_LEVEL level) {
    int print_result;
    if (!logger) return SP_LOGGER_UNDIFINED;
    if (!msg || !file || !function || line < 0) return SP_LOGGER_INVAlID_ARGUMENT;
    if (logger->level >= level) {
        print_result = fprintf(logger->outputChannel, "---%s---\n- file: %s\n- function: %s\n- line: %d\n- message: %s\n",
                               type, file, function, line, msg);
        if (print_result < 0) {// Writing failed
            return SP_LOGGER_WRITE_FAIL;
        }
    }
    return SP_LOGGER_SUCCESS;
}

SP_LOGGER_MSG spLoggerPrintError(const char* msg, const char* file,
                                 const char* function, const int line) {
    return prntMessageAtCorrectLevel(msg, file, function, line, "ERROR", SP_LOGGER_ERROR_LEVEL);
}

SP_LOGGER_MSG spLoggerPrintWarning(const char* msg, const char* file,
                                   const char* function, const int line) {
    return prntMessageAtCorrectLevel(msg, file, function, line, "WARNING", SP_LOGGER_WARNING_ERROR_LEVEL);
}

SP_LOGGER_MSG spLoggerPrintInfo(const char* msg) {
    int print_result;
    if (!logger) return SP_LOGGER_UNDIFINED;
    if (!msg) return SP_LOGGER_INVAlID_ARGUMENT;
    if (logger->level >= SP_LOGGER_INFO_WARNING_ERROR_LEVEL) {
        print_result = fprintf(logger->outputChannel, "---INFO---\n- message: %s\n",
                               msg);
        if (print_result < 0) {// Writing failed
            return SP_LOGGER_WRITE_FAIL;
        }
    }
    return SP_LOGGER_SUCCESS;
}

SP_LOGGER_MSG spLoggerPrintDebug(const char* msg, const char* file,
                                 const char* function, const int line) {
    return prntMessageAtCorrectLevel(msg, file, function, line, "DEBUG", SP_LOGGER_DEBUG_INFO_WARNING_ERROR_LEVEL);
}

SP_LOGGER_MSG spLoggerPrintMsg(const char* msg) {
    int print_result;
    if (!logger) return SP_LOGGER_UNDIFINED;
    if (!msg) return SP_LOGGER_INVAlID_ARGUMENT;
    print_result = fprintf(logger->outputChannel, "%s\n",
                           msg);
    if (print_result < 0) {// Writing failed
        return SP_LOGGER_WRITE_FAIL;
    }
    return SP_LOGGER_SUCCESS;
}
