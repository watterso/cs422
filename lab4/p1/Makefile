files = zombie_handler.c copy_file.c

all: client server

client:
	gcc client.c $(files) -o fsclient 
server:
	gcc server.c $(files) -o fsserver

clean:
	rm -rf fsclient 
	rm -rf fsserver
