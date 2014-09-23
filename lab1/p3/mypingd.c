#include "myping.h"

int main(int argc, char** argv){
	if(argc < 2){
		printf("Syntax Error\n");
		printf("usage:\n\tmypingd portnumber\n");
		exit(-1);
	}
	int port_number;
	sscanf(argv[1], "%d", &port_number);
	if(port_number < 49152 || port_number > 65535){
		printf("Port out of range, using default port\n");
		port_number = DEFAULT_PORT;
	}
	dumb_respond(port_number);	
}
