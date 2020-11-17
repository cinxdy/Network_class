#define BUFSIZE 30
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

void error_handling(char *message);

int main(int argc, char **argv){
    int sock;
    char filename[BUFSIZE], filebuf[BUFSIZE];
    int file_len, addr_size, i;
    struct sockaddr_in serv_addr;
    struct sockaddr_in from_addr;

    if(argc!=4){
        printf("Usage : %s <IP> <port> <file name>\n", argv[0]);
        exit(1);
    }
    sock=socket(PF_INET, SOCK_STREAM, 0);
    if(sock == -1)
        error_handling("TCP 소켓생성오류");

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family=AF_INET;
    serv_addr.sin_addr.s_addr=inet_addr(argv[1]);
    serv_addr.sin_port=htons(atoi(argv[2]));
    strcpy(filename, argv[3]);
    printf("filename: %s\n",filename);
    filename[strlen(argv[3])]=0;
    connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

    write(sock, filename, strlen(filename));
    FILE* file = fopen(filename, "rb");
    
    while(1){
        if(feof(file)) break;
        file_len = fread(filebuf,sizeof(char),BUFSIZE, file);
        printf("보내는 데이터:%s\n",filebuf);
        write(sock, filebuf, file_len);
        printf("보내기 성공\n");
    }
    
    fclose(file);
    close(sock);
    return 0;
}

void error_handling(char *message){
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}
