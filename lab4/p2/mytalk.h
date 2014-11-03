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
#include <termios.h>
#include <time.h>
#include <unistd.h>

#define PAYLOAD_SIZE 1024
#define DEFAULT_PORT 54269
#define ACK 0x01
#define EMPTY_FLAG 0x00
#define ACK_WAIT 3

struct sockaddr_in client;
int peer_connected;
int global_port;
int loop;
char remote_ip[PAYLOAD_SIZE];
struct timeval before;
struct timeval after;

void mylisten(int port_number, int (*loop_condition)(),
							void (*handle_packet)(int, char*, struct sockaddr_in* local,
							struct sockaddr_in* remote));
void send_buffer(char* ip, int port, char* payload, int size, char flags);
void zombie_handler(int sig);
void ack_handler(int sig);
void alert_user(char * alert_message);
void user_interact();
