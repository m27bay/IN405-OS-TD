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

typedef struct {
  char msg[SIZE_BUF][SIZE_BUF];
  int position, num_msg, max_size;
} DATA;

void get_time(char *buf) {
  time_t timep = time( NULL );
  ctime_r(&timep, buf);
  buf[strlen(buf)-1] = '\0';
}

DATA data_init() {
  DATA data;
  strcpy(data.msg[0], "Bonjour, ceci est un affichage informatisé");

  char buf[SIZE_BUF];
  get_time(buf);
  strcpy(data.msg[1], buf);

  data.position = 0;
  data.num_msg  = 2;
  data.max_size = SIZE_BUF;

  return data;
}

void data_print(DATA *data) {
  if(data->position  >=  data->num_msg) {
    printf("Reset\n");
    data->position = 0;
  }

  sleep(2);
  printf("%s\n", data->msg[data->position]);
  data->position+=1;
}

int receive_signal(int signal_read) {
  int fd = -1;

  if( ( fd = open(TUBESIG, O_RDONLY) )  ==  -1 ) {
    perror("Error with 'open()' in 'receive_signal()'");
    return errno;
  }

  size_t size_r;
  if( ( size_r = read(fd, &signal_read, sizeof(int)) )  ==  -1 ) {
    perror("Error with 'read()' in 'receive_signal()'");
    return errno;
  }

  printf("Msg received: %d\n", signal_read);

  return 0;
}

int send_pid(pid_t PID) {
  int fd = -1;

  printf("Pid send\n");
  if( mkfifo(TUBEPID, 0666)  ==  -1 ) {
    perror("Error with 'mkfifo()' in 'send_pid()'");
    return errno;
  }

  if( ( fd = open(TUBEPID, O_WRONLY) )  ==  -1 ) {
    perror("Error with 'open()' in 'send_pid()'");
    return errno;
  }

  write(fd, &PID, sizeof(int));

  close(fd);
  return 0;
}


void del_tube(const char* name_tube) {
  unlink(name_tube);
}

int treatment() {
  int signal_read = 0;
  receive_signal(signal_read);

  // if(  )

  return 0;
}


int main(int argc, char const *argv[]) {
  // char pwd[SIZE_BUF];
  // if( !getcwd(pwd, SIZE_BUF) ) {
  //     perror("Error with 'getcwd()' in 'print.c'");
  //     return errno;
  // }
  // strcat(pwd, "/");

  // DATA data = data_init();
  // data_print(&data);
  // data_print(&data);
  // data_print(&data);
  printf("SIGUSR1: %d\nSIGUSR2: %d\n\n", SIGUSR1, SIGUSR2);

  printf("My PID: %d\n", (int)getpid());
  send_pid(getpid());

  treatment();

  del_tube(TUBESIG);
  del_tube(TUBEPID);

  return 0;
}
