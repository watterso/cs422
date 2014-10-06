#include "reminder.h"

void mylisten(int port_number, int (*loop_condition)(),
						void (*handle_packet)(int, char*, struct sockaddr_in*,
						struct sockaddr_in*)){
	
	struct sockaddr_in server_addr;
	int socket_fd;

	if ((socket_fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		printf("Failed to create socket fd\n");
		exit(-1);
	}

	memset((char *)&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(port_number);

	if (bind(socket_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
		printf("Failed to bind socket\n");
		exit(-1);
	}

	char payload[PAYLOAD_SIZE];
	int bytes_received;
	struct sockaddr_in client_addr;
	socklen_t addrlen = sizeof(client_addr);
	printf("Now listing on port %d\n", port_number);
	while(loop_condition()){
		bytes_received = recvfrom(socket_fd, payload, PAYLOAD_SIZE, 0,
			(struct sockaddr *) &client_addr, &addrlen);
		if (bytes_received > 0) {
			//printf("received payload: \"%s\"\n", payload);
			handle_packet(bytes_received, payload, &server_addr, &client_addr);
			//sendto(socket_fd, DUMB_MSG, strlen(DUMB_MSG), 0, (struct sockaddr *) &client_addr, addrlen);
		}
	}
	close(socket_fd);
}
