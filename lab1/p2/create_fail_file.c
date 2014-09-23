#include "checksum.h"

void create_fail_file(int bytes_to_write, char* input_file, char* output_file){
	int i_fp = open(input_file, O_RDONLY);
	int o_fp = open(output_file, O_CREAT | O_WRONLY, 0666);
	if(i_fp == -1 || o_fp == -1){
		printf("Error opening %s file\n", o_fp == -1 ? "output" : "input");
		exit(-1);
	}

	char inp_buffer[bytes_to_write];
	int num_read = read(i_fp, inp_buffer, bytes_to_write);
	if(num_read == -1){
		printf("Error reading input file\n");
		exit(-1);
	}
	num_read = write(o_fp, inp_buffer, bytes_to_write);

	int i_close_result = close(i_fp);
	int o_close_result = close(o_fp);
	if(i_close_result == -1 || o_close_result == -1){
		printf("Error closing file\n");
		exit(-1);
	}
}
