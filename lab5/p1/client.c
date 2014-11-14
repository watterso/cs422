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

	//set globals
	global_port = DEFAULT_PORT;
	sscanf(argv[2], "%d", &global_port);

	//printf("len: %d, for: '%s'\n", strlen(argv[3]), argv[3]);
	strcat(argv[3], "\0");
	strcpy(req_filename, argv[3]);

	//Send initial request
	char init_req[PACKET_SIZE+1];
	init_req[0] = 0;
	memcpy(init_req+1, req_filename, PACKET_SIZE);
	send_buffer(argv[1], global_port, init_req, PACKET_SIZE+1);

	myfile = fopen(argv[4], "wb");	
	struct timeval before;
	gettimeofday(&before, NULL);
	//Listen for file packets
	mylisten(global_port, &myloop, &myhandle_packet);
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
	int combined_size = PACKET_SIZE +1;
	printf("bytes received: %d\n", size);
	//Integer division truncating means incomplete packets go unnoticed
	int num_pot_packets = size/(combined_size);	
	//All Indices 0's means done	
	int sum_packet_indices = 0;
	int i =0;
	int prev_pack_ind = -1;
	int num_packets = 0;
	for(i; i<num_pot_packets; i++){
		char pack_ind = payload[i*combined_size];
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
		i = 0;
		int dbl_chk_sum = 0;
		for(i;i<num_pot_packets;i++){
			dbl_chk_sum += payload[i*combined_size];
		}
		if(dbl_chk_sum ==0){
			//SERVER SAYS DONE
			packet_index = -1;
			return;
		}
	}
	printf("received %d packets, last: %d\n", num_packets, prev_pack_ind);
	char latest_packet_ind = (char)prev_pack_ind; //-1 for 0 index
	packet_index = prev_pack_ind;
	//Send ACK with last seen index
	char buffer[combined_size];
	buffer[0] = latest_packet_ind;
	strcpy(buffer+1, req_filename);
	send_buffer_sock(*remote, buffer, combined_size);

	i = 0;
	for(i; i<num_packets; i++){
		char c = *(payload+(i*combined_size)+PACKET_SIZE);
		int j = 0;
		while(c==0 && j<PACKET_SIZE){
			c = *(payload+(i*combined_size)+PACKET_SIZE-j);
			if(c==0) j++;
		}
		printf("packet is %d bytes long\n", PACKET_SIZE-j);
		fwrite(payload+(i*combined_size)+1, PACKET_SIZE-j, 1, myfile);
	}
}
