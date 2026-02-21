#ifndef VIEW_H                   // Include guard start: prevent multiple inclusion
#define VIEW_H

#include <stdio.h>              // Standard I/O library
#include "type.h"               // Custom types and enums


void view_mp3(const char *filename, FileInfo *file_info);       // Prototype to view MP3 tags
Status open_mp3_file(const char *filename, FileInfo *file_info); // Prototype to open MP3 file
Status read_id3v2_tag(FileInfo *file_info, ID3v2Tag *tag_read); // Prototype to read ID3v2 tag data
void convert_big_to_lit(int *size);                            // Prototype to convert big-endian to little-endian
const char* tagname(char tag[]);                               // Prototype to get readable tag name

void print_usage();                                            // Prototype to print usage instructions
void print_help();                                             // Prototype to print help menu

void edit_mp3(char str[], char new_char[], char file_name[]);  // Prototype to edit MP3 tag contents
int isnumeric(char str[]);                                    // Prototype to check if string is numeric
void display_edit(char str[], char new[]);                     // Prototype to display edit confirmation

#endif                                                        // Include guard end
