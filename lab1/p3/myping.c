#include "myping.h"

int main(int argc, char** argv){
	if(argc < 3){
		printf("Syntax Error\n");
		printf("usage:\n\tmyping ip port\n");
		exit(-1);
	}
	int port_number;
	sscanf(argv[2], "%d", &port_number);

	struct sockaddr_in server_addr;    
	int socket_fd;
	if((socket_fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		printf("Failed to create socket fd\n");
		exit(-1);
	}
	memset((char*)&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(port_number);
	memcpy((void *)&server_addr.sin_addr, argv[1], strlen(argv[1]));

	int payload[PAYLOAD_SIZE] = {0};

	if(sendto(socket_fd, payload, PAYLOAD_SIZE, 0, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
		printf("Failed to send packet\n");
		exit(-1);
	}else{
		struct timeval before;
		gettimeofday(&before, NULL);
		int bytes_received = recvfrom(socket_fd, payload, PAYLOAD_SIZE, 0, NULL, NULL);
		struct timeval after;
		gettimeofday(&after, NULL);
		printf("%ld\n", after.tv_sec-before.tv_sec);
	}
}
