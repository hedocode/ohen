all: server client progBar

client: client.c display.c
	gcc client.c display.c color.c -o client -pthread
	
server:
	gcc server.c player.c -o server -pthread
	
progBar: 
	gcc progBar.c display.c color.c -o progBar

clean:
	rm -rf server client progBar