files = reminder.c read_reminders.c register_reminder.c reminder_handler.c
server_files = server.c listen.c read_reminders.c register_reminder.c register_client.c reminder_handler.c secret_matches.c nisalnum.c send_string.c
client_files = client.c listen.c send_string.c

all:
	gcc $(server_files) -o mobilereminderd
	gcc $(client_files) -o mobilereminder

clean:
	rm -rf mobilereminderd
	rm -rf mobilereminder
