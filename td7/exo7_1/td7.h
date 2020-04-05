// Librairy td7.h

#include <stdlib.h>
#include <stdio.h>

#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <errno.h>

#define SIZE_BUF 256

typedef struct {
  int x, y;
} PAIR;

int exercice7_1();

int write_in_fifo(const char *pathname, const char *msg_write);
int read_in_fifo(const char *pathname, char *msg_read, size_t msg);
int exercice7_1_v2(const char *pipe_name);
