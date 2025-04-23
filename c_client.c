#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 3000
#define HOST_IP "enter host ip"

void send_file(FILE *fp, int sockfd){
    char data[1024] = {0};

    while(fgets(data, 1024, fp) != NULL){
        if(send(sockfd, data, sizeof(data), 0)==-1){
            perror("Error sending data.\n");
            exit(1);
        }
        bzero(data, 1024);
    }
}

int main(){
    int clientSocket;
    struct sockaddr_in serverAddr;
    char buffer[1024];
    FILE *fp;
    char filename[64];

    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if(clientSocket < 0){
        printf("Error in connection.\n");
        exit(1);
    }
    printf("Client socket is created.\n");

    memset(&serverAddr, '\0', sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = inet_addr(HOST_IP);

    int ret = connect(clientSocket, (struct sockaddr *)(&serverAddr), sizeof(serverAddr));
    if(ret<0){
        printf("Could not connect to server.\n");
        exit(1);
    }
    printf("Connected to server.\n");

    while(1){
        scanf("%s", &filename[0]);
        // send(clientSocket, buffer, strlen(buffer), 0);
        

        if(strcmp(filename, ":exit")== 0){   
            printf("Disconnected from server.\n");
            exit(1);
        }

        fp = fopen(filename, "r");
        if(fp==NULL){
            perror("Error reading file.\n");
            exit(1);
        }

        send_file(fp, clientSocket);
        printf("File data sent successfully.\n");
        close(clientSocket);
        printf("Disconnected from server.\n");
        bzero(filename, sizeof(filename));
        exit(1);

        // if(recv(clientSocket, buffer, 1024, 0)<0){
        //     printf("Error receiving data.\n");
        // }else{
        //     printf("Server: \t%s\n", buffer);
        // }
    }

    return 0;
}