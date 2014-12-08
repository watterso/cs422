files = listen.c send_buffer.c zombie_handler.c

all: client server

client:
	gcc client.c $(files) -o myaudiostream

server:
	gcc server.c udp_audio.c $(files) -o myaudiostreamd

clean:
	rm -f myaudiostream
	rm -f myaudiostreamd
