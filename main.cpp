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
    int i, j, numOfFeats, featsCount = 0, numOfImages, queryCount = 0, spKNN, spSimIm, bpqSize, imIndexT;
    char* imagePath, *featsPath, *loggerPath, *queryPath = NULL;
    ImageProc* proc = NULL;
    SPPoint* featsT, *feats;
    SPKDArray kdarr;
    SPKDTreeNode tree;
    SPBPQueue bpq;
    SPListElement el;
    int** imgFeatCount_p, *imgFeatCount_d;
    bool extraction_mode, flag = false;
    SP_SPLIT_METHOD split_method;
    SP_LOGGER_LEVEL loggerLevel;
    SP_EXTRACTION_MSG ext_msg;
    SP_CONFIG_MSG conf_msg;;
    SP_LOGGER_MSG log_msg;

    // get filename from argv[2] (or use default)
    filename = spcbirGetConfigFilename(argc, argv);
    if (!filename) exit(0);

    SPConfig conf = spConfigCreate(filename, &conf_msg);
    // TODO: Logger
    if (conf_msg == SP_CONFIG_CANNOT_OPEN_FILE) {
        if (strcmp(filename, "spcbir.config") != 0) {
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
        flag = true;
    }
    numOfFeats = spConfigGetNumOfFeatures(conf, &conf_msg);
    if (conf_msg != SP_CONFIG_SUCCESS) {
        flag = true;
    }
    spKNN = spConfigGetKNN(conf, &conf_msg);
    if (conf_msg != SP_CONFIG_SUCCESS) {
        flag = true;
    }
    spSimIm = spConfigGetNumOfSimilarImages(conf, &conf_msg);
    if (conf_msg != SP_CONFIG_SUCCESS) {
        flag = true;
    }
    extraction_mode = spConfigIsExtractionMode(conf, &conf_msg);
    if (conf_msg != SP_CONFIG_SUCCESS) {
        flag = true;
    }
    split_method = spConfigGetSplitMethod(conf, &conf_msg);
    if (conf_msg != SP_CONFIG_SUCCESS) {
        flag = true;
    }
    loggerPath = spConfigGetLoggerFilename(conf, &conf_msg);
    if (conf_msg != SP_CONFIG_SUCCESS) {
        flag = true;
    }
    loggerLevel = spConfigGetLoggerLevel(conf, &conf_msg);
    if (conf_msg != SP_CONFIG_SUCCESS) {
        flag = true;
    }
    log_msg = spLoggerCreate(loggerPath, loggerLevel);
    if (log_msg != SP_LOGGER_SUCCESS) {
        flag = true;
    }
    if (flag) {
        // TODO: Logger / Error
    }
    // Logger created path can be freed
    free(loggerPath);

    // create a big enough SPPoint array for all features from all images.
    // the spNumOfFeatures attricute from the config file states the MAX num of features per image,
    // but since opencv sometimes extracts (spNumOfFeatures+1) features, it's been addad.
    feats = (SPPoint*)calloc(numOfImages*(numOfFeats + 1), sizeof(SPPoint));
    queryPath = (char*)calloc(CONFIG_LINE_MAX_SIZE+1, sizeof(char));
    imgFeatCount_p = (int**)calloc(numOfImages, sizeof(int*));
    imgFeatCount_d = (int*)calloc(2*numOfImages, sizeof(int));
    proc = new ImageProc(conf); // creat ImageProc ovject
    if (imgFeatCount_d == NULL || imgFeatCount_p == NULL || feats == NULL || queryPath == NULL) {
        // TODO: Logger / Error
        freeSPPointsArray(feats, numOfFeats);
        free(queryPath);
        free(imgFeatCount_d);
        free(imgFeatCount_p);
        spConfigDestroy(conf);
        spLoggerDestroy();
        delete proc;
        exit(0);
    }
    printf("0. General memory allocation, and preparation successful\n");
    // fix pointers for results array
    for (i=0; i<numOfImages; i++) {
        imgFeatCount_p[i] = imgFeatCount_d + 2*i;
    }

    if (extraction_mode) {
        // Extract mode
        printf("1-a. Extraction mode\n");
        imagePath = (char*)malloc(1+CONFIG_LINE_MAX_SIZE*4); // create a big enough buffer
        featsPath = (char*)malloc(1+CONFIG_LINE_MAX_SIZE*4); // create a big enough buffer

        if (!proc || !imagePath || !featsPath) {
            // TODO: Logger
            free(imagePath);
            free(featsPath);
            free(imgFeatCount_d);
            free(imgFeatCount_p);
            freeSPPointsArray(feats, numOfFeats);
            free(queryPath);
            delete proc;
            spConfigDestroy(conf);
            spLoggerDestroy();
            exit(0);
        }
        // go over all pictures and store data in file and in the feats array
        for (i = 0; i < numOfImages; i++) {
            // TODO: Logger
            if (spConfigGetFeaturesFilePath(featsPath, conf, i) != SP_CONFIG_SUCCESS || spConfigGetImagePath(imagePath, conf, i) != SP_CONFIG_SUCCESS) {
                free(imagePath);
                free(featsPath);
                free(queryPath);
                free(imgFeatCount_d);
                free(imgFeatCount_p);
                delete proc;
                spConfigDestroy(conf);
                spLoggerDestroy();
                freeSPPointsArray(feats, numOfFeats);
                exit(0);
            }
            featsT = proc->getImageFeatures(imagePath, i, &numOfFeats); // get features for image
            ext_msg = spExtractFromImage(featsT, numOfFeats, i, featsPath); // save features to file
            if (!featsT || ext_msg != SP_EXTRACTION_SUCCESS) {
                // TODO: Logger
                free(imagePath);
                free(featsPath);
                free(queryPath);
                free(imgFeatCount_d);
                free(imgFeatCount_p);
                delete proc;
                spConfigDestroy(conf);
                spLoggerDestroy();
                freeSPPointsArray(feats, numOfFeats);
                exit(0);
            }
            // move features to generall array and destroy the temp array
            for (j=0; j < numOfFeats; j++) {
                feats[featsCount] = featsT[j];
                featsCount++;
            }
            free(featsT);
            // TODO: Logger
        }
        free(imagePath);
        free(featsPath);
        // TODO: Logger
    } else {
        // Non-Extract mode
        printf("1-b. Non-Extraction Mode:\n");
        featsPath = (char*)malloc(1+CONFIG_LINE_MAX_SIZE*4); // create a big enough buffer
        if (!featsPath) {
            // TODO: Logger
            free(featsPath);
            free(imgFeatCount_d);
            free(imgFeatCount_p);
            freeSPPointsArray(feats, numOfFeats);
            free(queryPath);
            spConfigDestroy(conf);
            spLoggerDestroy();
            delete proc;
            exit(0);
        }
        for (i = 0; i < numOfImages; i++) {
            conf_msg = spConfigGetFeaturesFilePath(featsPath, conf, i);
            featsT = spExtractFromFiles(featsPath, &numOfFeats, &ext_msg);
            // TODO: Logger
            if (conf_msg != SP_CONFIG_SUCCESS || ext_msg != SP_EXTRACTION_SUCCESS ||  featsT == NULL) {
                free(featsPath);
                free(queryPath);
                free(imgFeatCount_d);
                free(imgFeatCount_p);
                spConfigDestroy(conf);
                spLoggerDestroy();
                freeSPPointsArray(feats, numOfFeats);
                delete proc;
                exit(0);
            }
            for (j=0; j<numOfFeats; j++) {
                feats[featsCount] = featsT[j];
                featsCount++;
            }
            free(featsT);
            // TODO: Logger
        }
        free(featsPath);
        // TODO: Logger
    }
    printf("2. Finished processing - %d features\n", featsCount);
    kdarr = Init(feats, featsCount); // no need to to destroy KDArray (handles in tree creation)
    // at this point no neef ro the SPPoint array (feats)
    freeSPPointsArray(feats, numOfFeats);
    printf("3. KDArray created\n");

    tree = spKDTreeCreate(kdarr, split_method);
    printf("4. KDTree created\n");

    do {
        if (getLine(queryMsg, queryPath, CONFIG_LINE_MAX_SIZE) != OK) {
            // TODO: Logger
        } else if (strcmp(queryPath,"<>") != 0) {
            queryCount++;
            printf("5-a. Query image path (%d) recieved : '%s'\n", queryCount, queryPath);
            featsT = proc->getImageFeatures(queryPath, numOfImages+1, &numOfFeats);

            // reset image result counter array
            for (i=0; i<numOfImages; i++) {
                imgFeatCount_p[i][0] = i;
                imgFeatCount_p[i][1] = 0;
            }
            // count image features per feature
            for (i=0; i<numOfFeats; i++) {
                bpq = spKDTreeFindKNearestNeighbors(tree, spKNN, featsT[i]);
                bpqSize = spBPQueueSize(bpq);
                for (j=0; j<bpqSize; j++) {
                    el = spBPQueuePeek(bpq);
                    imIndexT = spListElementGetIndex(el);
                    imgFeatCount_p[imIndexT][1] += 1;
                    spBPQueueDequeue(bpq);
                    spListElementDestroy(el);
                }
                spBPQueueDestroy(bpq);
            }
            freeSPPointsArray(featsT, numOfFeats); // free the query image features array

            printf("5-b. Calculating %d best similar images\n", spSimIm);

            // sort results to get best images
            qsort(imgFeatCount_p, numOfImages, sizeof(int*), compare2DInt);

            for (i=0; i<spSimIm; i++) {
                printf("\t- #%d closes image is %d (%d)\n", i, imgFeatCount_p[i][0], imgFeatCount_p[i][1]);
            }
        } else {
            // TODO: Logger
            // go to exit phase
        }
    } while (strcmp(queryPath,"<>") != 0);

    // exit phase
    printf("6. Processed %d queries, preparing to exit\n", queryCount);
    printf("Exiting...\n");
    free(queryPath);
    free(imgFeatCount_d);
    free(imgFeatCount_p);
    delete proc;
    spKDTreeDestroy(tree);
    spConfigDestroy(conf);
    spLoggerDestroy();
    return 0;
}
