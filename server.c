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

void handleMessageOnServer(int newsockfd, unsigned char *buffer, int size)
{
    int n = read(newsockfd, buffer, 1023);
    printf("Here is the message: %s\n", buffer);
    write(newsockfd, "I got your message", 18);
}

void handleFactorialOnServer(int newsockfd, unsigned char *buffer, int size)
{
    int n = read(newsockfd, buffer, 1023);
    int num = atoi(buffer);
    int fact = 1;
    for (int i = 1; i <= num; i++)
    {
        fact *= i;
    }
    char factStr[1024];
    sprintf(factStr, "%d", fact);
    write(newsockfd, factStr, strlen(factStr));
}

void handleImageTransfertOnServer(int newsockfd, unsigned char *buffer, int size)
{

    // get image from client

    // step 1 : get the image name
    int n = read(newsockfd, buffer, 1023);
    char *imageName = buffer;
    char *basePath = "./server-assets/";
    char *destination = malloc(strlen(basePath) + strlen(imageName) + 1);
    destination = concat(basePath, imageName);

    if (access(destination, F_OK) != -1)
    {
        // file exists
        printf("File already exists.\n");
        write(newsockfd, "File already exists.", 20);
    }
    else
    {
        printf("File does not exist.\n");
        FILE *fp;
        fp = fopen(destination, "wb");
        if (fp == NULL)
        {
            perror("fopen");
            exit(1);
        }
        printf("Receiving image from client...\n");
        while (read(newsockfd, buffer, 1024) != 0)
        {
            fwrite(buffer, 1, 1024, fp);
        }
        fclose(fp);
        printf("Image received from client.\n");
    }
}

void handleMenuChoices(int newsockfd, unsigned char *buffer, int size)
{
    int choice;
    int n = read(newsockfd, buffer, 1023);
    choice = atoi(buffer);
    switch (choice)
    {
    case 1:
        handleMessageOnServer(newsockfd, buffer, size);
        break;
    case 2:
        handleFactorialOnServer(newsockfd, buffer, size);
        break;
    case 3:
        handleImageTransfertOnServer(newsockfd, buffer, size);
        break;
    default:
        printf("Invalid choice.\n");
        break;
    }
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("Usage: %s <server-ip-address> <port>\n", argv[0]);
        exit(1);
    }
    else
    {
        // get ip address and port from command line
        char *ip = argv[1];
        char *port = argv[2];

        // start the server
        int sockfd, newsockfd, clilen, n;
        int portno = atoi(port); // 9002
        unsigned char buffer[1024];
        struct sockaddr_in serv_addr, cli_addr;
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_addr.s_addr = inet_addr(ip);
        serv_addr.sin_port = htons(portno);

        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd < 0)
        {
            perror("socket");
            exit(1);
        }
        bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
        listen(sockfd, 5);
        clilen = sizeof(cli_addr);
        newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);
        bzero(buffer, 1024);
        handleMenuChoices(newsockfd, buffer, 1023);
        // handleImageTransfertOnServer(newsockfd, buffer, 1024);
        close(newsockfd);
        close(sockfd);
        return 0;
    }
}