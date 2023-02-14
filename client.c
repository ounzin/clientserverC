#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <ctype.h>

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

void handleImageTransfertOnClient(char *buffer, int sockfd)
{
    FILE *fp;
    fp = fopen("cat.jpg", "rb");
    if (fp == NULL)
    {
        perror("fopen");
        exit(1);
    }
    printf("Sending image to server...\n");
    while (fgets(buffer, 1024, fp) != NULL)
    {
        write(sockfd, buffer, strlen(buffer));
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
    choice = atoi(buffer);
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

        exit(0);
        break;
    }
}
int main()
{
    int sockfd;
    int portno = 9002;
    struct sockaddr_in serv_addr;
    char buffer[1024];

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        perror("socket");
        exit(1);
    }
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);
    connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    displayMenuAndChooseOperation(sockfd, buffer, 1024);
}