#ifndef DECODE_H
#define DECODE_H

#include "types.h" 
#include"common.h"


#define MAX_SECRET_BUF_SIZE 1
#define MAX_IMAGE_BUF_SIZE (MAX_SECRET_BUF_SIZE * 8)
#define DMAX_FILE_SUFFIX 15


class DecodeInfo
{
    private:
	char* stego_src_name;
	FILE* fptr_stego_image;

	char output_fname[DMAX_FILE_SUFFIX];
	FILE* fptr_output;
	long dsize_secret_file;
	long dsize_secret_file_data;
	char *extn_output_file;
	
    const char *password=MAGIC_STRING;

	Status open_files_decode();

	Status decode_magic_string(const char * data);

	char decode_byte_from_lsb(char* buffer);

	int decode_size_from_lsb(char* buffer);

	Status decode_extension_size();

	Status decode_secret_file_size();

	Status decode_secret_extension();

	Status decode_secret_file_data();
   

    public:
	OperationType check_operation_type(char *argv[]);

	Status read_and_validate_decode_args(char *argv[]);

	Status do_decoding();
};

#endif
