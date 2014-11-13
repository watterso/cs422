files = circle_buffer.c listen.c send_buffer.c zombie_handler.c

all: client server

client:
	gcc client.c $(files) -o myroadrunner 
server:
	gcc server.c $(files) -o myroadrunnerd

clean:
	rm -rf myroadrunner
	rm -rf myroadrunnerd
