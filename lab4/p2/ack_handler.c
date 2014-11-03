#include "mytalk.h"

void ack_handler(int sig){
	alert_user("No response from peer");	
}
