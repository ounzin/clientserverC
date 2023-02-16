#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>

#include "utils.c"

int main(int argc, char *argv[])
{
    // get argument from command line
    char *port = argv[1];
    char *ip = argv[2];

    // print size of argv
    printf("Size of argv: %d\n", argc);

    // prints the argument
    printf("Port: %s\n", port);
    printf("IP: %s\n", ip);
}