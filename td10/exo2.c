#include "exo2.h"

void *sum(void *arg) {

  // Cast
  thread_arg_t *targ = (thread_arg_t *)arg;

  //
  printf("thread n°%ld add done\n", targ->tid);

  // Try lock mutex
  int lock;
  if( !( lock = pthread_mutex_lock( targ->mut ) ) ) {

    // Add 1
    *(targ->tret) += 1;
  }
  else
    perror("sum: pthread_mutex_lock failed\n");

  // If lock is good, try unlock
  if( !lock ) {
    if( !pthread_mutex_unlock( targ->mut ) ) {

      // Lock is good and unlock is good, leave function
      pthread_exit( NULL );
      return NULL;
    }

    else
      perror("sum: pthread_mutex_unlock failed\n");
  }

  //
  pthread_exit( NULL );
  return NULL;
}

int fence1(int number_thread) {

  // Init struct table
  thread_arg_t *tab_targ = malloc( number_thread * sizeof(thread_arg_t) );
  if( !tab_targ ) {
    printf("fence1: wrong alloc 'tab_targ'\n");
    return -1;
  }

  // Init mutex
  pthread_mutex_t mut;
  if( pthread_mutex_init( &mut, NULL ) ) {
    printf("fence1: pthread_mutex_init failed\n");
    return -1;
  }

  //
  int global_sum = 0;

  //
  for( int i = 0 ; i < number_thread ; i++ ) {
    tab_targ[i].mut  = &mut;
    tab_targ[i].tret = &global_sum;
  }

  // Create all threads
  for( int i = 0 ; i < number_thread ; i++ ) {
    if( pthread_create( &tab_targ[i].tid, NULL, sum, (void *)&tab_targ[i] ) ) {
      perror("fence1: pthread_create failed\n");
      return errno;
    }
  }

  // Join all threads
  for( int i = 0 ; i < number_thread ; i++ ) {
    if( pthread_join( tab_targ[i].tid, NULL ) ) {
      perror("fence1: pthread_join failed\n");
      return errno;
    }
  }

  //  Destoy mutex
  if( pthread_mutex_destroy( &mut ) ) {
    printf("fence1; pthread_mutex_destroy failed\n");
    return -1;
  }

  // Print
  for( int i = 0 ; i < number_thread ; i++ )
    printf("tab_targ[%d].tret = %d\n", i, *(tab_targ[i].tret) );
  printf("global_sum = %d\n", global_sum );

  return 0;
}
