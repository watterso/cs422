#include "myfile.h"

void copy_file(int in_file, int out_file){
	char buffer[WINDOW_SIZE];
	int num_read = read(in_file, buffer, WINDOW_SIZE);
	fcntl(in_file, F_SETFL, O_NONBLOCK);
	if(strncmp(buffer, "INVALID REQUEST", 15)==0){
		printf("len:%d, %s\n", strlen(buffer), buffer);
		return;
	}
	int num_write;
	while(num_read>0){
		//printf("copying(%d)......\n", num_read);
		//printf("%.*s\n", num_read, buffer);
		num_write = write(out_file, buffer, num_read);
		//printf("wrote(%d)\n", num_write);
		num_read = read(in_file, buffer, WINDOW_SIZE);
		//printf("read(%d)\n", num_read);
	}
	printf("Copying finished!\n");
}
