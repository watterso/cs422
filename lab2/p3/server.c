#include "reminder.h"

int myloop_condition();
void mypacket_handler(int size, char* payload, struct sockaddr_in* local,
											struct sockaddr_in* remote);

int main(int argc, char **argv){
	if(argc < 4){
		printf("Syntax Error\n");
		printf("usage:\n\tmobilereminderd filename port secret\n");
		exit(-1);
	}

	struct sigaction sigact;
	sigact.sa_handler = &reminder_handler;
	sigact.sa_flags = SA_RESTART;
	sigfillset(&sigact.sa_mask);
	if(sigaction(SIGALRM, &sigact, NULL) == -1){
		printf("Error setting signal handler\n");
		exit(-1);
	}

	global_port = DEFAULT_PORT;
	sscanf(argv[2], "%d", &global_port);
	
	secret_len = (int) strlen(argv[3]);
	strncpy(secret, argv[3], PAYLOAD_SIZE);

	mylisten(global_port, &myloop_condition, &mypacket_handler);
}

int myloop_condition(){
	return 1;
}

void mypacket_handler(int size, char* payload, struct sockaddr_in* local,
											struct sockaddr_in* remote){
	int msg_size = size-1;
	char* msg = payload+1;
	//TODO fork etc
}
