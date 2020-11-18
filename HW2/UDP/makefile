all: UDPclient UDPserver

UDPclient: UDPclient.c
	gcc -o UDPclient.out UDPclient.c 
UDPserver: UDPserver.c
	gcc -o UDPserver.out UDPserver.c 

clean:
	rm -f UDPclient.out UDPserver.out
