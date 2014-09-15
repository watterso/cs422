#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>

#define MAX_REMINDERS 100
#define MAX_REMINDER_LEN 140 //length of reminder string

//reminder array
char reminders[MAX_REMINDERS][MAX_REMINDER_LEN];

void read_reminders(char *filename);

int main(int argc, char **argv){
	if(argc < 2){
		printf("Syntax Error\n");
		printf("usage:\n\tmyreminder filename\n");
		exit(-1);
	}
	read_reminders(argv[1]);
}

void read_reminders(char *filename){

}
