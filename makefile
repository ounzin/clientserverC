all : client server
client: 
	gcc -o client client.c
server1: 
	gcc -o server server.c