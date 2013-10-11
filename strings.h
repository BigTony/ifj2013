#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "errors.h"

typedef struct{
    int length;
    int alloc;
    char *data;
} String;

