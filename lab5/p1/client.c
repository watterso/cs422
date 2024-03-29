#include "myroadrunner.h"
int myloop();
void myhandle_packet(int, char*, struct sockaddr_in* local,
		struct sockaddr_in* remote);
int main(int argc, char **argv){
	if(argc < 5){
		printf("Syntax Error\n");
		printf("usage:\n\tmyroadrunner ip-address port pathname filename\n");
		exit(-1);
	}

	struct sigaction sigact;
	sigact.sa_handler = &packetloss_handler;
	sigact.sa_flags = SA_RESTART;
	sigfillset(&sigact.sa_mask);
	if(sigaction(SIGALRM, &sigact, NULL) == -1){
		printf("Error setting signal handler\n");
		exit(-1);
	}

	//set globals
	global_port = DEFAULT_PORT;
	sscanf(argv[2], "%d", &global_port);

	//printf("len: %d, for: '%s'\n", strlen(argv[3]), argv[3]);
	strcpy(req_filename, argv[3]);
	//printf("file_req:%s\n", req_filename);
	strcat(argv[3], "\0");

	//Send initial request
	char init_req[PACKET_SIZE+1];
	init_req[0] = 0;
	memcpy(init_req+1, req_filename, PACKET_SIZE);
	send_buffer(argv[1], global_port, init_req, PACKET_SIZE+1);

	remote_conn.sin_family = AF_INET;
	remote_conn.sin_addr.s_addr = inet_addr(argv[1]);
	remote_conn.sin_port = htons(global_port);

	myfile = fopen(argv[4], "wb");	
	struct timeval before;
	gettimeofday(&before, NULL);
	//Set alarm for timeout resend
	alarm(PACKETLOSS);
	//Listen for file packets
	mylisten(CLIENT_LISTEN, &myloop, &myhandle_packet);
	fclose(myfile);

	struct timeval after;
	gettimeofday(&after, NULL);

	long int diff_sec = after.tv_sec - before.tv_sec;
	long int diff_usec = after.tv_usec - before.tv_usec;
	float agg_diff_msec = diff_sec * 1000L + diff_usec / 1000.0;
	printf("Took %f milliseconds to copy %s:%s/%s to %s\n", agg_diff_msec, argv[1], argv[2], argv[3], argv[4]);
}

int myloop(){
	return packet_index >= 0 ? 1 : 0;
}

void myhandle_packet(int size, char* payload, struct sockaddr_in* local,
		struct sockaddr_in* remote){
	//data received, disable timeout resend
	alarm(0);
	remote->sin_port = htons(global_port);
	//Number of bytes actually from the file
	int total_peek =0;
	memcpy(&total_peek, payload, sizeof(total_peek));
	//increment past the size field
	payload = payload+4;
	int combined_size = PACKET_SIZE +1;
	//printf("bytes received: %d, total_peek:%d\n", size, total_peek);
	int num_pot_packets = (size-4)/(combined_size);	
	if((size-4)%combined_size>0) num_pot_packets++;
	//All Indices 0's means done	
	int sum_packet_indices = 0;
	int i =0;
	int prev_pack_ind = -1;
	int num_packets = 0;
	//count the number of correctly numbered packets in the payload
	for(i; i<num_pot_packets; i++){
		char pack_ind = payload[i*combined_size];
	//	printf("%d|", pack_ind);
		if(prev_pack_ind!= -1){
			int tmp = (prev_pack_ind+1)%PACKET_IND_LIMIT;
			if(pack_ind != tmp) break; //If the next packet we want isn't there stop checking
			num_packets++;
			prev_pack_ind = tmp;
		}else{
			prev_pack_ind = pack_ind;
			num_packets++;
		}
		sum_packet_indices += pack_ind;
	}
	if(sum_packet_indices == 0){
		//verify the EOF signal
		i = 0;
		int dbl_chk_sum = 0;
		for(i;i<num_pot_packets;i++){
			dbl_chk_sum += payload[i*combined_size];
		}
		if(dbl_chk_sum ==0){
			//SERVER SAYS DONE
			packet_index = -1;
			alarm(0);
			return;
		}
	}
	if((size-4)<=total_peek){
		//last packet got truncated enroute, re-request this one
		num_packets--;
		prev_pack_ind = (prev_pack_ind - 1) % PACKET_IND_LIMIT;
		if(prev_pack_ind<0) prev_pack_ind+=PACKET_IND_LIMIT;
	}
	//printf("received %d packets, last: %d\n", num_packets, prev_pack_ind);
	char latest_packet_ind = (char)prev_pack_ind; 
	packet_index = prev_pack_ind;
	//Send ACK with index of last full packet 
	char buffer[combined_size];
	buffer[0] = latest_packet_ind;
	strcpy(buffer+1, req_filename);
	send_buffer_sock(*remote, buffer, combined_size);
	memcpy(&remote_conn, remote, sizeof(remote_conn));
	alarm(PACKETLOSS);

	i = 0;
	for(i; i<num_packets-1; i++){
		//write all but the last packet
		fwrite(payload+(i*combined_size)+1, PACKET_SIZE, 1, myfile);
	}
	int last_packet_size = 0; 
	if((size-4)<=total_peek){
		//last sent packet was truncated ==> last valid packet is full size
		last_packet_size = PACKET_SIZE;	
	}else{
		//last packet is less than PACKET_SIZE
		last_packet_size = total_peek - (num_packets-1)*PACKET_SIZE;	
	}
	//printf("last packet is %d bytes long\n", last_packet_size);
	fwrite(payload+((num_packets-1)*combined_size)+1, last_packet_size, 1, myfile);
}
