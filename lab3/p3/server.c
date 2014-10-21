#include "vpn.h"

int myloop_condition();
void mypacket_handler(int size, char* payload, struct sockaddr_in* local,
		struct sockaddr_in* remote);
void pipe_packet_handler(int size, char* payload, struct sockaddr_in* local,
		struct sockaddr_in* remote);

int main(int argc, char **argv){
	if(argc < 2){
		printf("Syntax Error\n");
		printf("usage:\n\tvpntunneld port\n");
		exit(-1);
	}

	vpn_port = DEFAULT_PORT;
	sscanf(argv[1], "%d", &vpn_port);

	pipe_pid = -1;
	mylisten(vpn_port, &myloop_condition, &mypacket_handler);
}

int myloop_condition(){
	return 1;
}

void mypacket_handler(int size, char* payload, struct sockaddr_in* local,
		struct sockaddr_in* remote){
	//printf("Local port: %d remote port: %d\n", ntohs(local->sin_port), ntohs(remote->sin_port));
	int msg_size = size-1;
	char* msg = payload+1;
	if(pipe_pid != -1){
		printf("KILL CHILD\n");
		//if there already is a pipe child process running, kill it
		if(kill(pipe_pid, 9) < 0){
			printf("Failed to kill child process, continuing anyway.\n");
		}
		pipe_pid = -1;
	}
	//if(client.sin_port != 0) return;
	register_client(remote);
	printf("Client trying to register with '%s'\n", msg);
	sscanf(msg, "%[^:]:%d", remote_ip, &remote_port); 
	printf("Desired server - %s:%d\n", remote_ip, remote_port);
	memset((char*)&target, 0, sizeof(target));
	target.sin_family = AF_INET;
	target.sin_addr.s_addr = inet_addr(remote_ip);
	target.sin_port = htons(remote_port);

	pipe_port = DEFAULT_PIPE_PORT + pipe_counter;
	pipe_counter++;
	char port_str[6];
	sprintf(port_str, "%d", pipe_port);
	port_str[5] = '|';
	send_string(inet_ntoa(client.sin_addr), vpn_port, port_str, 6, ACK);

	int my_pid = fork();
	if(my_pid == 0){
		//Should loop until parent process ends or it is killed
		mylisten(pipe_port, &myloop_condition, &pipe_packet_handler);
	}
	pipe_pid = my_pid;
}
void pipe_packet_handler(int size, char* payload, struct sockaddr_in* local,
		struct sockaddr_in* remote){
	printf("Packet from %s: %s\n", inet_ntoa(remote->sin_addr), payload);
	printf("remote(%ld) ? target(%ld) or client(%ld)\n", remote->sin_addr.s_addr, target.sin_addr.s_addr, client.sin_addr.s_addr);
	if(remote->sin_addr.s_addr == client.sin_addr.s_addr){
		//Send to target server
		send_raw(inet_ntoa(target.sin_addr), remote_port, payload, size);
		//printf("sending to %d at %s\n", target.sin_port, inet_ntoa(target.sin_addr));
	}
	if(remote->sin_addr.s_addr == target.sin_addr.s_addr){
		//Send to client
		send_raw(inet_ntoa(client.sin_addr), pipe_port, payload, size);
	}
}
