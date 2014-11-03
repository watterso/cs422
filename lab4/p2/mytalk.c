#include "mytalk.h"

int myloop_condition();
void mypacket_handler(int size, char* payload, struct sockaddr_in* local,
											struct sockaddr_in* remote);

int main(int argc, char **argv){
	if(argc < 2){
		printf("Syntax Error\n");
		printf("usage:\n\tmytalk port\n");
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

	struct sigaction sigact1;
	sigact1.sa_handler = &ack_handler;
	sigact1.sa_flags = SA_RESTART;
	sigfillset(&sigact1.sa_mask);
	if(sigaction(SIGALRM, &sigact1, NULL) == -1){
		printf("Error setting signal handler\n");
		exit(-1);
	}

	global_port = DEFAULT_PORT;
	sscanf(argv[1], "%d", &global_port);
	
	int mypid = fork();
	if(mypid == 0){
		mylisten(global_port, &myloop_condition, &mypacket_handler);
		exit(0);
	}else{
		user_interact();	
	}
}

int myloop_condition(){
	return 1;
}

void mypacket_handler(int size, char* payload, struct sockaddr_in* local,
											struct sockaddr_in* remote){
	//skip flag byte
	int msg_size = size-1;
	char* msg = payload+1;
	if(peer_connected){
		alert_user(msg);
	}else{
		//TODO parse and respond
	}
}
