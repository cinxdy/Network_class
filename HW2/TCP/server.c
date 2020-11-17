#include<sys/socket.h>
#include<sys/types.h>
#include<stdio.h>
#include<string.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<stdlib.h>

#define BUFSIZE 30
void error_handling(char *message);

int main(int argc, char **argv){
    int serv_sock;
    char filename[BUFSIZE];
    int filename_len, num=0, file_len;
    int sin_size;
    int new_fd;

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

    listen(serv_sock,5);
    clnt_addr_size=sizeof(clnt_addr);
    new_fd = accept(serv_sock,(struct sockaddr*)&clnt_addr, &clnt_addr_size);


    filename_len = read(serv_sock, filename, BUFSIZE);
    filename[filename_len]=0;
    printf("filename: %s\n",filename);
    FILE* file = fopen(filename, "w");
    char filebuf[BUFSIZE];

    while(1){
        file_len = read(serv_sock, filebuf, BUFSIZE);
        fwrite(file, 1, sizeof(filebuf), filebuf);
    }

    return 0;
}
void error_handling(char *message)
{
        fputs(message, stderr);
        fputc('\n', stderr);
        exit(1);
}
