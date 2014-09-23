#include "checksum.h"

void create_checksum_file(int bytes_to_write, unsigned long long checksum, char* input_file, char* output_file){
	int i_fp = open(input_file, O_RDONLY);
	int o_fp = open(output_file, O_CREAT | O_RDWR, 0666);
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
	char chksum_buffer[8];
	int i;
	//printf("checksum: %lld, %llx\n", checksum, checksum);
	for(i = 0; i<8; i++){
		chksum_buffer[i] = (checksum >> (8-i-1)*8);
		//printf("byte %d: %x | %u\n", i, chksum_buffer[i], chksum_buffer[i]);
	}
	//printf("%x, %x\n", chksum_buffer[0], chksum_buffer[1]);
	num_read = write(o_fp, chksum_buffer, 8);

	int i_close_result = close(i_fp);
	int o_close_result = close(o_fp);
	if(i_close_result == -1 || o_close_result == -1){
		printf("Error closing file\n");
		exit(-1);
	}
}

