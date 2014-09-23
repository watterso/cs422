#include "myping.h"

int main(int argc, char** argv){
	if(argc < 3){
		printf("Syntax Error\n");
		printf("usage:\n\tmyping ip port\n");
		exit(-1);
	}
	int port_number;
	sscanf(arg[2], "%d", &port_number);
}
