#include "checksum.h"

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
	//printf("%d: %llx\n", bytes_read, checksum);
	create_checksum_file(bytes_read, checksum, argv[1], argv[2]);
}
