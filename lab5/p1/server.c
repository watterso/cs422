#include "myroadrunner.h"
int myloop();
void mypacket_handler(int num_received, char* payload, struct sockaddr_in* local,
							struct sockaddr_in* remote);

int main(int argc, char **argv){
	if(argc < 2){
		printf("Syntax Error\n");
		printf("usage:\n\tmyroadrunnerd port\n");
		exit(-1);
	}	struct sigaction sigact;
	struct sigaction sigact;
	struct sigaction sigact;
	sigact.sa_handler = &zombie_handler;
	sigact.sa_flags = SA_RESTART | SA_NOCLDSTOP;
	sigfillset(&sigact.sa_mask);
	if(sigaction(SIGCHLD, &sigact, NULL) == -1){
		printf("Error setting signal handler\n");
		exit(-1);
	}

	circ_index = 0;
	circ_size = 0;

	global_port = DEFAULT_PORT;
	sscanf(argv[1], "%d", &global_port);

	mylisten(global_port, &myloop, &mypacket_handler);
}
int myloop(){
	return 1;
}

void mypacket_handler(int num_received, char* payload, struct sockaddr_in* local,
							struct sockaddr_in* remote){
	char packet_req = *payload;
	if(packet_req == 0){
		//First request, setup
		strncpy(req_filename, (payload + 1), (size_t) (num_received - 1));
		req_fd = open(req_filename, O_READ);
		circ_write(req_fd, circ_max_size);
	}
}

