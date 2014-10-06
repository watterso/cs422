#include "reminder.h"

void register_client(struct sockaddr_in* address){
	printf("REGISTERED CLIENT\n");
	memcpy(&client, address, sizeof(struct sockaddr_in));	
}
