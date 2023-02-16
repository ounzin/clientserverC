#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *concat(const char *s1, const char *s2)
{
    char *result = malloc(strlen(s1) + strlen(s2) + 1); // +1 for the null-terminator
    // in real code you would check for errors in malloc here
    strcpy(result, s1);
    strcat(result, s2);
    return result;
}

// remove white spaces from a string
char *removeWhiteSpaces(char *str)
{
    char *end;
    // Trim leading space
    while (isspace((unsigned char)*str))
        str++;
    if (*str == 0) // All spaces?
        return str;
    // Trim trailing space
    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end))
        end--;
    // Write new null terminator
    end[1] = '\0';
    return str;
}