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

#define PAYLOAD_SIZE 1024
#define DEFAULT_PORT 54269
#define DEFAULT_VPN_PORT 54299
#define DEFAULT_PIPE_PORT 54300
#define ACK 0x01
#define EMPTY_FLAG 0x00
#define ACK_WAIT 3

int pipe_pid;

struct sockaddr_in client;
struct sockaddr_in target;
char vpn_ip[PAYLOAD_SIZE];
int vpn_port;
int pipe_port;
int pipe_counter;		//offset to be added to default pipe_port
char remote_ip[PAYLOAD_SIZE];
int remote_port;

void ack_handler(int sig);
void mylisten(int port_number, int (*loop_condition)(),
							void (*handle_packet)(int, char*, struct sockaddr_in* local,
							struct sockaddr_in* remote));
void register_client(struct sockaddr_in* address);
void send_raw(char* ip, int port, char* buffer, int size);
void send_string(char* ip, int port, char* string, int size, char flags);
