#include "myroadrunner.h"
int myloop();
void mypacket_handler(int num_received, char* payload, struct sockaddr_in* local,
		struct sockaddr_in* remote);
void send_eof(struct sockaddr_in* remote);

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
	//first byte from client is the index of the packet it fully processed last
	char last_packet_rec = *payload;
	//printf("last packet (from client): %d\n", last_packet_rec);
	remote->sin_port = htons(CLIENT_LISTEN);
	if(req_fd == -1){
		//First request, setup
		strncpy(req_filename, (payload + 1), (size_t) (num_received - 1));
		if(access(req_filename, R_OK) == 0){
			req_fd = open(req_filename, O_RDONLY);
			//printf("found %s, starting to serve\n", req_filename);
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
	//printf("last_packet_rec: %d, packet_index: %d, diff:%d\n", last_packet_rec, packet_index, diff);
	//Mark circle buffer as read
	circ_step(diff*PACKET_SIZE);
	packet_index = (packet_index + diff) % PACKET_IND_LIMIT;
	if(circ_size == 0){
		send_eof(remote);
	}
	char buffer[FRAME_SIZE*(PACKET_SIZE+1)+4] = {0}; //+1 for index 
	int i =0;
	int total_peek = 0;
	int cnt = 0; //count of fully read packets
	int last_packet_sent =last_packet_rec;
	for(i; i<FRAME_SIZE && total_peek < circ_size; i++){
		cnt++;
		//printf("tot peek: %d, circ_size:%d\n", total_peek, circ_size);
		last_packet_sent = (last_packet_rec + i + 1) % PACKET_IND_LIMIT;
		buffer[i*(PACKET_SIZE+1)+4] = last_packet_sent; //+1 because we want the 1 after last_packet_rec
		int payload_offset = 4+i*(PACKET_SIZE+1) + 1;	//+1 to start after the index byte, +4 length field
		int num_peek = circ_peek(buffer+payload_offset, PACKET_SIZE, total_peek);
		total_peek += num_peek;
		//printf("%d: %d peeked\n", (last_packet_rec + i + 1), num_peek);
		if(num_peek < PACKET_SIZE){
			//HIT EOF
			//printf("EOF after %d chars\n", num_peek);
			memset(buffer+payload_offset+num_peek, 0, PACKET_SIZE - num_peek);
			break;
		}
	}
	//printf("peek_tot:%d, i:%d\n", total_peek, i);
	//printf("i:%d, cnt:%d\n", i, cnt);
	//copy number of bytes into the length field
	memcpy(&buffer, &total_peek, sizeof(total_peek));
	send_buffer_sock(*remote, buffer, total_peek+cnt+4); //+4 for length
	int num_read = circ_write(req_fd, diff*PACKET_SIZE);
	//printf("last_packet_sent: %d\n", last_packet_sent);
	if(num_read == 0 && last_packet_rec == last_packet_sent){
		send_eof(remote);
	}

}
void send_eof(struct sockaddr_in* remote){
		//EOF
		//printf("EOF\n");
		char eof_err[2*(PACKET_SIZE+1)] = {0};
		send_buffer_sock(*remote, eof_err, 2*(PACKET_SIZE+1));
		//reset server state
		close(req_fd);
		req_fd = -1;
		packet_index = 0;
		//reset circular buffer
		circ_index = 0;
		circ_size = 0;
		memset(circ_window, 0, CIRC_MAX_SIZE);
}
