#include "myroadrunner.h"

void zombie_handler(int sig){
	while (waitpid((pid_t)(-1), 0, WNOHANG) > 0) {}
}
