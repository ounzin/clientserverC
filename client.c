#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <ctype.h>

#include "utils.c"

void handleMessageOnClient(char *buffer, int sockfd)
{
    printf("Please enter the message: ");
    bzero(buffer, 1024);
    fgets(buffer, 1024, stdin);
    int n = write(sockfd, buffer, strlen(buffer));
    bzero(buffer, 1024);
    n = read(sockfd, buffer, 1024);
    printf("%s\n", buffer);
}

void handleFactorialOnClient(char *buffer, int sockfd)
{
    printf("Please enter the number: ");
    bzero(buffer, 1024);
    fgets(buffer, 1024, stdin);
    int n = write(sockfd, buffer, strlen(buffer));
    bzero(buffer, 1024);
    n = read(sockfd, buffer, 1024);
    printf("The factorial of the number is : %s\n", buffer);
}

char *getFileNameAndExtensionFromPath(char *p)
{
    char *s = p;
    char *t = p;
    while (*s)
    {
        if (*s == '/')
        {
            t = s + 1;
        }
        s++;
    }
    return t;
}

void handleImageTransfertOnClient(char *buffer, int sockfd)
{
    char *path = malloc(1024);
    printf("Give the path of the image: ");
    bzero(buffer, 1024);
    path = removeWhiteSpaces(fgets(buffer, 1024, stdin));

    FILE *fp;
    fp = fopen(path, "rb");
    if (fp == NULL)
    {
        perror("fopen");
        exit(1);
    }
    char *fileName = getFileNameAndExtensionFromPath(path);
    write(sockfd, fileName, strlen(fileName));
    int bytesRead = 0;
    printf("Sending image to server...\n");
    bzero(buffer, 1024);
    while ((bytesRead = fread(buffer, 1, 1024, fp)) > 0)
    {
        write(sockfd, buffer, bytesRead);
    }
    printf("Image sent to server.\n");
    fclose(fp);
}

void displayMenuAndChooseOperation(int newsockfd, char *buffer, int size)
{
    // printf menu choices
    printf("1. Send a message to the server\n");
    printf("2. Get factorial of a number from the server\n");
    printf("3. Send an image to the server\n");
    printf("4. Exit\n");

    int choice;

    printf("Please enter your choice: ");
    bzero(buffer, 1024);
    fgets(buffer, 1024, stdin);
    write(newsockfd, buffer, sizeof(buffer));
    choice = atoi(removeWhiteSpaces(buffer));
    printf("Your choice is: %d ", choice);
    switch (choice)
    {
    case 1:
        handleMessageOnClient(buffer, newsockfd);
        exit(0);
    case 2:

        handleFactorialOnClient(buffer, newsockfd);
        exit(0);
    case 3:

        handleImageTransfertOnClient(buffer, newsockfd);
        exit(0);
    case 4:
        printf("Exiting...\n");
        exit(0);
        break;
    }
}
int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        printf("Usage: ./client <server-ip-address> <port>\n");
        exit(1);
    }
    else
    {
        printf("A new client started...\n");
        printf("Connecting to server...\n");
        // get ip address and port from command line
        char *ip = argv[1];
        char *port = argv[2];

        // start the client
        int sockfd;
        int portno = atoi(port);
        struct sockaddr_in serv_addr;
        char buffer[1024];

        sockfd = socket(AF_INET, SOCK_STREAM, 0);

        if (sockfd < 0)
        {
            perror("socket");
            exit(1);
        }
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_addr.s_addr = inet_addr(ip);
        serv_addr.sin_port = htons(portno);
        int state = connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
        if (state < 0)
        {
            perror("connect");
            exit(1);
        }
        else
        {
            printf("Connected to server %s:%d\n", ip, portno);

            displayMenuAndChooseOperation(sockfd, buffer, 1024);
        }
        close(sockfd);
        return 0;
    }
}