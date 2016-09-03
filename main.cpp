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
    int i, j, numOfFeats, featsCount = 0, numOfImages, queryCount = 0, spKNN, spSimIm;
    char* imagePath, *featsPath, *loggerPath, *queryPath = NULL;
    ImageProc* proc = NULL;
    SPConfig conf;
    SPPoint* featsT, *feats;
    SPKDArray kdarr;
    SPKDTreeNode tree;
    SPBPQueue bpq;
    int** imgFeatCount_p, *imgFeatCount_d;
    bool extraction_mode, sanity = true;
    SP_SPLIT_METHOD split_method;
    SP_LOGGER_LEVEL loggerLevel;
    SP_EXTRACTION_MSG ext_msg;
    SP_CONFIG_MSG conf_msg;;
    SP_LOGGER_MSG log_msg;

    // get filename from argv[2] (or use default)
    filename = spcbirGetConfigFilename(argc, argv);
    if (!filename) {
        // TODO: Error
        exit(0);
    }

    conf = spcbirOpenConfigFile(filename);
    if (!conf) {
        // TODO: Error
        exit(0);
    }

    // get attributes from config file for program use
    conf_msg = spcbirGetValuesFromConfig(conf, &numOfImages, &numOfFeats, &spKNN, &spSimIm, &extraction_mode, &split_method, &loggerPath, &loggerLevel);
    if (conf_msg != SP_CONFIG_SUCCESS) {
        // TODO: Error
        spConfigDestroy(conf);
        exit(0);
    }
    log_msg = spLoggerCreate(loggerPath, loggerLevel);
    if (log_msg != SP_LOGGER_SUCCESS) {
        // TODO: Error
        spConfigDestroy(conf);
        exit(0);
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
    imagePath = (char*)malloc(1+CONFIG_LINE_MAX_SIZE*4); // create a big enough buffer
    featsPath = (char*)malloc(1+CONFIG_LINE_MAX_SIZE*4); // create a big enough buffer
    proc = new ImageProc(conf); // creat ImageProc ovject
    if (featsPath == NULL || imagePath == NULL || proc == NULL || imgFeatCount_d == NULL || imgFeatCount_p == NULL || feats == NULL || queryPath == NULL) {
        // TODO: Logger / Error
        freeSPPointsArray(feats, numOfFeats);
        simpleFree(queryPath, imgFeatCount_d, imgFeatCount_p, imagePath, featsPath);
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
        // go over all pictures and store data in file and in the feats array
        for (i = 0; i < numOfImages; i++) {
            if (spConfigGetFeaturesFilePath(featsPath, conf, i) != SP_CONFIG_SUCCESS || spConfigGetImagePath(imagePath, conf, i) != SP_CONFIG_SUCCESS) {
                // TODO: Logger
                freeSPPointsArray(feats, numOfFeats);
                simpleFree(queryPath, imgFeatCount_d, imgFeatCount_p, imagePath, featsPath);
                spConfigDestroy(conf);
                spLoggerDestroy();
                delete proc;
                exit(0);
            }
            featsT = proc->getImageFeatures(imagePath, i, &numOfFeats); // get features for image
            ext_msg = spExtractFromImage(featsT, numOfFeats, i, featsPath); // save features to file
            if (!featsT || ext_msg != SP_EXTRACTION_SUCCESS) {
                // TODO: Logger
                freeSPPointsArray(feats, numOfFeats);
                simpleFree(queryPath, imgFeatCount_d, imgFeatCount_p, imagePath, featsPath);
                spConfigDestroy(conf);
                spLoggerDestroy();
                delete proc;
                exit(0);
            }
            // move features to generall array and destroy the temp array
            for (j=0; j < numOfFeats; j++) {
                feats[featsCount] = featsT[j];
                featsCount++;
            }
            free(featsT);
        }
    } else {
        // Non-Extract mode
        printf("1-b. Non-Extraction Mode:\n");
        for (i = 0; i < numOfImages; i++) {
            conf_msg = spConfigGetFeaturesFilePath(featsPath, conf, i);
            featsT = spExtractFromFiles(featsPath, &numOfFeats, &ext_msg);
            if (conf_msg != SP_CONFIG_SUCCESS || ext_msg != SP_EXTRACTION_SUCCESS) {
                // TODO: Logger
                freeSPPointsArray(feats, featsCount);
                simpleFree(queryPath, imgFeatCount_d, imgFeatCount_p, imagePath, featsPath);
                spConfigDestroy(conf);
                spLoggerDestroy();
                delete proc;
                exit(0);
            }
            for (j=0; j<numOfFeats; j++) {
                feats[featsCount] = featsT[j];
                featsCount++;
            }
            free(featsT);
        }
    }
    simpleFree(imagePath, featsPath, NULL, NULL, NULL);
    printf("2. Finished processing - %d features\n", featsCount);
    kdarr = Init(feats, featsCount); // no need to to destroy KDArray (handles in tree creation)
    // at this point no neef ro the SPPoint array (feats)
    freeSPPointsArray(feats, featsCount);
    printf("3. KDArray created\n");
    tree = spKDTreeCreate(kdarr, split_method);
    printf("4. KDTree created\n");
    // main query loop
    do {
      // sanity explained:
      // ---------------------
      // It's a mechanism for making sure nothing went wrong without a large amout of exit().
      // sanity = true -> everything is ok
      // sanity = false -> something went wrong
      // Therefor every stage in the program checks is sanity == true.
        if (getLine(queryMsg, queryPath, CONFIG_LINE_MAX_SIZE) != OK) {
            //  TODO: Logger
            sanity = false;
        } else if (strcmp(queryPath,"<>") != 0) {
            queryCount++;
            printf("5-a. Query image path (%d) recieved : '%s'\n", queryCount, queryPath);
            featsT = proc->getImageFeatures(queryPath, numOfImages+1, &numOfFeats);
            if (!featsT) {
                // TODO: Logger
                sanity = false;
            }
            // reset image result counter array
            for (i=0; i<numOfImages; i++) {
                imgFeatCount_p[i][0] = i;
                imgFeatCount_p[i][1] = 0;
            }
            // count image features per feature
            for (i=0; i<numOfFeats && sanity; i++) {
                bpq = spKDTreeFindKNearestNeighbors(tree, spKNN, featsT[i]);
                if (!bpq) {
                    // TODO: Logger
                    sanity = false;
                } else {
                    if (spcbirCountImageProximity(bpq, imgFeatCount_p) < 0) {
                        // TODO: Logger
                        sanity = false;
                    }
                    spBPQueueDestroy(bpq);
                }
            }
            if (featsT) freeSPPointsArray(featsT, numOfFeats);     // free the query image features array
            if (sanity) {
                printf("5-b. Calculating %d best similar images\n", spSimIm);
                // sort results to get best images
                qsort(imgFeatCount_p, numOfImages, sizeof(int*), compare2DInt);
                for (i=0; i<spSimIm; i++) {
                    printf("\t- #%d closes image is %d (%d)\n", i, imgFeatCount_p[i][0], imgFeatCount_p[i][1]);
                }
            }
        } else {
            // TODO: Logger
            // go to exit phase
        }
    } while (sanity && strcmp(queryPath,"<>") != 0);

    // exit phase
    printf("6. Processed %d queries, preparing to exit\n", queryCount);
    printf("Exiting...\n");
    simpleFree(queryPath, imgFeatCount_d, imgFeatCount_p, NULL, NULL);
    spKDTreeDestroy(tree);
    spConfigDestroy(conf);
    spLoggerDestroy();
    delete proc;
    return 0;
}
