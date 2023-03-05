#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define MAX_COMMAND_SIZE 1024

int main(int argc, char const *argv[])
{
    int sock =0;
    struct sockaddr_in serv_addr;
    char command[MAX_COMMAND_SIZE] = {0};
    char response[MAX_COMMAND_SIZE] = {0};

    if(argc < 3)
    { 
        printf("Usage:%s <ip_address> <port>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    if((sock = socket(AF_INET, SOCK_STREAM, 0))<0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(atoi(argv[2]));
  
    //CONVERT IP address from string to binary
    if(inet_pton(AF_INET, argv[1], &serv_addr.sin_addr)<=0)
    {
        printf("invalid IP address\n");
        exit(EXIT_FAILURE);
    }
  
    if(connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr))<0)
    {
        perror("connect failed");
        exit(EXIT_FAILURE);
    }
  
    printf("Connected to server: %s:%s\n", argv[1], argv[2]);
    while (1)
    { 
        printf("> ");
        fgets(command, MAX_COMMAND_SIZE, stdin);
        command[strcspn(command, "\n")]=0;
        if(strcmp(command, "disconnect") == 0)
        {
            send(sock, command, strlen(command), 0);
            printf("Disconnected from server.\n");
            break;
        }
        else if(strncmp(command, "shell ", 6) == 0)
        {
            send(sock, command, strlen(command), 0);
            printf("%s\n", response);
        }
        else
        {
            printf("Invalid command");
        }
    
        memset(command, 0, sizeof(command));
        memset(response, 0, sizeof(response));
    }
  
    close(sock);
    return 0;
}

