#include "myping.h"

int myloop_condition();
void mypacket_handler(int size, char* payload, struct sockaddr_in* local,
											struct sockaddr_in* remote);

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
	
	mylisten(global_port, &myloop_condition, &mypacket_handler);
}

int myloop_condition(){
	return 1;
}

void mypacket_handler(int size, char* payload, struct sockaddr_in* local,
											struct sockaddr_in* remote){
	//skip flag byte
	int msg_size = size-1;
	char* msg = payload+1;
	int pid = fork();
	if(pid == 0){
		char* payload = "REKT"; 
		send_buffer(inet_ntoa(remote->sin_addr), global_port, payload, 4, EMPTY_FLAG);
		exit(0);
	}
}
