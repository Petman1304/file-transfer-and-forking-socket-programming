#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>

#define PORT 3000


void write_file(int sockfd){
    int n;
    FILE *fp;
    char filename[24];
    char buffer[1024];
    bzero(buffer, 1024);

    while (1)
    {
        n = recv(sockfd, buffer, 1024, 0);
        if(n<=0){
            break;
            return;
        }
        time_t t = time(NULL);
        struct tm tm = *localtime(&t);
        strftime(filename, sizeof(filename),"%Y%m%d_%H%M%S.txt" ,&tm);

        fp = fopen(filename, "w");
        if (fp == NULL){
            perror("Error creating file.\n");
            exit(1);
        }
        fprintf(fp,"%s", buffer);
        printf("File received successfully. Saved as : %s\n", filename);
        fclose(fp);

    }
    return;
}

int main(){

    int sockfd, ret;
    struct sockaddr_in serverAddr;

    int newSocket;
    struct sockaddr_in newAddr;

    socklen_t addr_size;

    char buffer[1024];
    pid_t childpid;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0){
        printf("Error in connection.\n");
        exit(1);
    }
    printf("Server socket is created.\n");

    memset(&serverAddr, '\0', sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = inet_addr("0.0.0.0");

    ret = bind(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
    if(ret<0){
        printf("Error in binding.\n");
        exit(1);
    }
    printf("Bind to port %d\n", PORT);

    if(listen(sockfd, 10)==0){
        printf("Listening...\n");
    }else{
        printf("Error in binding\n");
    }

    while(1){
        newSocket = accept(sockfd, (struct sockaddr*)&newAddr, &addr_size);
        if(newSocket < 0){
            exit(1);
        }
        printf("Connection accepted from %s:%d\n", inet_ntoa(newAddr.sin_addr), ntohs(newAddr.sin_port));
    
        if((childpid = fork())==0){
            close(sockfd);

            write_file(newSocket);
        }
    }

    close(newSocket);

    return 0;
}