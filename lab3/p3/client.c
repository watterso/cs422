#include "vpn.h"

int myloop_condition();
void mypacket_handler(int size, char* payload, struct sockaddr_in* local,
	struct sockaddr_in* remote);

int main(int argc, char **argv){
    if(argc < 5){
	printf("Syntax Error\n");
	printf("usage:\n\tvpntunnel vpn-ip vpn-port ip-address port\n");
	exit(-1);
    }

    strcpy(vpn_ip, argv[1]);
    vpn_port = DEFAULT_VPN_PORT;
    sscanf(argv[2], "%d", &vpn_port);

    strcpy(remote_ip, argv[1]);
    remote_port = DEFAULT_PORT;
    sscanf(argv[2], "%d", &remote_port);

    struct sigaction sigact;
    sigact.sa_handler = &ack_handler;
    sigact.sa_flags = SA_RESTART;
    sigfillset(&sigact.sa_mask);
    if(sigaction(SIGALRM, &sigact, NULL) == -1){
	printf("Error setting signal handler\n");
	exit(-1);
    }

    char vpn_config[PAYLOAD_SIZE];
    int tmp_len = strlen(remote_ip);
    tmp_len += 5 + 1 + 1; 		//5: port chars, 1: colon, 1: padding
    snprintf(vpn_config, tmp_len, "%s:%d", remote_ip, remote_port);
    printf("\t%s\n", vpn_config);
    send_string(vpn_ip, vpn_port, vpn_config, tmp_len, EMPTY_FLAG);
    alarm(ACK_WAIT);	
    mylisten(vpn_port, &myloop_condition, &mypacket_handler);
}

int myloop_condition(){
    return 1; 
}

void mypacket_handler(int size, char* payload, struct sockaddr_in* local,
	struct sockaddr_in* remote){
    char flags = *payload;
    payload++;
    if(flags & ACK){
	alarm(0);
	char rec_port[6];
	strncpy(rec_port, payload, 5);
	rec_port[5] = '|';
	sscanf(payload,"%d|", &pipe_port);
	printf("VPN configured, send packets to %s:%d\n", vpn_ip, pipe_port);
	//printf("ACK RECEIVED\n");
    }else{
	printf("Server says: %s\n", payload);
    }
}
