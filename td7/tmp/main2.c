#include <stdlib.h>
#include <stdio.h>

#include <sys/types.h>
#include <sys/stat.h>

#include <errno.h>

#include <unistd.h>
#include <string.h>
#include <fcntl.h>

#define SIZE_BUF 256

int main(int argc, char **argv)
{
    const char *message = "Hello World !";
    int fd = -1;

    char *name_pipe = "pipe1";
    char pwd[SIZE_BUF];
    if( !getcwd(pwd, SIZE_BUF) ) {
        perror("Error with 'getcwd()' in 'exercice7_1_v2()'");
        return errno;
    }
    strcat(pwd, "/");
    strcat(pwd, name_pipe);


    printf("Create the PIPE...\n");
    if( ( mkfifo(pwd, 0666) )  !=  0 ) {
        perror("Error with 'mkfifo()' in main2.c");
        return errno;
    }

    if( ( fd = open(pwd, O_WRONLY | O_NONBLOCK) )  ==  -1 ) {
        perror("Error with 'open()' in main2.c");
        return errno;
    }

    if( write(fd, message, strlen(message) )  ==  -1 ) {
        perror("Error with 'write()' in main2.c");
        return errno;
    }

    close(fd);
    unlink(name_pipe);

    exit(0);
}
