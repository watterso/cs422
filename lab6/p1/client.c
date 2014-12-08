#include "mystream.h"

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
	//TODO read from socket_fd for ok

	//TODO UDP receive and send diagnostic
	;
}
