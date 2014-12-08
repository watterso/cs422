#include "mystream.h"

void udp_audio(char* file_path, char* addr, int port, int payload_size, int spacing, int mode, char* log_file){
	int audio_file = open(file_path, O_RDONLY);
	tau = spacing;
	cong_mode = mode;

	//TODO SIGPOLL to RECEIVE AND CONTROL TAU

	char packet[payload_size + 4];
	int num_read = read(audio_file, packet+4, payload_size);
	while(num_read!=-1){
		send_buffer(addr, port, packet, num_read+4);
		num_read = read(audio_file, packet+4, payload_size);
		//TODO LOG TAU
		usleep(tau);
	}
}
