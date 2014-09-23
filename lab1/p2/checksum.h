#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int bytes_in_file(char* filename);
int calc_checksum(unsigned long long* chksum, char* filename);
int calcn_checksum(unsigned long long* chksum, int max_bytes, char* filename);
void create_checksum_file(int bytes_to_write, unsigned long long checksum, char* input_file, char* output_file);
void create_fail_file(int bytes_to_write, char* input_file, char* output_file);
unsigned long long extract_checksum(int start_index, char* filename);
