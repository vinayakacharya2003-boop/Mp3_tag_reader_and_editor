#ifndef TYPE_H
#define TYPE_H

#include <stdio.h>

// Operation types
typedef enum {
    unsupported,
    view,
    edit
} OperationType;

// Status codes
typedef enum {
    SUCCESS,
    FAILURE,
    END_OF_FRAMES
} Status;

typedef struct {
    char id3[4];
   // char version[3];
    char tag[5];
    int size;
} ID3v2Tag;

// File information structure
typedef struct {
    FILE *fptr;
    char filename[256];
    long file_size;
} FileInfo;

#endif
