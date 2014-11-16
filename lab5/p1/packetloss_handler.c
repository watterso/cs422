#include "myroadrunner.h"

void packetloss_handler(int sig){
	printf("PACKETLOSS RESEND REQUEST\n");
	char buffer[PACKET_SIZE+1];
	buffer[0] = packet_index;
	strcpy(buffer+1, req_filename);
	send_buffer_sock(remote_conn, buffer, PACKET_SIZE+1);
	alarm(2);
}
