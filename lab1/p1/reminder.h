#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>

#define MAX_REMINDERS 100
#define MAX_REMINDER_LEN 140 //length of reminder string

char reminders[MAX_REMINDERS][MAX_REMINDER_LEN];
int alarm_times[MAX_REMINDERS];
int number_of_reminders;
int next_reminder;

void reminder_handler(int sig);
void read_reminders(char *filename);
void register_reminder(int reminder_index, char *text);
