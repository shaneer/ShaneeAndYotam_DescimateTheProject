/*
 *  KDArray_unit_test test file for Config
 *  Author: Shanee
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "unit_test_util.h"
#include "SPConfig.h"


int main() {
  SP_CONFIG_MSG msg1 = SP_CONFIG_ALLOC_FAIL;
  printf("LET US BEGIN:\n");
  char* imagepath;
  char* PCApath;
  imagepath = (char*) malloc(sizeof(1024));
  PCApath = (char*) malloc(sizeof(1024));

  SPConfig check1 = spConfigCreate("configEX1.txt", &msg1);
  printf("\nPrinting imDirectory 1: {%s}\n", spConfigGetImageDirectory(check1, &msg1));

  printf("\nCreated successfully\n");
  int numOfImgs = spConfigGetNumOfImages(check1, &msg1);
  printf("\nNum of Images should be 17 and is : %d\n", numOfImgs );
  bool msgCheck1;

  msgCheck1 = (msg1 == SP_CONFIG_SUCCESS);
  if (msgCheck1){
    printf("\nSuccess!\n");
  }

  spConfigGetImagePath(imagepath, check1, 10);
  puts(imagepath);
  free(imagepath);

  spConfigGetPCAPath(PCApath, check1);
  puts(PCApath);
  free(PCApath);

  spConfigDestroy(check1);
  return 0;
}
