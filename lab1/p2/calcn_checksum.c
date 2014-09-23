#include "checksum.h"

int calcn_checksum(unsigned long long* chksum, int max_bytes, char* filename){
	int fp = open(filename, O_RDONLY);
	if(fp == -1){
		printf("Error opening file\n");
		return -1;
	}
	unsigned long long running_chk = 0;
	int bytes_read = 0;

	unsigned int one_byte = 0;
	int num_read = read(fp, &one_byte, 1);
	while(num_read > 0 && bytes_read < max_bytes){
		bytes_read += num_read;
		running_chk += one_byte;
		num_read = read(fp, &one_byte, 1);
	}

	int close_result = close(fp);
	if(close_result == -1){
		printf("Error closing file\n");
		return -1;
	}
	*chksum = running_chk;
	return bytes_read;
}
