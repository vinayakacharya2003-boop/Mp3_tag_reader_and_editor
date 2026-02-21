#include <stdio.h>                            // Standard I/O functions
#include <string.h>                           // String handling functions
#include<stdlib.h>                            // Standard library (malloc, free)
#include "type.h"                            // User-defined types and enum
#include "view.h"                            // User-defined view functions
#include<ctype.h>                            // Character functions (isdigit)

const char* tagname(char tag[])              // Map frame ID to human-readable tag name
{
    if(strcmp(tag,"TIT2")==0) return "Title";   // Title frame
    if(strcmp(tag,"TPE1")==0) return "Artist";  // Artist frame
    if(strcmp(tag,"TALB")==0) return "Album";   // Album frame
    if(strcmp(tag,"TYER")==0) return "Year";    // Year frame
    if(strcmp(tag,"TCON")==0) return "Music";   // Music genre frame
    if(strcmp(tag,"COMM")==0) return "Comments"; // Comments frame
    return "Unknown";                           // Unknown frame
}

Status open_mp3_file(const char *filename, FileInfo *file_info) {
    file_info->fptr = fopen(filename, "rb");   // Open file in binary read mode
    if (file_info->fptr == NULL) return FAILURE;  // Return failure if open fails

    if (strlen(filename) < sizeof(file_info->filename)) { // Check filename length fits buffer
        strcpy(file_info->filename, filename);  // Copy filename
    } else {
        fclose(file_info->fptr);
        return FAILURE;                         // Fail if filename too large
    }

    if (fseek(file_info->fptr, 0, SEEK_END) != 0) {   // Seek to end to get file size
        fclose(file_info->fptr);
        return FAILURE;
    }

    long sz = ftell(file_info->fptr);            // Get file size
    if (sz < 0) {
        fclose(file_info->fptr);
        return FAILURE;                         // Fail if ftell error
    }
    file_info->file_size = sz;                    // Store file size
    rewind(file_info->fptr);                       // Reset to file start

    return SUCCESS;                               // Success
}

Status read_id3v2_tag(FileInfo *file_info, ID3v2Tag *tag_read)
{
    size_t read_count = fread(tag_read->id3, 3, 1, file_info->fptr); // Read 3-byte ID3 tag signature
    if(read_count != 1) {
        printf("Error: Could not read ID3 signature.\n");
        return FAILURE;                          // Fail if unable to read signature
    }
    tag_read->id3[3] = '\0';                      // Null-terminate read string

    if (strcmp(tag_read->id3, "ID3") != 0) {     // Check for proper ID3 identifier
        printf("Error: Its not a mp3 file.\n");
        return FAILURE;                          // Fail if not an mp3 with ID3 tag
    }

    if (fseek(file_info->fptr, 7, SEEK_CUR) != 0) { // Skip version, flags, and size bytes
        printf("Error: Could not skip ID3 header.\n");
        return FAILURE;
    }

    printf("----------- MP3 Tag Information -----------\n");  // Header for tag data display
    printf("%-15s | %s\n", "Tag", "Content");                // Column titles
    printf("------------------------------------------\n");

    for (int i = 0; i < 6; i++) {                          // Read up to 6 frames
        if (fread(tag_read->tag, 4, 1, file_info->fptr) != 1) // Read 4-byte frame ID
            break;

        tag_read->tag[4] = '\0';                           // Null-terminate frame ID

        if (tag_read->tag[0] == 0)                         // Break on padding byte (end)
            break;

        unsigned char sizebuf[4];
        if (fread(sizebuf, 4, 1, file_info->fptr) != 1)   // Read 4-byte frame size
            break;

        tag_read->size =                                    // Calculate size as big-endian int
            (sizebuf[0] << 24) |
            (sizebuf[1] << 16) |
            (sizebuf[2] << 8)  |
            (sizebuf[3]);

        if (fseek(file_info->fptr, 2, SEEK_CUR) != 0)     // Skip 2-byte frame flags
            break;

        if (tag_read->size <= 0 || tag_read->size > file_info->file_size) { // Validate size
            printf("Error: Invalid frame size.\n");
            break;
        }

        char *str = malloc(tag_read->size + 1);             // Allocate buffer for frame data
        if (str == NULL) {
            printf("Error: Memory allocation failed.\n");
            return FAILURE;
        }

        if (fread(str, tag_read->size, 1, file_info->fptr) != 1) { // Read frame data
            free(str);
            break;
        }

        int offset = (str[0] == 0 || str[0] == 1) ? 1 : 0;  // Offset for encoding byte
        str[tag_read->size] = '\0';                         // Null-terminate string

        printf("%-15s | %s\n", tagname(tag_read->tag), str + offset); // Display tag name & content

        free(str);                                         // Free allocated buffer
    }

    printf("------------------------------------------\n");    // Footer line

    return SUCCESS;                                      // Success
}

void view_mp3(const char *filename, FileInfo *file_info)
{
    if (open_mp3_file(filename, file_info) == FAILURE) {  // Open MP3 file
        printf("ERROR: Could not open file!\n");
        return;
    }

    ID3v2Tag tag_read;
    if (read_id3v2_tag(file_info, &tag_read) != SUCCESS) { // Read and display tags
        printf("ERROR: Could not read tags!\n");
    }
}
