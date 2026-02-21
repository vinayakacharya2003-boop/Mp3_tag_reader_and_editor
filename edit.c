#include <stdio.h>                            // Include standard I/O functions
#include <string.h>                           // Include string manipulation functions
#include <stdlib.h>                           // Include standard library (memory allocation etc.)
#include "type.h"                            // Include user-defined type definitions
#include "view.h"                            // Include user-defined view functions
#include <ctype.h>                           // Include character type functions (isdigit etc.)

void convert_big_to_lit(int *size)
{
    char *ptr = (char*)size;                  // Cast int pointer to byte pointer
    char temp;
    temp = ptr[0];                            // Temporarily store first byte
    ptr[0] = ptr[3];                          // Swap first and fourth byte
    ptr[3] = temp;                            // Complete swap
    temp = ptr[1];                            // Temporarily store second byte
    ptr[1] = ptr[2];                          // Swap second and third byte
    ptr[2] = temp;                            // Complete swap
}

int isnumeric(char str[])
{
    int i = 0;
    while (str[i] != '\0') {                  // Iterate till end of string
        if (!isdigit((unsigned char)str[i])) {// If any char is not digit
            return 0;                         // Return false
        }
        i++;                                 // Increment index
    }
    return i;                                // Return number of digits (i.e. length)
}

void display_edit(char str[], char new[])
{
    printf("-------------=====SELECTED EDIT DETAILS=====---------\n\n");  // Display header
    printf("-------------------CHANGE THE %s--------------\n", str);        // Display tag name changed
    printf("\n");
    printf("\t\t%-10s    :   %-20s\n", str, new);                      // Display new value formatted
    printf("\n");
    printf("============%s CHANGED SUCCESSFULLY=================\n", str); // Display success message
}

void edit_mp3(char str[], char new_char[], char file_name[])
{
    FILE *fptr = fopen(file_name, "rb");      // Open original file in binary read mode
    if (fptr == NULL) {
        printf("Error: File could not be opened.\n");   // Error if file open fails
        return;
    }

    char header[10];
    if (fread(header, 1, 10, fptr) != 10) {   // Read 10-byte ID3 header
        printf("Error: Could not read MP3 header.\n");    // Error if header read fails
        fclose(fptr);
        return;
    }

    FILE *fp = fopen("music.mp3", "wb");      // Open temporary file in binary write mode
    if (fp == NULL) {
        printf("Error: Could not create temporary file.\n");  // Error if temp file open fails
        fclose(fptr);
        return;
    }

    if (fwrite(header, 1, 10, fp) != 10) {    // Write header to temp file
        printf("Error: Could not write header to file.\n");   // Error if write fails
        fclose(fptr);
        fclose(fp);
        return;
    }

    char tag_name[5];
    if (strcmp(str, "-t") == 0)
        strcpy(tag_name, "TIT2");              // Map option to Title tag
    else if (strcmp(str, "-a") == 0)
        strcpy(tag_name, "TPE1");              // Map option to Artist tag
    else if (strcmp(str, "-A") == 0)
        strcpy(tag_name, "TALB");              // Map option to Album tag
    else if (strcmp(str, "-y") == 0) {
        if ((strlen(new_char) != 4) || (isnumeric(new_char) != 4)) {   // Validate year length and digits
            printf("Invalid Year\n");
            fclose(fptr);
            fclose(fp);
            return;
        }
        strcpy(tag_name, "TYER");              // Map option to Year tag
    } 
    else if (strcmp(str, "-m") == 0)
        strcpy(tag_name, "TCON");              // Map option to Music genre tag
    else if (strcmp(str, "-c") == 0)
        strcpy(tag_name, "COMM");              // Map option to Comments tag
    else {
        printf("Unknown tag option: %s\n", str);  // Error for unknown option
        fclose(fptr);
        fclose(fp);
        return;
    }
    tag_name[4] = '\0';                        // Null terminate tag_name string

    char tag[5];
    int size = (int)strlen(new_char);
    int new_size, temp_size;
    char flag[2];

    while (fread(tag, 1, 4, fptr) == 4) {    // Read frame ID (4 bytes)
        if (tag[0] == 0)                        // Break if frame ID is padding
            break;
        tag[4] = '\0';                        // Null terminate frame ID

        if (fwrite(tag, 1, 4, fp) != 4) {    // Write frame ID to temp file
            printf("Error: Could not write tag frame ID.\n");
            break;
        }

        if (fread(&temp_size, 4, 1, fptr) != 1) {  // Read frame size (4 bytes)
            printf("Error: Could not read frame size.\n");
            break;
        }

        if (fread(flag, 1, 2, fptr) != 2) {        // Read frame flags (2 bytes)
            printf("Error: Could not read frame flags.\n");
            break;
        }

        convert_big_to_lit(&temp_size);         // Convert frame size endian

        if (strcmp(tag, tag_name) == 0) {       // If current frame is the tag to edit
            new_size = size + 1;                 // New size includes encoding byte
            int new_size_be = new_size;
            convert_big_to_lit(&new_size_be);   // Convert new size endian

            if (fwrite(&new_size_be, 1, 4, fp) != 4) {  // Write new frame size
                printf("Error: Could not write new frame size.\n");
                break;
            }
            if (fwrite(flag, 1, 2, fp) != 2) {          // Write frame flags
                printf("Error: Could not write new frame flags.\n");
                break;
            }
            if (fputc('\0', fp) == EOF) {                 // Write encoding byte (0)
                printf("Error: Could not write encoding byte.\n");
                break;
            }
            if (fwrite(new_char, 1, size, fp) != size) { // Write new data
                printf("Error: Could not write new tag content.\n");
                break;
            }
            if (fseek(fptr, temp_size, SEEK_CUR) != 0) {  // Skip old data in original
                printf("Error: Could not skip old frame content.\n");
                break;
            }
        }
        else {                                      // For other frames, copy as-is
            int temp_size_be = temp_size;
            convert_big_to_lit(&temp_size_be);

            if (fwrite(&temp_size_be, 1, 4, fp) != 4) {  // Write frame size
                printf("Error: Could not write frame size.\n");
                break;
            }
            if (fwrite(flag, 1, 2, fp) != 2) {           // Write frame flags
                printf("Error: Could not write frame flags.\n");
                break;
            }

            char *frame_content = malloc(temp_size);     // Allocate buffer for frame data
            if (!frame_content) {
                printf("Memory allocation error.\n");
                break;
            }
            if (fread(frame_content, 1, temp_size, fptr) != (size_t)temp_size) { // Read frame data
                printf("Error: Could not read frame content.\n");
                free(frame_content);
                break;
            }
            if (fwrite(frame_content, 1, temp_size, fp) != (size_t)temp_size) { // Write frame data
                printf("Error: Could not write frame content.\n");
                free(frame_content);
                break;
            }
            free(frame_content);                         // Free allocated buffer
        }
    }

    int ch;
    while ((ch = fgetc(fptr)) != EOF) {             // Copy remaining audio data
        if (fputc(ch, fp) == EOF) {
            printf("Error writing audio data.\n");
            break;
        }
    }

    fclose(fptr);                                   // Close original file
    fclose(fp);                                     // Close temp file

    if (remove(file_name) != 0) {                   // Remove original file
        perror("Error deleting original file");
        return;
    }

    if (rename("music.mp3", file_name) != 0) {     // Rename temp file to original filename
        perror("Error renaming temporary file");
        return;
    }

    char opt[20];
    strcpy(opt, tagname(tag_name));                 // Get readable tag name
    display_edit(opt, new_char);                     // Display success message
}
