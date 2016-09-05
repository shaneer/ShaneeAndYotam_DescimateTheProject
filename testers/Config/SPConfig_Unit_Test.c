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

  SPConfig check1 = spConfigCreate("configEX1.txt", &msg1);
  
  spConfigDestroy(check1);
  return 0;
}
