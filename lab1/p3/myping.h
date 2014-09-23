#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>


#define PAYLOAD_SIZE 1024
#define DEFAULT_PORT 54269
#define DUMB_MSG "MY_RESPONSE"

void dumb_respond(int port_number);
