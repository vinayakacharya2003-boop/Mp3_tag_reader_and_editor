# Mp3_tag_reader_and_editor
This project is a command-line utility developed in C that allows users to read,  validate, and modify MP3 file metadata (ID3 tags). It interacts directly with the MP3 binary files  to extract and update metadata fields such as:Title,Artist,Album,Year,Genre,Comment.
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
