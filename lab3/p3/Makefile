server_files = server.c listen.c register_client.c send_string.c
client_files = client.c listen.c send_string.c ack_handler.c

all:
	gcc $(server_files) -o vpntunneld
	gcc $(client_files) -o vpntunnel

clean:
	rm -rf vpntunneld
	rm -rf vpntunnel
