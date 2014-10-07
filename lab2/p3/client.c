#include "myping.h"

int myloop_condition();
void mypacket_handler(int size, char* payload, struct sockaddr_in* local,
		struct sockaddr_in* remote);

int main(int argc, char **argv){
	if(argc < 2){
		printf("Syntax Error\n");
		printf("usage:\n\tmpying ip-address port\n");
		exit(-1);
	}

	//set globals
	loop = 1;
	global_port = DEFAULT_PORT;
	sscanf(argv[2], "%d", &global_port);
	gettimeofday(&before, NULL);

	char* buf = "oym8";
	send_buffer(argv[1], global_port, buf, 4, EMPTY_FLAG);
	mylisten(global_port, &myloop_condition, &mypacket_handler);
}

int myloop_condition(){
	return loop; 
}

void mypacket_handler(int size, char* payload, struct sockaddr_in* local,
		struct sockaddr_in* remote){
	char flags = *payload;
	payload++;
	int msg_size = size-1;
	gettimeofday(&after, NULL);
	long int diff_sec = after.tv_sec - before.tv_sec;
	long int diff_usec = after.tv_usec - before.tv_usec;
	float agg_diff_msec = diff_sec * 1000L + diff_usec / 1000.0;
	printf("Time to %s is %f milliseconds\n", inet_ntoa(remote->sin_addr), agg_diff_msec);
	loop = 0;
}
