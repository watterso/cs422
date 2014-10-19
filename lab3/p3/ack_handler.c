#include "vpn.h"

void ack_handler(int sig){
    printf("NO ACK RECEIVED, RESENDING\n");
    char vpn_config[PAYLOAD_SIZE];
    int tmp_len = strlen(remote_ip);
    tmp_len += 5 + 1 + 1; 		//5: port chars, 1: colon, 1: padding
    snprintf(vpn_config, tmp_len, "%s:%d", remote_ip, remote_port);
    printf("\t%s\n", vpn_config);
    send_string(vpn_ip, vpn_port, vpn_config, tmp_len, EMPTY_FLAG);
    alarm(ACK_WAIT);
}
