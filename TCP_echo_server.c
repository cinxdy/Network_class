#define BUFSIZE 5
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

void error_handling(char *message);

int main(int argc, char **argv){
    int serv_sock;
    char message[BUFSIZE];
    int str_len, num=0;
    struct sockaddr_in serv_addr;
    struct sockaddr_in clnt_addr;
    int clnt_addr_size;

    if(argc!=2){
        printf("Usage : %s <port>\n", argv[0]);
        exit(1);
    }

    serv_sock=socket(PF_INET, SOCK_STREAM, 0);
    if(serv_sock == -1)
        error_handling("TCP 소켓 생성 오류");

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family=AF_INET;
    serv_addr.sin_addr.s_addr=htonl(INADDR_ANY);
    serv_addr.sin_port=htons(atoi(argv[1]));
    if(bind(serv_sock, (struct sockaddr*) &serv_addr, sizeof(serv_addr))==-1)
        error_handling("bind() error");
    sleep(5);

    listen(serv_sock,5);
    int sin_size=sizeof(clnt_addr);
    int new_fd = accept(serv_sock, (struct sockaddr *)&clnt_addr, &sin_size);
    
    while( (str_len=read(new_fd,message, BUFSIZE)) != 0){
        sleep(1);
        printf("수신 번호 : %d \n", num++);
        printf("%s\n",message);
        write(new_fd, message, str_len);
        //send(new_fd, message, str_len, 0);
    }
    return 0;
}

void error_handling(char *message){
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}