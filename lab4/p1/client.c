#include "myfile.h"

int main(int argc, char **argv){
	if(argc < 5){
		printf("Syntax Error\n");
		printf("usage:\n\tfsclient ip-address port pathname filename\n");
		exit(-1);
	}

	//set globals
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
	//printf("len: %d, for: '%s'\n", strlen(argv[3]), argv[3]);
	strcat(argv[3], "\0");
	//Send file request
	write(socket_fd, argv[3], strlen(argv[3])+1);
	int out_file = open(argv[4], O_RDWR | O_CREAT, 0666);
	//printf("out_file: %d\n", out_file);
	//Read from socket and save to file
	copy_file(socket_fd, out_file);
	close(out_file);
	gettimeofday(&after, NULL);

	long int diff_sec = after.tv_sec - before.tv_sec;
	long int diff_usec = after.tv_usec - before.tv_usec;
	float agg_diff_msec = diff_sec * 1000L + diff_usec / 1000.0;
	printf("Took %f milliseconds to copy %s:%s/%s to %s\n", agg_diff_msec, argv[1], argv[2], argv[3], argv[4]);
}
