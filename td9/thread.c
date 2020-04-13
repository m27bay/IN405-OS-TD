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

  // Cast
  char *str = (char *)arg;

  //
  printf("Child says; '%s'\n", str);

  // Exit
  pthread_exit( NULL );
  return NULL;
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

  // Exit
  return 0;
}

/////////////////////////////////////////////////////////////////////////////////////

void *print_int(void *arg) {

  // Cast
  int *alea = (int *)arg;

  //
  printf("Child: number alea = %d\n", *alea);

  // Exit
  pthread_exit( NULL );
  return NULL;
}

int main2(int base) {

  // Init the thread variable
  pthread_t thread;

  // Create the thread
  srand( getpid() );
  int alea = rand() % base;

  // Create the thread
  if( pthread_create( &thread, NULL, print_int, &alea ) ) {
    perror("In thread.c: Error with 'pthread_create()'\n");
    return errno;
  }

  // Join the thread
  if( pthread_join( thread, NULL ) ) {
    printf("In thread.c: Error with 'pthread_join()'\n");
    return errno;
  }

  // Exit
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

  // Cast
  thread_arg_t *t = (thread_arg_t *)arg;

  // Init
  srand( time( NULL ) );
  t->tret = rand() % t->targ;
  printf("Child: number alea = %d\n", t->tret);

  // Exit
  pthread_exit( NULL );
  return NULL;
}

int main3(int base) {

  // Init the struct
  thread_arg_t t;

  // Fill the struct
  t.targ = base;
  t.tret = 0;

  // Create the thread
  if( pthread_create( &t.tid, NULL, init_alea, &t ) ) {
    perror("In thread.c: Error with 'pthread_create()'\n");
    return errno;
  }

  // Join the thread
  if( pthread_join( t.tid, NULL) ) {
    printf("In thread.c: Error with 'pthread_join()'\n");
    return errno;
  }

  //
  printf("Father: number alea = %d\n", t.tret);

  // Exit
  return 0;
}

/////////////////////////////////////////////////////////////////////////////////////

typedef struct {

  // Average of element
  float tab_average;

  // Number of element
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

  // Cast
  thread_arg_t2 *t2= (thread_arg_t2 *)arg;

  // Sum elements
  int sum = 0;
  for( int i = 0 ; i < t2->tabint.count ; i++ )
    sum += t2->tabint.tab[i];

  //
  printf("Child: sum of elements = %d\n", sum);

  // Calcul average
  t2->tabint.tab_average = (float)sum / t2->tabint.count;

  //
  printf("Child: moy = %.2f\n", t2->tabint.tab_average );

  // Exit
  pthread_exit( NULL );
  return NULL;
}

int main4() {

  // Init the struct
  thread_arg_t2 thread;

  // Init average
  thread.tabint.tab_average = -1;

  // Fill the number of elements
  printf("Give me the number of elements: "); scanf("%d", &thread.tabint.count);
  thread.tabint.tab = malloc( thread.tabint.count * sizeof(int) );

  // If malloc is good
  if( thread.tabint.tab ){

    // Fill the table with few elements
    for( int i = 0 ; i < thread.tabint.count ; ++i) {
      printf("Give me a number: ");
      scanf("%d", &thread.tabint.tab[i]);
    }

    // Create the thread
    if( pthread_create( &thread.tid, NULL, moyenne, &thread ) ) {
      perror("In thread.c: Error with 'pthread_create()'\n");
      return errno;
    }

    // Join the threads
    if( pthread_join( thread.tid, NULL ) ) {
      printf("In thread.c: Error with 'pthread_join()'\n");
      return errno;
    }

    //
    printf("Father: moy = %.2f\n", thread.tabint.tab_average);

    // Relaese
    free(thread.tabint.tab);
  }

  else {
    printf("main4: wrong alloc of 'thread.tabint.tab'\n");
  }

  return 0;
}

/////////////////////////////////////////////////////////////////////////////////////
