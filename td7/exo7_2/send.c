#include <stdlib.h>
#include <stdio.h>

#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <time.h>

#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>

#include <errno.h>

#define SIZE_BUF 256
#define TUBESIG "tube-signal"
#define TUBEPID "tube-pid"

int send_signal(const int signal) {
  int fd = -1;

  printf("In send.c: 'send_signal()': Message send\n");
  if( mkfifo(TUBESIG, 0666)  ==  -1 ) {
    perror("Error with 'mkfifo()' in 'send_signal()'");
    return errno;
  }

  if( ( fd = open(TUBESIG, O_WRONLY) )  ==  -1 ) {
    perror("Error with 'open()' in 'send_signal()'");
    return errno;
  }

  write(fd, &signal, sizeof(int));

  close(fd);
  return 0;
}

int receive_pid() {
  pid_t pid_read;
  int fd = -1;

  if( ( fd = open(TUBEPID, O_RDONLY) )  ==  -1 ) {
    perror("Error with 'open()' in 'receive_pid()'");
    return errno;
  }

  read(fd, &pid_read, sizeof(int));
  printf("In send.c: 'receive_pid()': My PID: %d, Msg received: %d\n",
    (int)getpid(), (int)pid_read);

  return 0;
}

int main(int argc, char const *argv[])
{
  receive_pid();
  send_signal(SIGUSR1);
  return 0;
}
