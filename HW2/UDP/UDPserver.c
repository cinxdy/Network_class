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
    // Variable for socket
    int serv_sock;
    int sin_size;
    int new_fd;

    struct sockaddr_in serv_addr;
    struct sockaddr_in clnt_addr;
    int clnt_addr_size;

    // Variable for file
    FILE* file;
    char filename[BUFSIZE], filebuf[BUFSIZE];
    int filename_len, filebuf_len;

    if(argc!=2){
            printf("Usage : %s <port>\n", argv[0]);
            exit(1);
    }

    // Make a socket
    serv_sock=socket(PF_INET, SOCK_DGRAM, 0);
    if(serv_sock == -1)
        error_handling("UDP 소켓 생성 오류");
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family=AF_INET;
    serv_addr.sin_addr.s_addr=htonl(INADDR_ANY);
    serv_addr.sin_port=htons(atoi(argv[1]));

    // Bind
    if(bind(serv_sock, (struct sockaddr*) &serv_addr, sizeof(serv_addr))==-1)
        error_handling("bind() error");

    // Receive the file name
    long file_size;
    int totalBufferNum;
    int BufferNum;

    // Store the chunked data to the file
    filename_len = recvfrom(serv_sock, filename, BUFSIZE, MSG_PEEK, (struct sockaddr*) &clnt_addr, &clnt_addr_size);
    recvfrom(serv_sock, filename, filename_len+1, 0, (struct sockaddr*) &clnt_addr, &clnt_addr_size);
    filename[filename_len]=0;
    printf("filename: %s\n",filename);

    // Save the file
    file = fopen(filename, "wb");

    // Store the chunked data to the file
    while(1){
        clnt_addr_size = sizeof(clnt_addr);
        filebuf_len = recvfrom(serv_sock, filebuf, BUFSIZE, 0,(struct sockaddr*)&clnt_addr, &clnt_addr_size);
        filebuf[filebuf_len]=0;
        printf("받은 데이터:%s\n",filebuf);
        fwrite(filebuf, sizeof(char), filebuf_len, file);
        printf("받기 성공\n");
        //if(filebuf_len==0) break;
    }
    fclose(file);
    close(serv_sock);
    
    printf("===WC result===\n");
    char wc[BUFSIZE+3]="wc ";
    system(strcat(wc,filename));
    return 0;
}

void error_handling(char *message)
{
        fputs(message, stderr);
        fputc('\n', stderr);
        exit(1);
}