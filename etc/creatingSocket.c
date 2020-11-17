#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>

int main(){
    int sockfd;
    if ((sockfd = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
        /* print “socket error + the error message */
        perror("socket error"); 
        exit(1);
    }
}