all: server client progBar

client: client.c display.c
	gcc client.c core.c display.c color.c -o client -pthread -Wall
	
server:
	gcc server.c core.c player.c -o server -pthread -Wall
	
progBar: 
	gcc progBar.c display.c color.c -o progBar -Wall

clean:
	rm -rf server client progBar