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
}

using namespace sp;

int main(int argc, char const *argv[]) {
    const char* filename;
    int i, j, numOfFeats, featsCount = 0;
    char* imagePath, *featsPath, *loggerPath;
    SPPoint* featsT, *feats;
    SPKDArray kdarr;
    SPKDTreeNode tree;
    ImageProc* proc;
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
    // Logger created
    free(loggerPath);

    feats = (SPPoint*)calloc(spConfigGetNumOfImages(conf, &conf_msg)*spConfigGetNumOfFeatures(conf, &conf_msg), sizeof(SPPoint));
    if (feats == NULL) {
        // TODO: Error
        spConfigDestroy(conf);
        spLoggerDestroy();
        exit(0);
    }

    if (spConfigIsExtractionMode(conf, &conf_msg)) {
      printf("Extraction mode\n");
        // Extract mode
        proc = new ImageProc(conf); // staticlly creat ImageProc ovject (delete not needed)
        imagePath = (char*)malloc(1+CONFIG_LINE_MAX_SIZE*4); // create a big enough buffer
        featsPath = (char*)malloc(1+CONFIG_LINE_MAX_SIZE*4); // create a big enough buffer

        for (i = 0; i < spConfigGetNumOfImages(conf, &conf_msg) && conf_msg == SP_CONFIG_SUCCESS; i++) {
            conf_msg = spConfigGetImagePath(imagePath, conf, i);
            // TODO: Logger
            if (conf_msg != SP_CONFIG_SUCCESS) {
                free(imagePath);
                free(featsPath);
                delete proc;
                spLoggerDestroy();
                exit(0);
            }
            conf_msg = SPConfigGetFeaturesFilePath(featsPath, conf, i);
            // TODO: Logger
            if (conf_msg != SP_CONFIG_SUCCESS) {
                free(imagePath);
                free(featsPath);
                delete proc;
                spLoggerDestroy();
                exit(0);
            }
            featsT = proc->getImageFeatures(imagePath, i, &numOfFeats);
            ext_msg = spExtractFromImage(featsT, numOfFeats, i, featsPath);
            if (ext_msg != SP_EXTRACTION_SUCCESS) {
                // TODO: Logger
                free(imagePath);
                free(featsPath);
                delete proc;
                spLoggerDestroy();
                exit(0);
            }
            // move features to generall array and destroy the temp array
            for (j=0; j<numOfFeats; j++) {
                feats[featsCount] = featsT[j];
                featsCount++;
            }
            free(featsT);
            printf("- Processed %s\n", imagePath);
        }
        free(imagePath);
        free(featsPath);
        delete proc;
    } else {
        // Non-Extract mode
    }
    printf("Finished processing\n");
    kdarr = Init(feats, featsCount); // no needto to destroy (handles in tree creation)
    printf("KDArray created\n");
    tree = spKDTreeCreate(kdarr, split_method);
    printf("KDTree created\n");

    printf("Tree root node\n");
    spKDTreeNodePrint(tree);

    printf("Preparing to exit\n");
    spKDTreeDestroy(tree);
    spConfigDestroy(conf);
    spLoggerDestroy();
    return 0;
}
