#include "checksum.h"

int main(int argc, char** argv){
	if(argc < 3){
		printf("Syntax Error\n");
		printf("usage:\n\tmychecksum1 in_filename out_filename\n");
		exit(-1);
	}
	int bytes_read = bytes_in_file(argv[1]);
	if(bytes_read == -1){
		printf("Error calculating file size\n");
		exit(-1);
	}
//	printf("%d bytes large\n", bytes_read);
	unsigned long long checksum;
	int chksum_bytes_read = calcn_checksum(&checksum, bytes_read - 8, argv[1]);
	if(chksum_bytes_read != bytes_read - 8){
		printf("Did not use the right number of bytes to calculate the checksum\n");
		printf("%d vs %d\n", chksum_bytes_read, bytes_read-8);
		exit(-1);
	}
	unsigned long long chksum_ext = extract_checksum(bytes_read - 8, argv[1]);
	if(checksum != chksum_ext){
		create_fail_file(bytes_read - 8, argv[1], argv[2]);
	}
	printf("Checksums %s\n", checksum==chksum_ext ? "match!" : "do NOT match :(");
	printf("Calculated checksum: %llx  checksum parsed from file: %llx\n", checksum, chksum_ext);
}
