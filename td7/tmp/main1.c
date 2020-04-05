#include <stdio.h>
#include <stdlib.h>

#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#include <errno.h>

#include <sys/types.h>
#include <sys/stat.h>

#define SIZE_BUF 256

int main(int argc, char **argv)
{
    char message[256];
    int fd;

    char *name_pipe = "tube_TD7exo1";
    char pwd[SIZE_BUF];
    if( !getcwd(pwd, SIZE_BUF) ) {
        perror("Error with 'getcwd()' in 'exercice7_1_v2()'");
        return errno;
    }
    strcat(pwd, "/");
    strcat(pwd, name_pipe);

    fd = open(pwd, O_RDONLY | O_NONBLOCK);

    read(fd, message, 256);
    close(fd);

    printf("Message receive : %s \n", message);

    exit(0);
}
