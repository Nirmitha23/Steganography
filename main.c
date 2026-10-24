#include <stdio.h>
#include <string.h>
#include "encode.h"
#include "decode.h"
#include "common.h"
#include "types.h"

OperationType check_operation_type(char *argv[])
{
    if(strcmp(argv[1],"-e")==0)
       return e_encode;
    else if(strcmp(argv[1],"-d")==0)
       return e_decode;
    else  
       return e_unsupported; 
}

int main(int argc, char *argv[])
{
    //res = check_operationType(argv);
    //res -> e_encode -> read_and_validate_encodde_arg(); -> do_encoding();
    //res -> e_decode -> read_and_validate_decode_arg();  -> do_decoding();
    //res -> e_usupported -> error -> terminate
    if(argc<2){
        printf("Invalid argument\n");
        printf("For encoding: ./a.out -e beautiful.bmp secrect.txt [stego.bmp]\n");
        printf("For decoding : ./a.out -d stego.bmp [decode.txt\n]");
        return 0;
    }
    if(check_operation_type(argv) == e_encode){
        if(argc<4){
            printf("Not enough arguments\n");
            return 0;
        }
        printf("Selected Encoding\n");
        EncodeInfo encode;
        if(read_and_validate_encode_args(argv,&encode) == e_success){
            printf("Read and Validate encode arguments is successful\n");
            printf("--------------Encoding started--------------------------\n");
            if(do_encoding(&encode)==e_success){
                printf("Encoding Successfull\n");
            }else{
                printf("Encoding Failed\n");
            }
        }
    }else if(check_operation_type(argv) == e_decode){
        if(argc<4){
            printf("Not enough arguments\n");
            return 0;
        }
        printf("Selected Decoding\n");
        DecodeInfo decode;
        if(read_and_validate_decode_args(argv,&decode) == e_success){
            printf("Read and Validate encode arguments is successful\n");
            printf("--------------Decoding started--------------------------\n");
            if(do_decoding(&decode)==e_success){
                printf("Decoding Successfull\n");
            }else{
                printf("Decoding Failed\n");
            }
        }
    }else{
        printf("Invalid argument\n");
        printf("For encoding: ./a.out -e beautiful.bmp secrect.txt [stego.bmp]\n");
        printf("For decoding : ./a.out -d stego.bmp [decode.txt\n]");
    }
    return 0;
}
