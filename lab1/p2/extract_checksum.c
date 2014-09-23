#include "checksum.h"

unsigned long long extract_checksum(int start_index, char * filename){
	int fp = open(filename, O_RDONLY);
	if(fp == -1){
		printf("Error opening file\n");
		return -1;
	}
	unsigned long long chksum = 0;
	unsigned int offset_buff[start_index];
	unsigned int one_byte = 0;
	int num_read = read(fp, &offset_buff, start_index);
	//  printf("num_read: %d\n", num_read);
	int i;
	for(i = 0; i<8; i++){
		num_read = read(fp, &one_byte, 1);
		//Little Endian!
		unsigned long long shifted_byte = (one_byte << (8-i-1)*8);
		//      printf("byte %d (%d): %llx | %llu\n", i, one_byte, shifted_byte, shifted_byte);
		chksum += shifted_byte;
	}
	int close_result = close(fp);
	if(close_result == -1){
		printf("Error closing file\n");
		return -1;
	}
	return chksum;
}
