
#ifndef MAINAUX_C_
#define MAINAUX_C_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "SPPoint.h"

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
  for (i=0;i<size;i++) spPointDestroy(arr[i]);
  free(arr);
  return;
}

#endif
