#include "reminder.h"

void read_reminders(char *filename){
	char reminder[MAX_REMINDER_LEN];
	int fp = open(filename, O_RDONLY);
	if(fp == -1){
		printf("Error opening file\n");
		exit(-1);
	}
	char tmp_buffer[2];
	int i = 0;
	int reminder_index = 0;
	//************************************
	//* Read from file 1 byte at a time  *
	//************************************
	int num_read = read(fp, tmp_buffer, 1);
	while(num_read > 0){ 
		reminder[i] = tmp_buffer[0];
		if(tmp_buffer[0] == '\n'){
			reminder[i+1] = '\0';
			register_reminder(reminder_index, reminder);
			reminder_index++;
			i = 0;           
		}else{
			i++;
		}
		num_read = read(fp, tmp_buffer, 1);
	}
	if(i != 0){
		reminder[i+1] = '\0';
		register_reminder(reminder_index, reminder);
	}

	int close_result = close(fp);
	if(close_result == -1){
		printf("Error closing file\n");
	}
}
