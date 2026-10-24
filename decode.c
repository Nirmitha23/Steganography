#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "decode.h"
#include "common.h"
#include "types.h"

Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo)
{
    // argv[2] -> .bmp -> store argv[2] in structure decInfo->stego_image_fname = argv[2];
    if (strstr(argv[2], ".bmp") != NULL)
        decInfo->stego_image_fname = argv[2];
    else
        return e_failure;

    // argv[3] -> .txt -> decInfo->secret_fname = argv[3]
    // if (strstr(argv[3], ".txt") != NULL)
    //     strcpy(decInfo->secret_fname, argv[3]);
    // else
    //     return e_failure;
    if(argv[3] != NULL && strstr(argv[3], ".txt") != NULL)
        strcpy(decInfo->secret_fname, argv[3]);
    else
        strcpy(decInfo->secret_fname, "decode.txt");
    
    return e_success;
}

Status open_files_decode(DecodeInfo *decInfo)
{
    // Stego image file
    decInfo->fptr_stego_image = fopen(decInfo->stego_image_fname, "rb");
    if (decInfo->fptr_stego_image == NULL)
    {
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open file %s\n",decInfo->stego_image_fname);
        return e_failure;
    }

    // Secret output file
    decInfo->fptr_secret = fopen(decInfo->secret_fname, "w");
    if (decInfo->fptr_secret == NULL)
    {
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open file %s\n",decInfo->secret_fname);
        return e_failure;
    }

    return e_success;
}

Status decode_size_from_lsb(long *size, DecodeInfo *decInfo)
{
    *size = 0;
    for (int i = 0; i < 32; i++)
    {
        char byte;
        if (fread(&byte, 1, 1, decInfo->fptr_stego_image) != 1)
            return e_failure;

        *size = (*size << 1) | (byte & 1); // read LSB
    }
    return e_success;
}


char decode_byte_from_lsb(char *image_buffer)
{
    char data = 0;
    for (int i = 0; i < 8; i++)
        data = (data << 1) | (image_buffer[i] & 1);
    return data;
}

Status decode_data_from_image(char *data, int size, DecodeInfo *decInfo)
{
    char image_buffer[8];
    for (int i = 0; i < size; i++)
    {
        if (fread(image_buffer, 1, 8, decInfo->fptr_stego_image) != 8)
            return e_failure;
        data[i] = decode_byte_from_lsb(image_buffer);
    }

    return e_success;
}

Status decode_magic_string(char *magic_string, DecodeInfo *decInfo)
{
    decode_data_from_image(magic_string, strlen(MAGIC_STRING), decInfo);
    return e_success;
}

Status decode_secret_file_extn_size(DecodeInfo *decInfo)
{
    decode_size_from_lsb(&decInfo->extension_size, decInfo);
    return e_success;
}

Status decode_secret_file_extn(char *file_extn, DecodeInfo *decInfo)
{
    if(decInfo->extension_size > MAX_FILE_SUFFIX)
        return e_failure; // extension too large

    if(decode_data_from_image(file_extn, decInfo->extension_size, decInfo) != e_success)
        return e_failure;

    file_extn[decInfo->extension_size] = '\0'; // null terminate
    return e_success;
}

Status decode_secret_file_size(DecodeInfo *decInfo)
{
    decode_size_from_lsb(&decInfo->size_secret_file, decInfo);
    return e_success;
}

Status decode_secret_file_data(DecodeInfo *decInfo)
{
    decode_data_from_image(decInfo->secret_data, decInfo->size_secret_file, decInfo);
    return e_success;
}

Status skip_bmp_header(FILE *fptr_stego_image)
{
    if (fseek(fptr_stego_image, 54, SEEK_SET) != 0) // skip 54 bytes of BMP header
    {
        perror("fseek");
        return e_failure;
    }
    return e_success;
}


Status do_decoding(DecodeInfo *decInfo)
{
    if(open_files_decode(decInfo)==e_success){
        printf("Open files is a success \n");
    }else{
        printf("Failed to open files\n");
        return e_failure;
    }

    if(skip_bmp_header(decInfo->fptr_stego_image) != e_success){
        printf("Failed to skip BMP header\n");
        return e_failure;
    }

    //fseek(decInfo->fptr_stego_image, 54, SEEK_SET);
    char magic_string[strlen(MAGIC_STRING) + 1];
    magic_string[strlen(MAGIC_STRING)] = '\0';
    if (decode_magic_string(magic_string, decInfo) == e_success) {
        printf("Decoded Magic String Successfully\n");
    } else {
        printf("Failed to decode magic string\n");
        return e_failure;
    }

    if(decode_secret_file_extn_size(decInfo) == e_success) {
        printf("Decoded secret file extension size successfully\n");
    } else {
        printf("Failed to decode secret file extension size\n");
        return e_failure;
    }

    if(decode_secret_file_extn(decInfo->extn_secret_file, decInfo) == e_success) {
        printf("Decoded secret file extension successfully\n");
    } else {
        printf("Failed to decode secret file extension\n");
        return e_failure;
    }

    if(decode_secret_file_size(decInfo) == e_success) {
        printf("Decoded secret file size successfully\n");
    } else {
        printf("Failed to decode secret file size\n");
        return e_failure;
    }

    decInfo->secret_data = (char *)malloc(decInfo->size_secret_file);
    if(decInfo->secret_data == NULL){
        printf("Memory allocation failed\n");
        return e_failure;
    }

    if(decode_secret_file_data(decInfo) == e_success) {
        printf("Decoded secret file data successfully\n");
    } else {
        printf("Failed to decode secret file data\n");
        return e_failure;
    }

    if(fwrite(decInfo->secret_data, 1, decInfo->size_secret_file, decInfo->fptr_secret) != decInfo->size_secret_file){
        printf("Failed to write secret data to file\n");
        free(decInfo->secret_data);
        return e_failure;
    }
    free(decInfo->secret_data);
    return e_success;
}