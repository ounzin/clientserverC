#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <ctype.h>

void handleMessageOnServer(int newsockfd, char *buffer, int size)
{
    int n = read(newsockfd, buffer, 255);
    printf("Here is the message: %s\n", buffer);
    write(newsockfd, "I got your message", 18);
}

void handleFactorialOnServer(int newsockfd, char *buffer, int size)
{
    int n = read(newsockfd, buffer, 255);
    int num = atoi(buffer);
    int fact = 1;
    for (int i = 1; i <= num; i++)
    {
        fact *= i;
    }
    char factStr[256];
    sprintf(factStr, "%d", fact);
    write(newsockfd, factStr, strlen(factStr));
}

void handleImageTransfertOnServer(int newsockfd, char *buffer, int size)
{
    // get image from client
    FILE *fp;
    fp = fopen("cat1.jpg", "wb");
    if (fp == NULL)
    {
        perror("fopen");
        exit(1);
    }
    printf("Receiving image from client...\n");
    while (read(newsockfd, buffer, 1024) != 0)
    {
        fprintf(fp, "%s", buffer);
    }
    printf("Image received from client.\n");
    fclose(fp);
}

void handleMenuChoices(int newsockfd, char *buffer, size)
{
    int choice;
    int n = read(newsockfd, buffer, 255);
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

int main()
{
    int sockfd, newsockfd, clilen, n;
    int portno = 9002;
    char buffer[256];
    struct sockaddr_in serv_addr, cli_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
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
    bzero(buffer, 256);
    handleMenuChoices(newsockfd, buffer, 255);
}