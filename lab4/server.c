#include "myfiles.h"

int main(int argc, char **argv){
	if(argc < 2){
		printf("Syntax Error\n");
		printf("usage:\n\tmypingd port\n");
		exit(-1);
	}

	struct sigaction sigact;
	sigact.sa_handler = &zombie_handler;
	sigact.sa_flags = SA_RESTART | SA_NOCLDSTOP;
	sigfillset(&sigact.sa_mask);
	if(sigaction(SIGCHLD, &sigact, NULL) == -1){
		printf("Error setting signal handler\n");
		exit(-1);
	}

	global_port = DEFAULT_PORT;
	sscanf(argv[1], "%d", &global_port);

	struct sockaddr_in server_addr;
	int socket_fd;

	if ((socket_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		printf("Failed to create socket fd\n");
		exit(-1);
	}

	memset((char *)&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(global_port);

	if (bind(socket_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
		printf("Failed to bind socket\n");
		exit(-1);
	}

	if(listen(socket_fd, 5) < 0){
		printf("Failed to listen\n");
		exit(-1);
	}

	struct sockaddr_in client_addr;
	socklen_t addrlen = sizeof(client_addr);
	while(1){
		int new_socket = accept(socket_fd, (struct sockaddr *)&client_addr, &addrlen);
		if(new_socket < 0){
			printf("Failed to accept connection\n");
			exit(-1);
		}
		char buffer[WINDOW_SIZE];
		int num_read = read(new_socket, buffer, WINDOW_SIZE);
		if(access(buffer, R_OK) == 0){
			int pid = fork();
			if(pid == 0){
				int remote_socket = new_socket;
				struct sockaddr_in remote_addr;
				memcpy(&remote_addr, &client_addr, addrlen);
				//TODO READ AND SEND FILE
				exit(0);
			}
		}

	}
}
