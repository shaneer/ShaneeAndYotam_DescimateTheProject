
#ifndef MAINAUX_C_
#define MAINAUX_C_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "SPPoint.h"
#include "SPConfig.h"
#include "SPBPQueue.h"
#include "SPListElement.h"

typedef enum aux_readline_state_t {
    OK, NO_INPUT, TOO_LONG
} AUX_READ_STATE;

AUX_READ_STATE getLine(const char *prmpt, char *buff, size_t sz) {
    int ch, extra;

    // Get line with buffer overrun protection.
    if (prmpt != NULL) {
        printf ("%s", prmpt);
        fflush (stdout);
    }
    if (fgets (buff, sz, stdin) == NULL)
        return NO_INPUT;

    // If it was too long, there'll be no newline. In that case, we flush
    // to end of line so that excess doesn't affect the next call.
    if (buff[strlen(buff)-1] != '\n') {
        extra = 0;
        while (((ch = getchar()) != '\n') && (ch != EOF))
            extra = 1;
        return (extra == 1) ? TOO_LONG : OK;
    }

    // Otherwise remove newline and give string back to caller.
    buff[strlen(buff)-1] = '\0';
    return OK;
}

void freeSPPointsArray(SPPoint* arr, int size) {
    int i;
    for (i=0; i<size; i++) spPointDestroy(arr[i]);
    free(arr);
    return;
}

int compare2DInt(const void* a, const void* b) {
    int* pa = *(int**)a;
    int* pb = *(int**)b;
    if (pb[1] != pa[1]) {
        return (pb[1] - pa[1]);
    }
    return (pa[0] - pb[0]);
}

const char* spcbirGetConfigFilename(int argc, const char* argv[]) {
    if (argc == 1) {
        return "spcbir.config";
    } else if (argc == 3 && strcmp(argv[1],"-c") == 0 && strlen(argv[2]) > 0) {
        return argv[2];
    } else {
        printf("Invalid command line : use -c <config_filename>\n");
        return NULL;
    }
}

SPConfig spcbirOpenConfigFile(const char* filename) {
    SP_CONFIG_MSG conf_msg;
    SPConfig conf = spConfigCreate(filename, &conf_msg);
    if (conf_msg == SP_CONFIG_CANNOT_OPEN_FILE) {
        if (strcmp(filename, "spcbir.config") != 0) {
            // filename entered
            printf("The configuration file %s couldn’t be open\n", filename);
        } else {
            // default file
            printf("The default configuration file spcbir.config couldn’t be open\n");
        }
        return NULL;
    }
    return conf;
}

SP_CONFIG_MSG spcbirGetValuesFromConfig(SPConfig conf, int* numOfImages, int* numOfFeats, int* spKNN, int* spSimIm, bool* extraction_mode, SP_SPLIT_METHOD* split_method, char** loggerPath, SP_LOGGER_LEVEL* loggerLevel, bool* minimalGui) {
    SP_CONFIG_MSG conf_msg;
    *numOfImages = spConfigGetNumOfImages(conf, &conf_msg);
    if (conf_msg != SP_CONFIG_SUCCESS) {
        return conf_msg;
    }
    *numOfFeats = spConfigGetNumOfFeatures(conf, &conf_msg);
    if (conf_msg != SP_CONFIG_SUCCESS) {
        return conf_msg;
    }
    *spKNN = spConfigGetKNN(conf, &conf_msg);
    if (conf_msg != SP_CONFIG_SUCCESS) {
        return conf_msg;
    }
    *spSimIm = spConfigGetNumOfSimilarImages(conf, &conf_msg);
    if (conf_msg != SP_CONFIG_SUCCESS) {
        return conf_msg;
    }
    *extraction_mode = spConfigIsExtractionMode(conf, &conf_msg);
    if (conf_msg != SP_CONFIG_SUCCESS) {
        return conf_msg;
    }
    *split_method = spConfigGetSplitMethod(conf, &conf_msg);
    if (conf_msg != SP_CONFIG_SUCCESS) {
        return conf_msg;
    }
    *loggerPath = spConfigGetLoggerFilename(conf, &conf_msg);
    if (conf_msg != SP_CONFIG_SUCCESS) {
        return conf_msg;
    }
    *loggerLevel = spConfigGetLoggerLevel(conf, &conf_msg);
    if (conf_msg != SP_CONFIG_SUCCESS) {
        return conf_msg;
    }
    *minimalGui = spConfigMinimalGui(conf, &conf_msg);
    if (conf_msg != SP_CONFIG_SUCCESS) {
        return conf_msg;
    }
    return conf_msg;
}

void simpleFree(void* p1, void* p2, void* p3, void* p4, void* p5) {
    free(p1);
    free(p2);
    free(p3);
    free(p4);
    free(p5);
}

int spcbirCountImageProximity(SPBPQueue bpq, int** imgFeatCount_p) {
    int j, imIndexT;
    SPListElement el;
    int bpqSize = spBPQueueSize(bpq);
    for (j=0; j<bpqSize; j++) {
        el = spBPQueuePeek(bpq);
        if (!el) {
            return -1;
        }
        imIndexT = spListElementGetIndex(el);
        imgFeatCount_p[imIndexT][1] += 1;
        spBPQueueDequeue(bpq);
        spListElementDestroy(el);
    }
    return 0;
}

void spcbirPrintResultArray(int** imgFeatCount_p, int numOfImages, const char* filepath) {
    FILE* fp = fopen(filepath, "w");
    int i;
    for (i=0; i<numOfImages; i++) {
        fprintf(fp, "%d - %d (%d)\t", i, imgFeatCount_p[i][0], imgFeatCount_p[i][1]);
        if ((i+1)%5 == 0) {
            fprintf(fp, "\n");
        }
    }
    fclose(fp);
}

#endif
