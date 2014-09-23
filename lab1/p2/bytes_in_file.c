#include "checksum.h"

int bytes_in_file(char* filename){
	int fp = open(filename, O_RDONLY);
	if(fp == -1){
		printf("Error opening file\n");
		return -1;
	}
	int bytes_read = 0;
	unsigned int one_byte = 0;
	int num_read = read(fp, &one_byte, 1);
	while(num_read > 0){
		bytes_read += num_read;
		num_read = read(fp, &one_byte, 1);
	}
	int close_result = close(fp);
	if(close_result == -1){
		printf("Error closing file\n");
		return -1;
	}
	return bytes_read;
}

