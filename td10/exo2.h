// Librairy exo2.h

//
#include <stdlib.h>
#include <stdio.h>

//
#include <pthread.h>

// For getpid(), ..
#include <sys/types.h>
#include <unistd.h>

// For time(), ..
#include <time.h>

// For perror(), errno, ..
#include <errno.h>

//
#include <unistd.h>

//
#include <errno.h>

//
typedef struct thread_arg_s {

  //Thread ID
  pthread_t tid;

  //
  pthread_mutex_t *mut;

  //Thread return value
  int *tret;

} thread_arg_t;

void *sum(void *arg);
int fence1(int number_thread);
