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
	remote->sin_port = htons(global_port);
	if(req_fd == -1){
		//First request, setup
		strncpy(req_filename, (payload + 1), (size_t) (num_received - 1));
		if(access(req_filename, R_OK) == 0){
			req_fd = open(req_filename, O_RDONLY);
			printf("found %s, starting to serve\n", req_filename);
			circ_write(req_fd, CIRC_MAX_SIZE);
		}else{
			//SEND ERROR
			char eof_err[FRAME_SIZE*(PACKET_SIZE+1)] = {0};
			send_buffer_sock(*remote, eof_err, FRAME_SIZE*(PACKET_SIZE+1));
			return;
		}
	}
	int diff = last_packet_rec >= packet_index ? last_packet_rec - packet_index :
		PACKET_IND_LIMIT - packet_index + last_packet_rec;
	circ_step(diff*PACKET_SIZE);
	char buffer[FRAME_SIZE*(PACKET_SIZE+1)]; //+1 for index 
	int i =0;
	int num_peek = PACKET_SIZE;
	for(i; i<FRAME_SIZE; i++){
		buffer[i*(PACKET_SIZE+1)] = (last_packet_rec + i + 1) % PACKET_IND_LIMIT; //+1 because we want the 1 after last_packet_rec
		int payload_offset = i*(PACKET_SIZE+1) + 1;	// +1 to start after the index byte
		num_peek = circ_peek(buffer+payload_offset, PACKET_SIZE);
		if(num_peek < PACKET_SIZE){
			//HIT EOF
			printf("EOF after %d chars\n", num_peek);
			memset(buffer+payload_offset+num_peek, 0, PACKET_SIZE - num_peek);
			break;
		}
	}
	printf("last packet sent: %d\n", (last_packet_rec + i + 1) % PACKET_IND_LIMIT); 
	send_buffer_sock(*remote, buffer, FRAME_SIZE*(PACKET_SIZE+1));
	
	int num_read = circ_write(req_fd, diff*PACKET_SIZE);
	if(num_read == 0 && last_packet_rec == packet_index){
		//EOF
		char eof_err[FRAME_SIZE*(PACKET_SIZE+1)] = {0};
		send_buffer_sock(*remote, eof_err, FRAME_SIZE*(PACKET_SIZE+1));
		//reset server state
		close(req_fd);
		req_fd = -1;
		packet_index = 0;
		//reset circular buffer
		circ_index = 0;
		circ_size = 0;
		memset(circ_window, 0, CIRC_MAX_SIZE);
	}

}

