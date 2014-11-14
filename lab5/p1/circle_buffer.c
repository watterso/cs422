#include "myroadrunner.h"

//Read n bytes from file descriptor fd and insert into circular buffer 
int circ_write(int fd, int n){
	int to_write = n > circ_free_space() ? circ_free_space() : n;
	if(to_write == 0) return 0;

	int offset = (circ_index + circ_size) % CIRC_MAX_SIZE;
	char buff[to_write];
	int num_read = read(fd, buff, to_write);

	int i = 0;
	for(i; i<num_read; i++){
		circ_window[offset + i] = buff[i];
	}
	circ_size += num_read;
	printf("read in %d\n", num_read);
	return num_read;

}
//Read from circular buffer without incrementing circ_index
int circ_peek(char* target, int n, int offset){
	int i = 0;
	int cnt = 0;
	int max = n > circ_size ? circ_size : n;
	if(offset > circ_size) return 0;
	for(i; i<n && i<circ_size; i++){
		int loc = (circ_index+offset + i) % CIRC_MAX_SIZE;
		target[i] = circ_window[loc];
		cnt++;
	}
	return cnt;
}

//Step circ_index n times
void circ_step(int n){
	circ_index = (circ_index + n) % CIRC_MAX_SIZE;
	circ_size = n > circ_size ? 0 : circ_size - n;
	printf("step %d times, size now %d\n", n, circ_size);
}

//Returns the number of free bytes in the buffer
int circ_free_space(){
	return CIRC_MAX_SIZE - circ_size;
}
