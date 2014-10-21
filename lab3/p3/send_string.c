#include "vpn.h"

void send_raw(char* ip, int port, char* buffer, int size){
	struct sockaddr_in server_addr;    
	int socket_fd;
	//printf("args - %s:%d \n", ip, port);
	if((socket_fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		printf("Failed to create socket fd\n");
		exit(-1);
	}
	memset((char*)&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = inet_addr(ip);
	server_addr.sin_port = htons(port);

	//printf("sending to %d at %s\n", server_addr.sin_port, inet_ntoa(server_addr.sin_addr));
	if(sendto(socket_fd, buffer, size+1, 0, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
		printf("Failed to send packet, %s\n", strerror(errno));
		exit(-1);
	}
}
void send_string(char* ip, int port, char* string, int size, char flags){
	char buffer[size+1];
	buffer[0] = flags;
	strncpy(buffer+1, string, size);
	send_raw(ip, port, buffer, size+1);
}
