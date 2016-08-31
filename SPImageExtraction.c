
/*
 *  SPImageExtraction
 *  Author: Yotam
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "SPConfig.h"
#include "SPPoint.h"
#include "SPImageExtraction.h"

SP_EXTRACTION_MSG spExtractFromImage(SPPoint *feats, int numOfFeatures, int index, const char *filename) {
    // input check
    if (!feats || !filename)
        return SP_EXTRACTION_INVALID_ARGUMENT;

    int i,j,fDim;

    fDim = spPointGetDimension(*feats);
    FILE *fp = fopen(filename, "w");

    if (fp == NULL) {
        return SP_EXTRACTION_FILE_ERROR;
    }
    // Write image index and features number
    if (fprintf(fp, "%d %d %d\n",index, numOfFeatures, fDim) < 0) {
        fclose(fp);
        return SP_EXTRACTION_FILE_ERROR;
    }
    for (i=0; i<numOfFeatures; i++) {
        for (j=0; j<fDim; j++) {
            if (fprintf(fp, "%.4f ", spPointGetAxisCoor(feats[i], j)) < 0) {
                fclose(fp);
                return SP_EXTRACTION_FILE_ERROR;
            }
            // File write was successful
        }
        fprintf(fp, "\n");
    }
    // Finished writing all features
    fclose(fp);
    return SP_EXTRACTION_SUCCESS;

}

SP_EXTRACTION_MSG spExtractFromFiles(const char* filename, SPPoint* arr) {
    if (!arr || !filename)
        return SP_EXTRACTION_INVALID_ARGUMENT;

    int i,j,k,index,numOfFeatures,fDim;
    double* pData;
    
    FILE *fp = fopen(filename, "r");

    if (fp == NULL) {
        return SP_EXTRACTION_FILE_ERROR;
    }
    // Read image index and features number
    if (fscanf(fp, "%d %d %d\n",&index, &numOfFeatures, &fDim) == 0) {
        fclose(fp);
        return SP_EXTRACTION_FILE_ERROR;
    }
    arr = (SPPoint*)calloc(numOfFeatures,sizeof(SPPoint));
    pData = (double*)calloc(fDim,sizeof(double));
    if (!arr || !pData) {
        free(arr);
        free(pData);
        fclose(fp);
        return SP_EXTRACTION_ALLOC_FAIL;
    }
    // Allocation successful
    for (i=0; i<numOfFeatures; i++) {
        for (j=0; j<fDim; j++) {
            if (fscanf(fp,"%lf ", &(pData[j])) == 0) {
                free(arr);
                free(pData);
                fclose(fp);
                return SP_EXTRACTION_FILE_ERROR;
            }
            // Read successful
        }
        arr[i] = spPointCreate(pData, fDim, index);
        if (!arr[i]) {
            for (k=0; k<i; k++) {
                spPointDestroy(arr[k]);
            }
            free(arr);
            free(pData);
            fclose(fp);
            return SP_EXTRACTION_ALLOC_FAIL;
        }
        // Point created successfuly
    }
    // Finished reading
    free(pData);
    fclose(fp);
    return SP_EXTRACTION_SUCCESS;
}
