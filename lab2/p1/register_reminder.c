#include "reminder.h"

void register_reminder(int reminder_index, char *text){
	int alarm_time = 0;
	sscanf(text, "%d %[^\n]s", &alarm_time, reminders[reminder_index]);
	number_of_reminders++;
	alarm_times[reminder_index] = alarm_time;
}
