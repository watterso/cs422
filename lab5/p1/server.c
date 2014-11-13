#include "myroadrunner.h"
int myloop();
void mypacket_handler(int num_received, char* payload, struct sockaddr_in* local,
		struct sockaddr_in* remote);

int main(int argc, char **argv){
	if(argc < 2){
		printf("Syntax Error\n");
		printf("usage:\n\tmyroadrunnerd port\n");
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

	circ_index = 0;
	circ_size = 0;

	req_fd = -1; //NO file requested...yet!

	global_port = DEFAULT_PORT;
	sscanf(argv[1], "%d", &global_port);

	mylisten(global_port, &myloop, &mypacket_handler);
}
int myloop(){
	return 1;
}

void mypacket_handler(int num_received, char* payload, struct sockaddr_in* local,
		struct sockaddr_in* remote){
	char last_packet_rec = *payload;
	if(req_fd == -1){
		//First request, setup
		strncpy(req_filename, (payload + 1), (size_t) (num_received - 1));
		//TODO VERIFY FILE VALID
		req_fd = open(req_filename, O_RDONLY);
		circ_write(req_fd, CIRC_MAX_SIZE);
	}
	//TODO HANDLE EOF (req_fd set to -1)
	int diff = last_packet_rec >= packet_index ? last_packet_rec - packet_index :
		MAX_PACKET_IND - packet_index + last_packet_rec;
	circ_step(diff*PACKET_SIZE);
	char buffer[FRAME_SIZE*(PACKET_SIZE+1)]; //+1 for index 
	int i =0;
	for(i; i<FRAME_SIZE; i++){
		buffer[i*(PACKET_SIZE+1)] = last_packet_rec + i + 1; //+1 because we want the 1 after last_packet_rec
		int payload_offset = i*(PACKET_SIZE+1) + 1;	// +1 to start after the index byte
		circ_peek(buffer+payload_offset, PACKET_SIZE);
	}
	send_buffer_sock(*remote, buffer, FRAME_SIZE*(PACKET_SIZE+1));
	circ_write(req_fd, diff*PACKET_SIZE);

}

