#include <stdlib.h>
#include <stdio.h>

#include <pthread.h>

// For getpid(), ..
#include <sys/types.h>
#include <unistd.h>

// For time(), ..
#include <time.h>

// For perror(), errno, ..
#include <errno.h>

#include "thread.h"

/////////////////////////////////////////////////////////////////////////////////////

void *print_str(void *arg) {
  char *str = (char *)arg;

  printf("Child says; '%s'\n", str);

  pthread_exit( NULL );
}

int main1() {
  // Init the thread variable
  pthread_t thread;

  // Create the thread
  if( pthread_create( &thread, NULL, print_str, "Hello world" ) ) {
    perror("In thread.c: Error with 'pthread_create()'");
    return errno;
  }

  // Close the thread
  if( pthread_join( thread, NULL ) ) {
    printf("In thread.c: Error with 'pthread_join()'\n");
    return errno;
  }

  return 0;
}

/////////////////////////////////////////////////////////////////////////////////////

void *print_int(void *arg) {
  int *alea = (int *)arg;

  printf("Child: number alea = %d\n", *alea);

  pthread_exit( NULL );
}

int main2(int base) {
  // Init the thread variable
  pthread_t thread;

  // Create the thread
  srand( getpid() );
  int alea = rand() % base;

  if( pthread_create( &thread, NULL, print_int, &alea ) ) {
    perror("In thread.c: Error with 'pthread_create()'\n");
    return errno;
  }

  // Close the thread
  if( pthread_join( thread, NULL ) ) {
    printf("In thread.c: Error with 'pthread_join()'\n");
    return errno;
  }

  return 0;
}

/////////////////////////////////////////////////////////////////////////////////////

typedef struct {

  //Thread ID
  pthread_t tid;

  //Thread argument/parameter
  int targ;

  //Thread return value
  int tret;

} thread_arg_t;

void *init_alea(void *arg) {

  //
  thread_arg_t *t = (thread_arg_t *)arg;

  //
  srand( time( NULL ) );
  t->tret = rand() % t->targ;
  printf("Child: number alea = %d\n", t->tret);

  //
  pthread_exit( NULL );
}

int main3(int base) {
  thread_arg_t t;

  t.targ = base;
  t.tret = 0;

  // Create the thread
  if( pthread_create( &t.tid, NULL, init_alea, &t ) ) {
    perror("In thread.c: Error with 'pthread_create()'\n");
    return errno;
  }

  // Close the thread
  if( pthread_join( t.tid, NULL) ) {
    printf("In thread.c: Error with 'pthread_join()'\n");
    return errno;
  }

  printf("Father: number alea = %d\n", t.tret);

  return 0;
}

/////////////////////////////////////////////////////////////////////////////////////

typedef struct {
  //
  float tab_moy;

  //
  int count;

  //
  int *tab;

} tabint_t;

typedef struct {

  //Thread ID
  pthread_t tid;

  //
  tabint_t tabint;

} thread_arg_t2;

void *moyenne(void *arg) {

  //
  thread_arg_t2 *t2= (thread_arg_t2 *)arg;

  //
  int sum = 0;
  for( int i = 0 ; i < t2->tabint.count ; i++ )
    sum += t2->tabint.tab[i];

  //
  printf("Child: sum of elements = %d\n", sum);
  t2->tabint.tab_moy = (float)sum / t2->tabint.count;

  printf("Child: moy = %f\n", t2->tabint.tab_moy );

  //
  pthread_exit( NULL );
}

int main4() {

  //
  thread_arg_t2 thread;

  //
  thread.tabint.tab_moy = -1;

  //
  printf("Give me the number of elements: "); scanf("%d", &thread.tabint.count);
  thread.tabint.tab = malloc( thread.tabint.count * sizeof(int) );

  //
  if( thread.tabint.tab ){

    //
    for( int i = 0 ; i < thread.tabint.count ; ++i) {
      printf("Give me a number: ");
      scanf("%d", &thread.tabint.tab[i]);
    }


    //
    if( pthread_create( &thread.tid, NULL, moyenne, &thread ) ) {
      perror("In thread.c: Error with 'pthread_create()'\n");
      return errno;
    }

    //
    if( pthread_join( thread.tid, NULL ) ) {
      printf("In thread.c: Error with 'pthread_join()'\n");
      return errno;
    }

    //
    printf("Father: moy = %f\n", thread.tabint.tab_moy);

    //
    free(thread.tabint.tab);
  }


  return 0;
}

/////////////////////////////////////////////////////////////////////////////////////
