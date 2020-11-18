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
    // Variable for socket
    int sock;
    int addr_size;
    struct sockaddr_in serv_addr;
    struct sockaddr_in from_addr;

    // Variable for file
    FILE* file;
    char filename[BUFSIZE], filebuf[BUFSIZE];
    int filebuf_len;

    if(argc!=4){
        printf("Usage : %s <IP> <port> <file name>\n", argv[0]);
        exit(1);
    }

    // Make a socket
    sock=socket(PF_INET, SOCK_STREAM, 0);
    if(sock == -1)
        error_handling("TCP 소켓생성오류");

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family=AF_INET;
    serv_addr.sin_addr.s_addr=inet_addr(argv[1]);
    serv_addr.sin_port=htons(atoi(argv[2]));
    strcpy(filename, argv[3]);
    printf("filename: %s\n",filename);

    // Connect to socket
    connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

    // Send the file name
    write(sock, filename, strlen(filename));
    
    // Send the file data
    file = fopen(filename, "rb");
    while(1){
        if(feof(file)) break;
        filebuf_len = fread(filebuf,sizeof(char),BUFSIZE, file);
        printf("보내는 데이터:%s\n",filebuf);
        write(sock, filebuf, filebuf_len);
        printf("보내기 성공\n");
    }

    printf("전송 완료\n");
    printf("filename:%s\n", filename);
    fclose(file);
    close(sock);
    
    printf("===WC result===\n");
    char wc[BUFSIZE+3]="wc ";
    system(strcat(wc,filename));
    return 0;
}

void error_handling(char *message){
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}
