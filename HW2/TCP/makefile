all: TCPclient TCPserver

TCPclient: TCPclient.c
	gcc -o TCPclient.out TCPclient.c 
TCPserver: TCPserver.c
	gcc -o TCPserver.out TCPserver.c 

clean:
	rm -f TCPclient.out TCPserver.out