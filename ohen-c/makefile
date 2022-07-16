all: server client

client: code/client/client.c code/client/display.c
	gcc code/client/client.c code/lib/core.c code/client/display.c code/client/color.c -o client -pthread -Wall
	
server:
	gcc code/server/server.c code/lib/core.c code/lib/player.c -o server -pthread -Wall
	
clean:
	rm -rf server client