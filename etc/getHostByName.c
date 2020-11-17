#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#define MYPORT 50000

int main(){
    struct hostent *phost;
    struct in_addr **addr_list;
    if ((phost = gethostbyname("www.handong.edu")) == NULL) {
        herror("gethostbyname");
        return 1;
    }
    // print information about this host:
    printf("Official name is: %s\n", phost->h_name);
    printf(" IP addresses: ");
    addr_list = (struct in_addr **) phost->h_addr_list;
    for(int i = 0; addr_list[i] != NULL; i++) {
        printf("%s ", inet_ntoa(*addr_list[i]));
    }
    printf("\n");
}


/*getHostByAddress*/
/*
* struct hostent *phost;
* struct in_addr addr;
* inet_aton(“203.252.97.12", &addr);
* phost = gethostbyaddr(&addr, sizeof(addr), AF_INET);
* printf("Host name: %s\n", phost->h_name);
*/