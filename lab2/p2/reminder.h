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

#define MAX_REMINDERS 100
#define MAX_REMINDER_LEN 140 //length of reminder string
#define PAYLOAD_SIZE 1024
#define DEFAULT_PORT 54269
#define ACK 0x01
#define EMPTY_FLAG 0x00
#define ACK_WAIT 3

char reminders[MAX_REMINDERS][MAX_REMINDER_LEN];
int alarm_times[MAX_REMINDERS];
int number_of_reminders;
int next_reminder;
struct sockaddr_in client;
char secret[PAYLOAD_SIZE];
int secret_len;
int global_port;
char remote_ip[PAYLOAD_SIZE];

void ack_handler(int sig);
void mylisten(int port_number, int (*loop_condition)(),
							void (*handle_packet)(int, char*, struct sockaddr_in* local,
							struct sockaddr_in* remote));
int nisalnum(int size, char* string);
void reminder_handler(int sig);
void read_reminders(char *filename);
void register_reminder(int reminder_index, char *text);
void register_client(struct sockaddr_in* address);
int secret_matches(int str_size, char* secret);
void send_string(char* ip, int port, char* secret, int size, char flags);
