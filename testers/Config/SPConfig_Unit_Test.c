/*
 *  KDArray_unit_test test file for Config
 *  Author: Shanee
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "unit_test_util.h"
#include "../../SPConfig.h"


int main() {
  SP_CONFIG_MSG* msg1;
  SPConfig check1 = spConfigCreate(".../testers/Config/configEX1.txt", msg1);
  printf("\n%s", check1->spImagesSuffix);
  printf("\n%s", msg1);

  return 0;
}
