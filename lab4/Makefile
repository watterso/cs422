files = listen.c send_buffer.c zombie_handler.c

all: client server

client:
	gcc client.c $(files) -o myping
server:
	gcc server.c $(files) -o mypingd

clean:
	rm -rf myping
	rm -rf mypingd
