#include "reminder.h"

void reminder_handler(int sig){
	struct timeval now;
	gettimeofday(&now, NULL);
	char buffer[9];
	strftime(buffer, 9, "%H:%M:%S", localtime(&now.tv_sec));
	const char separator[] = " - ";
	//Write timestamp
	write(STDOUT_FILENO, buffer, 9);
	write(STDOUT_FILENO, separator, 3);
	write(STDOUT_FILENO, reminders[next_reminder], MAX_REMINDER_LEN);
	write(STDOUT_FILENO, "\n", 1);
	//printf("%s - %s\n", buffer, reminders[next_reminder]);
	alarm(alarm_times[next_reminder+1]-alarm_times[next_reminder]);
	next_reminder++;
}
