#include "reminder.h"

int main(int argc, char **argv){
	if(argc < 2){
		printf("Syntax Error\n");
		printf("usage:\n\tmyreminder filename\n");
		exit(-1);
	}
	// initialize globals
	number_of_reminders = 0;
	next_reminder = 0;

	struct sigaction sigact;
	sigact.sa_handler = &reminder_handler;
	sigact.sa_flags = SA_RESTART;
	sigfillset(&sigact.sa_mask);
	if(sigaction(SIGALRM, &sigact, NULL) == -1){
		printf("Error setting signal handler\n");
		exit(-1);
	}

	read_reminders(argv[1]);
	alarm(alarm_times[0]);
	while(next_reminder < number_of_reminders){
	}
}
