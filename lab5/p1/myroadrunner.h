#include <arpa/inet.h>
#include <netinet/in.h>
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
#define FRAME_SIZE 24				//# of packets per frame 
#define MAX_PACKET_IND
#define PACKET_SIZE 1024		//# of bytes per packet
#define PAYLOAD_SIZE PACKET_SIZE*FRAME_SIZE
#define WINDOW_SIZE 50			//# of packets per window

typedef struct
{
	char index;
	char payload[PACKET_SIZE];
} Packet;

char req_filename[PACKET_SIZE];
int req_fd;
int global_port;
int packet_index;

struct timeval before;
struct timeval after;

//circular buffer resources
int circ_index;
int circ_size;
int circ_max_size = PACKET_SIZE*WINDOW_SIZE;
char circ_window[circ_size];
int circ_read(char* target, int n);
void circ_step(int n);
int circ_space();
int circ_write(int fd, int n);

void mylisten(int port_number, int (*loop_condition)(),
							void (*handle_packet)(int, char*, struct sockaddr_in* local,
							struct sockaddr_in* remote));
void send_buffer(char* ip, int port, char* payload, int size);
void zombie_handler(int sig);
