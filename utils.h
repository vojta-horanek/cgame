#ifndef UTILS_H
#define UTILS_H

#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

size_t trimwhitespace(char* out, size_t len, const char* str);
int read_map(char** map_data, int* cols, int* rows);

#endif
