#include "td7.h"

// with pipe
int exercice7_1() {
  char msg_write[16] = "KONO DIO DA", msg_read[16];

  pid_t PID;
  int fds[2];

  if( pipe(fds)  !=  0 ) {
      perror("Error with 'pipe()' in 'exercice7_1()'");
      return errno;
  }

  switch(PID = fork()) {
     case -1:
        perror("Error with 'fork()' in 'exercice7_1()'");
        return errno;

     case 0:
        printf("Child : PID = %d  Close entry\n", (int)getpid());
        close(fds[1]);

        read(fds[0], msg_read, 16*sizeof(char));
        printf("Message read by child: %s\n", msg_read);

        break;

     default:
      printf("Father: PID = %d  Close exit\n", (int)getpid());
      close(fds[0]);

      printf("Message write by father\n");
      write(fds[1], msg_write, 16*sizeof(char));

      wait(NULL);
  }
  return 0;
}

// with fifo
int write_in_fifo(const char *pathname, const char *msg_write) {
  if( !pathname  ||  !msg_write) {
    printf("Error in 'write_in_fifo()': pathname or msg_write invalid");
    return -1;
  }

  int fd = -1;

  if( mkfifo(pathname, 0666)  ==  -1 ) {
    perror("Error with 'mkfifo()' in 'write_in_fifo()'");
    return errno;
  }

  if( ( fd = open(pathname, O_WRONLY) )  ==  -1 ) {
    perror("Error with 'open()' in 'write_in_fifo()'");
    return errno;
  }

  if( ( write(fd, msg_write, strlen(msg_write)) )  ==  -1 ) {
    perror("Error with 'write()' in 'write_in_fifo()'");
    return errno;
  }

  if( ( close(fd) )  ==  -1 ) {
    perror("Error with 'close()' in 'write_in_fifo()'");
    return errno;
  }

  return 0;
}

int read_in_fifo(const char *pathname, char *msg_read, size_t s_msg) {
  if( !pathname  ||  !msg_read ) {
    printf("Error in 'read_in_fifo()': pathname or msg_read invalid");
    return -1;
  }

  int fd = -1;

  if( ( fd = open(pathname, O_RDONLY) )  ==  -1 ) {
    perror("Error with 'open()' in 'read_in_fifo()'");
    return errno;
  }

  size_t s_read;
  if( ( s_read = read(fd, msg_read, s_msg) )  ==  -1 ) {
    perror("Error with 'read()' in 'read_in_fifo()'");
    return errno;
  }

  if( s_read  <=  s_msg )
    msg_read[s_read] = '\0';

  if( ( unlink(pathname) )  ==  -1 ) {
    perror("Error with 'unlink()' in 'read_in_fifo()'");
    return errno;
  }
  return 0;
}

int exercice7_1_v2(const char *pipe_name) {
  if( !pipe_name ) {
    printf("Error in 'exercice7_1_v2()': pipe_name invalid");
    return -1;
  }

  // const char *msg_write = "KONO DIO DA";
  const char *msg_write = "Hello there";
  char msg_read[16];

  char pwd[SIZE_BUF];
  if( !getcwd(pwd, SIZE_BUF) ) {
    perror("Error with 'getcwd()' in 'exercice7_1_v2()'");
    return errno;
  }
  strcat(pwd, "/");
  strcat(pwd, pipe_name);

  pid_t PID;
  switch( PID = fork() ) {
    case -1:
      perror("Error with 'fork()' in 'exercice7_1_v2()'");
      return errno;

    case 0:
      read_in_fifo(pwd, msg_read, 16);
      printf("Message read by child   (PID = %d): %s\n", (int)getpid(), msg_read);
      break;

    default:
      printf("Message write by father (PID = %d)\n", (int)getpid());
      write_in_fifo(pwd, msg_write);
      wait(NULL);
      break;
  }
  return 0;
}
