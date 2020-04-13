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

  // Struct for mutex
  pthread_mutex_t *mut;

  //Thread return value
  int *tret;

} thread_arg_t;

////////////////////////////////////////////////////////////////////////////////////

void *sum(void *arg);
int mutex1(const int number_thread);

////////////////////////////////////////////////////////////////////////////////////

//
typedef struct thread_arg_s {

  //Thread ID
  pthread_t tid;

  // Struct for barrier
  pthread_barrier_t barrier;

  //Thread return value
  int *tret;

} thread_arg_t2;

void *sum2(void *arg);
int barrier(const int number_thread);
