#ifndef DECODE_H
#define DECODE_H

#include "types.h"   // Contains user defined types
#include <stdio.h>

/*
 * Structure to store information required for
 * decoding secret file from stego Image
 * Info about output and intermediate data is
 * also stored
 */

#define MAX_SECRET_BUF_SIZE 1
#define MAX_IMAGE_BUF_SIZE (MAX_SECRET_BUF_SIZE * 8)
#define MAX_FILE_SUFFIX 4

typedef struct _DecodeInfo
{
   /* Source Image info */
    char *src_image_fname;
    FILE *fptr_src_image;
    uint image_capacity;
    uint bits_per_pixel;
    char image_data[MAX_IMAGE_BUF_SIZE];

    /* Secret File Info */
    char secret_fname[20];
    FILE *fptr_secret;
    char extn_secret_file[MAX_FILE_SUFFIX+1];
    long size_secret_file;
    char *secret_data;
    char *magicstring;
    long extension_size;

    /* Stego Image Info */
    char *stego_image_fname;
    FILE *fptr_stego_image;

} DecodeInfo;


/* Decoding function prototypes */

/* Check operation type */
OperationType check_operation_type(char *argv[]); 

/* Read and validate Decode args from argv */
Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo);

/* Perform the decoding */
Status do_decoding(DecodeInfo *decInfo);

/* Get File pointers for i/p and o/p files */
Status open_files_decode(DecodeInfo *decInfo);

/* Decode magic string */
Status decode_magic_string(char *magic_string,DecodeInfo *decInfo);

/*Decode secret file extension size*/
Status decode_secret_file_extn_size(DecodeInfo *decInfo);

/* Decode size */
Status decode_size_from_lsb(long *size, DecodeInfo *decInfo);

/* Decode secret file extension */
Status decode_secret_file_extn(char *file_extn,DecodeInfo *decInfo);

/* Decode secret file size */
Status decode_secret_file_size(DecodeInfo *decInfo);

/* Decode secret file data */
Status decode_secret_file_data(DecodeInfo *decInfo);

/* Decode function, which does the real decoding */
Status decode_data_from_image(char *data, int size,DecodeInfo *decInfo);

/* Decode a byte from LSB of image data array */
char decode_byte_from_lsb(char *image_buffer);

/* Skip BMP header */
Status skip_bmp_header(FILE *fptr_stego_image);


#endif
