#include "myping.h"

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

	struct sockaddr_in local_addr;
	int socket_fd;

	if ((socket_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		printf("Failed to create socket fd\n");
		exit(-1);
	}

	memset((char *)&local_addr, 0, sizeof(local_addr));
	local_addr.sin_family = AF_INET;
	inet_aton(argv[1], &local_addr.sin_addr);
	local_addr.sin_port = htons(global_port);

	if(connect(socket_fd, (struct sockaddr *)&local_addr, sizeof(local_addr)) < 0){
		printf("Failed to connect socket: %s\n", strerror(errno));
		exit(-1);
	}
	char buf[PAYLOAD_SIZE];
	int num_read = read(socket_fd, buf, PAYLOAD_SIZE);
	gettimeofday(&after, NULL);

	long int diff_sec = after.tv_sec - before.tv_sec;
	long int diff_usec = after.tv_usec - before.tv_usec;
	float agg_diff_msec = diff_sec * 1000L + diff_usec / 1000.0;
	printf("Time to %s is %f milliseconds\n", argv[1], agg_diff_msec);
}
