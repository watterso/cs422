#include "reminder.h"

int myloop_condition();
void mypacket_handler(int size, char* payload, struct sockaddr_in* local,
		struct sockaddr_in* remote);

int main(int argc, char **argv){
	if(argc < 2){
		printf("Syntax Error\n");
		printf("usage:\n\tmobilereminder ip-address port secret\n");
		exit(-1);
	}

	global_port = DEFAULT_PORT;
	sscanf(argv[2], "%d", &global_port);

	secret_len = (int) strlen(argv[3]);
	strncpy(secret, argv[3], PAYLOAD_SIZE);

	send_string(argv[1], global_port, secret, secret_len);
	mylisten(global_port, &myloop_condition, &mypacket_handler);
}

int myloop_condition(){
	return 1; 
}

void mypacket_handler(int size, char* payload, struct sockaddr_in* local,
		struct sockaddr_in* remote){
	struct timeval now;
	gettimeofday(&now, NULL);
	char buffer[9];
	strftime(buffer, 9, "%H:%M:%S", localtime(&now.tv_sec));
	const char separator[] = " - ";

	write(STDOUT_FILENO, buffer, 9);
	write(STDOUT_FILENO, separator, 3);
	write(STDOUT_FILENO, payload, size);
	write(STDOUT_FILENO, "\n", 1);
}
