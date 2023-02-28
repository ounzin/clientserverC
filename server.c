#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <ctype.h>
#include <pthread.h>

#include "utils.c"
#include "handler.h"

void handleMessageOnServer(int newsockfd, char *buffer, int size)
{
    int n = read(newsockfd, buffer, 1023);
    printf("Here is the message: %s\n", buffer);
    write(newsockfd, "I got your message", 18);
}

void handleFactorialOnServer(int newsockfd, char *buffer, int size)
{
    printf("Calculating factorial...\n");
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

void handleImageTransfertOnServer(int newsockfd, char *buffer, int size)
{
    // step 1 : get the image name
    read(newsockfd, buffer, 1024);
    printf("Image name: %s\n", buffer);
    char *imageName = buffer;
    char *basePath = "./server-assets/";
    char *destination = malloc(strlen(basePath) + strlen(imageName) + 1);
    destination = concat(basePath, imageName);

    if (access(destination, F_OK) != -1)
    {
        // file exists
        printf("File already exists.\n");
    }
    else
    {
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

void handleMenuChoices(int newsockfd, char *buffer, int size)
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

void *handle_thread(void *arg)
{

    ThreadArgs *receivedArgs = (ThreadArgs *)arg;
    printf("Client %s connected.\n", inet_ntoa(receivedArgs->cli_addr.sin_addr));
    char buffer[1024];
    bzero(buffer, 1024);
    handleMenuChoices(receivedArgs->newsockfd, buffer, 1024);
    pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        printf("Usage: %s <server-ip-address> <port>\n", argv[0]);
        exit(1);
    }
    else
    {
        printf("Loading server...\n");
        // get ip address and port from command line
        char *ip = argv[1];
        char *port = argv[2];

        // start the server
        int sockfd, newsockfd, clilen, n;
        int portno = atoi(port); // 9002
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
        printf("Server started.\n");

        if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)))
        {
            perror("bind");
            exit(1);
        }
        else
        {
            listen(sockfd, 5);
            printf("Server listening on port %d ...\n", portno);
            // handle clients until kill signal is received
            while (1)
            {
                printf("----------------------------------\n");
                printf("\n Waiting for client...\n");
                printf("----------------------------------\n\n");
                clilen = sizeof(cli_addr);
                newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, (socklen_t *)&clilen);
                if (newsockfd < 0)
                {
                    perror("accept");
                    exit(1);
                }
                else
                {
                    // create a new thread to handle the client
                    pthread_t thread_id;

                    ThreadArgs *arg = malloc(sizeof(ThreadArgs));
                    arg->newsockfd = newsockfd;
                    arg->cli_addr = cli_addr;
                    int p = pthread_create(&thread_id, NULL, handle_thread, arg);
                    if (p < 0)
                    {
                        perror("pthread_create");
                        exit(1);
                    }
                    else
                    {
                        pthread_join(thread_id, NULL);
                    }
                    free(arg);
                }
            }
        }

        // close the socket
        close(newsockfd);
        close(sockfd);

        return 0;
    }
}