#include "utils.h"

size_t trimwhitespace(char* out, size_t len, const char* str)
{
    if (len == 0)
        return 0;

    const char* end;
    size_t out_size;

    // Trim leading space
    while (isspace((unsigned char)*str))
        str++;

    if (*str == 0) // All spaces?
    {
        *out = 0;
        return 1;
    }

    // Trim trailing space
    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end))
        end--;
    end++;

    // Set output size to minimum of trimmed string length and buffer size minus 1
    out_size = (end - str) < len - 1 ? (end - str) : len - 1;

    // Copy trimmed string and add null terminator
    memcpy(out, str, out_size);
    out[out_size] = 0;

    return out_size;
}

int read_map(char** map_data, int* cols, int* rows)
{

    FILE* f = fopen("map.dat", "r");

    if (f == NULL) 
        return -1;

    int allocated = 0;
    int read = 0;
    int c = 0;
    while((c = getc(f)) != '\n')
        if (c != ',')
            (*cols)++;

    rewind(f);

    while((c = getc(f)) != EOF) {
        if (c == ',')
            continue;
        else if (c == '\n')
            (*rows)++;
        else {
            if (allocated >= read) {
                char* temp = realloc(*map_data, allocated + 10);
                if (temp == NULL)
                    return -1;
                memset(temp+allocated, 0, 10);
                *map_data = temp;
                allocated += 10;
            }

            (*map_data)[read++] = c;
        }
    }
    fclose(f);
    return read;
}

