#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

int calc_checksum(unsigned long long* chksum, char* filename);
void create_checksum_file(int bytes_to_write, unsigned long long checksum, char* input_file, char* output_file);

int main(int argc, char** argv){
	if(argc < 3){
		printf("Syntax Error\n");
		printf("usage:\n\tmychecksum1 in_filename out_filename\n");
		exit(-1);
	}
	unsigned long long checksum;
	int bytes_read = calc_checksum(&checksum, argv[1]);
	if(bytes_read == -1){
		printf("Error calculating checksum\n");
		exit(-1);
	}
	printf("%d: %llx\n", bytes_read, checksum);
	create_checksum_file(bytes_read, checksum, argv[1], argv[2]);
}
int calc_checksum(unsigned long long* chksum, char* filename){
	int fp = open(filename, O_RDONLY);
	if(fp == -1){
		printf("Error opening file\n");
		return -1;
	}
	unsigned long long running_chk = 0;
	int bytes_read = 0;

	unsigned int one_byte = 0;
	int num_read = read(fp, &one_byte, 1);
	while(num_read > 0){
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
void create_checksum_file(int bytes_to_write, unsigned long long checksum, char* input_file, char* output_file){
	int i_fp = open(input_file, O_RDONLY);
	int o_fp = open(output_file, O_CREAT | O_WRONLY);
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
	int chksum_buffer[8];
	int i = 0;
	printf("checksum: %lld, %llx\n", checksum, checksum);
	for(i; i<8; i++){
		//TODO: Big Endian?
		chksum_buffer[i] = (checksum >> (8-i-1)*8) & 0xff;
		printf("byte %d: %x | %u\n", i, chksum_buffer[i], chksum_buffer[i]);
	}
	num_read = write(o_fp, chksum_buffer, 8);
	
	int i_close_result = close(i_fp);
	int o_close_result = close(o_fp);
	if(i_close_result == -1 || o_close_result == -1){
		printf("Error closing file\n");
		exit(-1);
	}
}
