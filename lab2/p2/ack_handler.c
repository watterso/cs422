#include "reminder.h"

void ack_handler(int sig){
	printf("NO ACK RECEIVED, RESENDING\n");
	send_string(remote_ip, global_port, secret, secret_len, EMPTY_FLAG);
	alarm(ACK_WAIT);
}
