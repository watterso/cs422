#include <arpa/inet.h>
#include <netinet/in.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#define DEFAULT_PORT 54269
#define CLIENT_LISTEN 54268
#define FRAME_SIZE 3 				//# of packets per frame 
#define PACKET_IND_LIMIT 128
#define PACKET_SIZE 512		//# of bytes per packet
#define PAYLOAD_SIZE (PACKET_SIZE*FRAME_SIZE)
#define WINDOW_SIZE (2*FRAME_SIZE)+1 		//# of packets per window

typedef struct
{
	char index;
	char payload[PACKET_SIZE];
} Packet;

char req_filename[PACKET_SIZE];
int req_fd;
int global_port;
int packet_index;
FILE * myfile;
struct sockaddr_in remote_conn;

struct timeval before;
struct timeval after;

//circular buffer resources
#define CIRC_MAX_SIZE (PACKET_SIZE*WINDOW_SIZE)
int circ_index;
int circ_size;
char circ_window[CIRC_MAX_SIZE];
int circ_peek(char* target, int n, int offset);
void circ_step(int n);
int circ_space();
int circ_write(int fd, int n);

void mylisten(int port_number, int (*loop_condition)(),
							void (*handle_packet)(int, char*, struct sockaddr_in* local,
							struct sockaddr_in* remote));
void send_buffer(char* ip, int port, char* payload, int size);
void send_buffer_sock(struct sockaddr_in targ, char* payload, int size);
void zombie_handler(int sig);
void packetloss_handler(int sig);
