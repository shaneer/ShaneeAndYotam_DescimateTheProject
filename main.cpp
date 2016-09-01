/*
 *  Main file
 *  Author: Yotam
 */

#include <cstdlib>
#include <cassert>
#include <cstring>
#include <cstdio>
#include <cstdbool>
#include "SPImageProc.h"

extern "C" {
#include "SPConfig.h"
#include "SPImageExtraction.h"
#include "SPPoint.h"
#include "SPLogger.h"
#include "SPKDTree.h"
#include "SPKDArray.h"
#include "SPBPQueue.h"
#include "main_aux.c"
}

using namespace sp;

int main(int argc, char const *argv[]) {
    const char* filename, *queryMsg = "Please enter an image path:\n";
    int i, j, numOfFeats, featsCount = 0, numOfImages, queryCount = 0;
    char* imagePath, *featsPath, *loggerPath, *queryPath = NULL;
    ImageProc* proc = NULL;
    SPPoint* featsT, *feats;
    SPKDArray kdarr;
    SPKDTreeNode tree;
    bool extraction_mode;
    SP_SPLIT_METHOD split_method;
    SP_LOGGER_LEVEL loggerLevel;
    SP_EXTRACTION_MSG ext_msg;
    SP_CONFIG_MSG conf_msg;;
    SP_LOGGER_MSG log_msg;

    if (argc == 1) {
        filename = "spcbir.config";
    } else if (argc == 3 && strcmp(argv[1],"-c") == 0 && strlen(argv[2]) > 0) {
        filename = argv[2];
    } else {
        printf("Invalid command line : use -c <config_filename>\n");
        exit(0);
    }
    // at this point the config file path is available

    SPConfig conf = spConfigCreate(filename, &conf_msg);
    if (conf_msg == SP_CONFIG_CANNOT_OPEN_FILE) {
        if (argc == 3) {
            // filename entered
            printf("The configuration file %s couldn’t be open\n", filename);
        } else {
            // default file
            printf("The default configuration file spcbir.config couldn’t be open\n");
        }
        exit(0);
    }

    // get attributes from config file for program use
    numOfImages = spConfigGetNumOfImages(conf, &conf_msg);
    if (conf_msg != SP_CONFIG_SUCCESS) {
        // TODO: Error
    }
    numOfFeats = spConfigGetNumOfFeatures(conf, &conf_msg);
    if (conf_msg != SP_CONFIG_SUCCESS) {
        // TODO: Error
    }
    extraction_mode = spConfigIsExtractionMode(conf, &conf_msg);
    if (conf_msg != SP_CONFIG_SUCCESS) {
        // TODO: Error
    }
    split_method = spConfigGetSplitMethod(conf, &conf_msg);
    if (conf_msg != SP_CONFIG_SUCCESS) {
        // TODO: Error
    }
    loggerPath = spConfigGetLoggerFilename(conf, &conf_msg);
    if (conf_msg != SP_CONFIG_SUCCESS) {
        // TODO: Error
    }
    loggerLevel = spConfigGetLoggerLevel(conf, &conf_msg);
    if (conf_msg != SP_CONFIG_SUCCESS) {
        // TODO: Error
    }
    log_msg = spLoggerCreate(loggerPath, loggerLevel);
    if (log_msg != SP_LOGGER_SUCCESS) {
        // TODO: Error
    }
    // Logger created path can be freed
    free(loggerPath);

    // create a big enough SPPoint array for all features from all images.
    // the spNumOfFeatures attricute from the config file states the MAX num of features per image,
    // but since opencv sometimes extracts (spNumOfFeatures+1) features, it's been addad.
    feats = (SPPoint*)calloc(numOfImages*(numOfFeats + 1), sizeof(SPPoint));
    queryPath = (char*)calloc(CONFIG_LINE_MAX_SIZE+1, sizeof(char));
    if (feats == NULL || queryPath == NULL) {
        // TODO: Error
        freeSPPointsArray(feats, numOfFeats);
        free(queryPath);
        spConfigDestroy(conf);
        spLoggerDestroy();
        exit(0);
    }

    if (extraction_mode) {
        printf("1-a. Extraction mode\n");
        // Extract mode
        proc = new ImageProc(conf); // staticlly creat ImageProc ovject (delete not needed)
        imagePath = (char*)malloc(1+CONFIG_LINE_MAX_SIZE*4); // create a big enough buffer
        featsPath = (char*)malloc(1+CONFIG_LINE_MAX_SIZE*4); // create a big enough buffer

        if (!proc || !imagePath || !featsPath) {
            // TODO: Logger
            free(imagePath);
            free(featsPath);
            freeSPPointsArray(feats, numOfFeats);
            free(queryPath);
            delete proc;
            spConfigDestroy(conf);
            spLoggerDestroy();
            exit(0);
        }
        printf("\t- Allocated memory\n");
        // go over all pictures and store data in file and in the feats array
        for (i = 0; i < numOfImages; i++) {
            conf_msg = spConfigGetImagePath(imagePath, conf, i);
            // TODO: Logger
            if (conf_msg != SP_CONFIG_SUCCESS) {
                free(imagePath);
                free(featsPath);
                free(queryPath);
                delete proc;
                spConfigDestroy(conf);
                spLoggerDestroy();
                freeSPPointsArray(feats, numOfFeats);
                exit(0);
            }
            conf_msg = SPConfigGetFeaturesFilePath(featsPath, conf, i);
            // TODO: Logger
            if (conf_msg != SP_CONFIG_SUCCESS) {
                free(imagePath);
                free(featsPath);
                free(queryPath);
                delete proc;
                spConfigDestroy(conf);
                spLoggerDestroy();
                freeSPPointsArray(feats, numOfFeats);
                exit(0);
            }
            featsT = proc->getImageFeatures(imagePath, i, &numOfFeats); // get features for image
            ext_msg = spExtractFromImage(featsT, numOfFeats, i, featsPath); // save features to file
            if (ext_msg != SP_EXTRACTION_SUCCESS) {
                // TODO: Logger
                free(imagePath);
                free(featsPath);
                free(queryPath);
                delete proc;
                spConfigDestroy(conf);
                spLoggerDestroy();
                freeSPPointsArray(feats, numOfFeats);
                exit(0);
            }
            // move features to generall array and destroy the temp array
            for (j=0; j<numOfFeats; j++) {
                feats[featsCount] = featsT[j];
                featsCount++;
            }
            free(featsT);
            // TODO: Logger
            printf("\t- Processed %s\n", imagePath);
        }
        free(imagePath);
        free(featsPath);
        delete proc;
        // TODO: Logger
    } else {
        // Non-Extract mode
        printf("1-b. Non-Extraction Mode:\n");
        featsPath = (char*)malloc(1+CONFIG_LINE_MAX_SIZE*4); // create a big enough buffer
        if (!featsPath) {
            // TODO: Logger
            free(featsPath);
            freeSPPointsArray(feats, numOfFeats);
            free(queryPath);
            spConfigDestroy(conf);
            spLoggerDestroy();
            exit(0);
        }
        printf("\t- Allocated memory\n");
        for (i = 0; i < numOfImages; i++) {
            conf_msg = SPConfigGetFeaturesFilePath(featsPath, conf, i);
            // TODO: Logger
            if (conf_msg != SP_CONFIG_SUCCESS) {
                free(featsPath);
                free(queryPath);
                spConfigDestroy(conf);
                spLoggerDestroy();
                freeSPPointsArray(feats, numOfFeats);
                exit(0);
            }
            featsT = spExtractFromFiles(featsPath, &numOfFeats, &ext_msg);
            if (ext_msg != SP_EXTRACTION_SUCCESS) {
                // TODO: Logger
                free(featsPath);
                free(queryPath);
                spConfigDestroy(conf);
                spLoggerDestroy();
                freeSPPointsArray(feats, numOfFeats);
                exit(0);
            }
            for (j=0; j<numOfFeats; j++) {
                feats[featsCount] = featsT[j];
                featsCount++;
            }
            free(featsT);
            // TODO: Logger
            printf("\t- Processed %s\n", featsPath);
        }
        free(featsPath);
        // TODO: Logger
    }
    printf("2. Finished processing - %d features\n", featsCount);
    kdarr = Init(feats, featsCount); // no needto to destroy KDArray (handles in tree creation)

    // at this point no neef ro the SPPoint array (feats)
    freeSPPointsArray(feats, numOfFeats);

    printf("3. KDArray created\n");
    tree = spKDTreeCreate(kdarr, split_method);
    printf("4. KDTree created\n");

    printf("\nTree root node:\n");
    spKDTreeNodePrint(tree);

    do {
        if (getLine(queryMsg, queryPath, CONFIG_LINE_MAX_SIZE) != OK) {
            // TODO: Logger
        } else if (strcmp(queryPath,"<>") != 0) {
            queryCount++;
            printf("5-a. Query image path (%d) recieved : '%s'\n", queryCount, queryPath);
        } else {
          // TODO: Logger
          // go to exit phase
        }
    } while (strcmp(queryPath,"<>") != 0);

    // exit phase
    printf("6. Processed %d queries, preparing to exit\n", queryCount);
    printf("Exiting...\n");
    free(queryPath);
    spKDTreeDestroy(tree);
    spConfigDestroy(conf);
    spLoggerDestroy();
    return 0;
}
