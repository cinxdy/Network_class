all: UDPclient UDPserver

TCPclient: UDPclient.c
	gcc -o UDPclient.out UDPclient.c 
TCPserver: UDPserver.c
	gcc -o UDPserver.out UDPserver.c 

clean:
	rm -f UDPclient.out UDPserver.out