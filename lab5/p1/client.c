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
	gettimeofday(&before, NULL);

	char buf[PAYLOAD_SIZE];
	//printf("len: %d, for: '%s'\n", strlen(argv[3]), argv[3]);
	strcat(argv[3], "\0");
	strcpy(req_filename, argv[3]);
	//TODO SEND INITIAL REQUEST

	mylisten(global_port, &myloop, &myhandle_packet);

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
	//Integer division truncating means incomplete packets go unnoticed
	int num_packets = size/(combined_size);	
	char latest_packet_ind = payload[(num_packets-1)*combined_size]; //-1 for 0 index
	//All Indices 0's means done	
	int sum_packet_indices = 0;
	int i =0;
	for(i; i<num_packets; i++){
		sum_packet_indices += payload[i*combined_size];
	}
	if(sum_packet_indices == 0){
		//SERVER SAYS DONE
		packet_index = -1;
		return;
	}
	//Send ACK with last seen index
	char buffer[combined_size];
	buffer[0] = latest_packet_ind;
	strcpy(buffer+1, req_filename);
	send_buffer_sock(*remote, buffer, combined_size);
	//TODO WRITE TO FILE
	//ONLY WRITE COMPLETE PACKETS
}
