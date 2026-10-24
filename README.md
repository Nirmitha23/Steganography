# LSB Image Steganography

## Description
LSB (Least Significant Bit) Image Steganography is a C-based project that hides secret messages inside image files by modifying the least significant bits of pixel data. It also supports extracting the hidden message from the encoded image.

## Features
- Encode secret text into an image.
- Decode and retrieve hidden messages.
- Preserve the visual appearance of the original image.
- Perform file handling and binary data manipulation.

## Technologies Used
- C Programming
- File Handling
- Bitwise Operations
- Image Processing
- Command-Line Arguments

## Concepts Covered
- Bitwise operators and bit manipulation
- Structures and functions
- File I/O operations
- Command-line argument handling
- Encoding and decoding techniques

## How It Works
The encoding process stores secret message bits in the least significant bits of image data. The decoding process reads these bits to reconstruct the original message.

## Compilation and Execution
Compile the source files using GCC:

```bash
gcc *.c -o lsb_steganography
```

Run the program with the required command-line arguments according to the implementation:

```bash
./lsb_steganography
```

## Applications
- Hidden message communication
- Data hiding and information security
- Learning bit manipulation and image data processing
