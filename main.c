/*
Name:Vinayak Acharya
Date:19/11/25
Description:This project is a command-line utility developed in C that allows users to read, 
validate, and modify MP3 file metadata (ID3 tags). It interacts directly with the MP3 binary files 
to extract and update metadata fields such as:Title,Artist,Album,Year,Genre,Comment.

Key Features:
    Supports parsing ID3v2.3 tags.
    Displays metadata in a user-friendly format.
    Allows in-place editing of specific tags without corrupting the audio data.
    Validates inputs to ensure data integrity.
    Operates via CLI, making it suitable for scripting and automation.

Purpose:
    The project aims to provide a lightweight, system-level tool for managing MP3 metadata directly through file operations in C.
    It enhances understanding of MP3 file structures, binary data handling, and file I/O programming in C.

Real-world Application:
    Building a custom metadata editor for music collections.
    Extending to support batch editing or interaction with other audio formats.
    Used as a learning project for system programming, file parsing, and binary data manipulation.


*/
#include <stdio.h>             // Include standard Input/Output library
#include <string.h>            // Include string handling library
#include "type.h"              // Include custom type definitions
#include "view.h"              // Include view-related functions and definitions

void print_usage() {
    printf("USAGE:\n");         // Print usage header
    printf("To view  : ./a.out -v mp3filename\n");        // Instruction for viewing
    printf("To edit  : ./a.out -e -t/-a/-A/-y/-m/-c changing_text mp3filename\n");  // Instruction for editing
    printf("To get help: ./a.out --help\n");               // Instruction for help
}

void print_help() {
    printf("------ HELP MENU ------\n");                   // Print help menu header
    printf("1. -v -> to view mp3 file contents\n");       // Help detail: view
    printf("2. -e -> to edit mp3 file contents\n");       // Help detail: edit
    printf("  2.1. -t -> to edit song title\n");           // Help detail: edit song title
    printf("  2.2. -a -> to edit artist name\n");          // Help detail: edit artist name
    printf("  2.3. -A -> to edit album name\n");           // Help detail: edit album name
    printf("  2.4. -y -> to edit year\n");                  // Help detail: edit year
    printf("  2.5. -m -> to edit music\n");                 // Help detail: edit music
    printf("  2.6. -c -> to edit comment\n");               // Help detail: edit comment
    printf("-----------------------\n");                      // End of help menu
}

OperationType check_operation_type(char *symbol) {          // Determine the operation type from argument
    if (strcmp(symbol, "-v") == 0)                          // If argument is "-v"
        return view;                                        // Return view operation
    else if (strcmp(symbol, "-e") == 0)                     // If argument is "-e"
        return edit;                                        // Return edit operation
    else                                                    // Otherwise
        return unsupported;                                 // Return unsupported
}

int main(int argc, char *argv[]) {
    printf("\n--------------------MP3 Tag Reader and Editor for ID3v2--------------------\n"); // Print program banner
    if (argc < 2) {                                          // If insufficient arguments
        printf("ERROR: Invalid arguments!\n");              // Print error
        print_usage();                                       // Show usage
        return 1;                                           // Exit with error
    }

    if (strcmp(argv[1], "--help") == 0) {                   // If user requests help
        print_help();                                       // Show help menu
        return 0;                                           // Exit normally
    }

    OperationType op = check_operation_type(argv[1]);        // Determine operation based on argument

    if (op == view) {                                        // If operation is view
        if (argc != 3) {                                     // If argument count incorrect
            printf("ERROR: Invalid arguments for view!\n"); // Print error
            print_usage();                                   // Show usage
            return 1;                                       // Exit with error
        }

        int len = strlen(argv[2]);                           // Get length of filename
        if (len >= 4 && strcmp(argv[2] + len - 4, ".mp3") == 0) { // Check if ends with ".mp3"
            printf("Viewing contents of: %s\n", argv[2]);   // Print file being viewed
            FileInfo file_info;                              // Declare FileInfo struct
            view_mp3(argv[2], &file_info);                   // Call function to view mp3 tags
        } else {
            printf("Error: It's not an mp3 file.\n");       // Print file type error
            return 1;                                       // Exit with error
        }
    } 
    else if (op == edit) {                                   // If operation is edit
        if (argc != 5) {                                     // Check argument count for edit
            printf("ERROR: Invalid arguments for edit!\n"); // Print error
            print_usage();                                   // Show usage
            return 1;                                       // Exit with error
        }

        int len1 = strlen(argv[4]);                          // Get length of filename argument
        if (!(len1 >= 4 && strcmp(argv[4] + len1 - 4, ".mp3") == 0)) { // Check for ".mp3"
            printf("Error: It's not an mp3 file.\n");       // Error if not mp3
            return 1;                                       // Exit with error
        }

        printf("Editing: %s\n", argv[4]);                    // Inform user about file being edited

        edit_mp3(argv[2], argv[3], argv[4]);                 // Call edit function with flags and new text
    } 
    else {                                                  // Unsupported operation
        printf("ERROR: Unsupported operation!\n");         // Print error
        print_usage();                                     // Show usage
        return 1;                                         // Exit with error
    }

    return 0;                                               // Return success
}
