#include<sys/socket.h>
#include<sys/types.h>
#include<stdio.h>
#include<string.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<stdlib.h>

#define BUFSIZE 1000
void error_handling(char *message);

int main(int argc, char **argv){
    int serv_sock;
    int sin_size;
    int new_fd;

    struct sockaddr_in serv_addr;
    struct sockaddr_in clnt_addr;
    int clnt_addr_size;

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
    printf("bind success\n");

    while(1){
        // Listen and Accept (Three hand shaking)
        if(listen(serv_sock,5)<0){
            error_handling("listen() error");
        }
        clnt_addr_size=sizeof(clnt_addr);
        new_fd = accept(serv_sock,(struct sockaddr*)&clnt_addr, &clnt_addr_size);
        printf("accept success\n");

        //routing
        char buf[BUFSIZE], path[BUFSIZE];
        int buf_len = recv(new_fd, buf, BUFSIZE, 0);
        buf[buf_len]=0;
        printf("request: %s\n",buf);


        enum Method {GET, POST};
        int method,i,j;
        for(i=0;i<buf_len-10;i++){
            if(buf[i]=='G' && buf[i+1]=='E' && buf[i+2]=='T'){
                method = GET;
                for(j=0;buf[i+j+4]!=' ';j++)
                    path[j]=buf[i+4+j];
                path[j]=0;
            }
            else if(buf[i]=='P' && buf[i+1]=='O' && buf[i+2]=='S'&&buf[i+3]=='T'){
                method = POST;
                for(j=0;buf[i+j+5]!=' ';j++)
                    path[j]=buf[i+5+j];
                path[j]=0;
            }
        }
        printf("method: %d\n path: %s\n",method,path);

        FILE* fp;
        int htmlBuf_len;
        char htmlBuf[BUFSIZE];
        char header[BUFSIZE];

        if(method==GET){
            if(!strcmp(path,"/") || !strcmp(path,"/index.html")){
                strcpy(header,"HTTP/1.0 200 OK\nContent-type: text/html\n\n");
                fp = fopen("index.html","r");  
            }
            else if(!strcmp(path,"/query.html")){
                strcpy(header,"HTTP/1.0 200 OK\nContent-type: text/html\n\n");
                fp = fopen("query.html","r");  
            }
            else{
                strcpy(header,"HTTP/1.0 404 NOTFOUND\n\n");
            }

            
            send(new_fd, header, strlen(header), 0);
            while(!feof(fp)){
                htmlBuf_len = fread(htmlBuf,sizeof(char),BUFSIZE,fp);
                htmlBuf[htmlBuf_len]=0;
                printf("read...\n");
                printf("htmlBuf:%s\n",htmlBuf);
                send(new_fd,htmlBuf,htmlBuf_len,0);
            }
            fclose(fp);
        }
        else if(method==POST){
            strcpy(header,"HTTP/1.0 200 OK\nContent-type: text/html\n\n");
            send(new_fd, header, strlen(header), 0);
            //WRITE CODE HERE
            
        }

        close(new_fd);
    }
    
    return 0;
}
void error_handling(char *message){
        fputs(message, stderr);
        fputc('\n', stderr);
        exit(1);
}