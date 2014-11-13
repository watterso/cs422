#include "myroadrunner.h"

//Read n bytes from file descriptor fd and insert into circular buffer 
int circ_write(int fd, int n){
	int to_write = n > circ_free_space() ? circ_free_space() : n;
	if(to_write == 0) return 0;

	int offset = (circ_index + circ_size) % CIRC_MAX_SIZE;
	char buff[to_write];
	int num_read = read(fd, buff, to_write);

	int i = 0;
	for(i; i<to_write; i++){
		circ_window[offset + i] = buff[i];
	}
	circ_size += to_write;
	return to_write;

}
//Read from circular buffer without incrementing circ_index
int circ_peek(char* target, int n){
	int i = 0;
	int offset = (circ_index + circ_size) % CIRC_MAX_SIZE;
	for(i; i<n; i++){
		target[i] = circ_window[(offset + i) % CIRC_MAX_SIZE];
	}
	return n;
}

//Step circ_index n times
void circ_step(int n){
	circ_index = (circ_index + n) % CIRC_MAX_SIZE;
	circ_size = n > circ_size ? 0 : circ_size - n;
}

//Returns the number of free bytes in the buffer
int circ_free_space(){
	return CIRC_MAX_SIZE - circ_size;
}
