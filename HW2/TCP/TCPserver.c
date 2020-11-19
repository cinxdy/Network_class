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
    serv_sock=socket(PF_INET, SOCK_STREAM, 0);
    if(serv_sock == -1)
        error_handling("TCP 소켓 생성 오류");
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family=AF_INET;
    serv_addr.sin_addr.s_addr=htonl(INADDR_ANY);
    serv_addr.sin_port=htons(atoi(argv[1]));

    // Bind
    if(bind(serv_sock, (struct sockaddr*) &serv_addr, sizeof(serv_addr))==-1)
        error_handling("bind() error");

    // Listen and Accept (Three hand shaking)
    listen(serv_sock,5);
    clnt_addr_size=sizeof(clnt_addr);
    new_fd = accept(serv_sock,(struct sockaddr*)&clnt_addr, &clnt_addr_size);

    // Receive the file name
    filename_len = recv(new_fd, filename, BUFSIZE, 0);
    filename[filename_len]=0;
    printf("filename: %s\n",filename);

    // Save the file
    file = fopen(filename, "wb");
    close(new_fd);

    //Store the chunked data to the file
    while(1){
        filebuf_len = recv(new_fd, filebuf, BUFSIZE, 0);
        if(filebuf_len==0) break;
        filebuf[filebuf_len]=0;
        printf("받은 데이터:%s\n",filebuf);
        fwrite(filebuf, sizeof(char), filebuf_len, file);
        printf("받기 성공\n");
    }
    printf("전송 완료\n");
    printf("filename:%s\n", filename);
    fclose(file);
    close(new_fd);
    
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
