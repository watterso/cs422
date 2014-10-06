#include "reminder.h"

void send_string(char* ip, int port, char* string, int size){
	struct sockaddr_in server_addr;    
	int socket_fd;
	if((socket_fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		printf("Failed to create socket fd\n");
		exit(-1);
	}
	memset((char*)&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = inet_addr(ip);
	server_addr.sin_port = htons(port);

	//printf("sending to %d at %s\n", server_addr.sin_port, inet_ntoa(server_addr.sin_addr));
	if(sendto(socket_fd, string, size, 0, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
		printf("Failed to send packet\n");
		exit(-1);
	}
}
