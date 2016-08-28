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

  SPConfig check1 = spConfigCreate("configEX1.txt", &msg1);
  int numOfImgs = spConfigGetNumOfImages(check1, &msg1);
  printf("\nNum of Images should be 17 and is : %d", numOfImgs );
  bool msgCheck1;
  msgCheck1 = (msg1 == SP_CONFIG_SUCCESS);
  if (msgCheck1){
    printf("Success!");
  }
  return 0;
}
