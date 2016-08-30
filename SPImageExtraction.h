
/*
 *  SPImageExtraction
 *  Author: Yotam
 */

#ifndef SPIMAGEEXTRACTION_H_
#define SPIMAGEEXTRACTION_H_

#include "SPConfig.h"
#include "SPPoint.h"

typedef enum sp_extraction_msg {
    SP_EXTRACTION_INVALID_ARGUMENT,
    SP_EXTRACTION_ALLOC_FAIL,
    SP_EXTRACTION_FILE_ERROR,
    SP_EXTRACTION_SUCCESS
} SP_EXTRACTION_MSG;

SP_EXTRACTION_MSG spExtractFromImage(SPPoint *feats, int numOfFeatures, int index, const char *filename);

SP_EXTRACTION_MSG spExtractFromFiles(const char* filename, SPPoint* arr);

#endif // SPIMAGEEXTRACTION_H_
