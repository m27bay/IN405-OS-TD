#include "process.h"

int who_i_am() {
  pid_t PID;
  char *who_i_am = "father";

  switch(PID = fork()) {
    case -1:
      perror("Error in 'fork()':");
      exit(errno);

    case 0: /* child process */
      who_i_am = "child";
      printf("%s : my PID: %d, my child  PID: %d\n",
        who_i_am, (int)getpid(), (int)getppid());
      break;

    default: /* father process */
      printf("%s: my PID: %d, my father PID: %d\n",
        who_i_am, (int)getpid(), PID);
      wait(NULL);
  }

  return 0;
}

int who_i_am_v2() {
  pid_t PID;
  char *who_i_am = "father";

  PID = fork();
  assert(PID != -1);
  if(!PID) { /* child process */
    who_i_am = "child";
    printf("who_i_am = %s\n", who_i_am);
  }
  else { /* father process */
    printf("who_i_am = %s\n", who_i_am);
    wait(NULL);
  }
  return 0;
}

void print_var(char *str, int *tab_int, int size_tab, double pi) {
  printf("str: %s\n", str);

  printf("tab int: {");
  for(int i = 0; i < size_tab; i++)
    printf(" %d, ", tab_int[i]);
  printf("}\n");

  printf("double: %f\n", pi);
}

int manip_process() {
  pid_t PID;  // Init var for the fork

  char *str = "Hello there";
  int tab_int[5] = {0, 1, 2, 3, 4};
  double pi = 3.14159265359;

  // Creat the process
  PID = fork();
  if(!PID) {  // Child process execution
    printf("\nChild: \n");
    tab_int[0] = 10;
    print_var(str, tab_int, 5, pi);
    printf("My PID: %d\tfather PID: %d\n",
      (int)getpid(), (int)getppid());
  }
  else {     // Father process execution
    printf("Father: \n");
    print_var(str, tab_int, 5, pi);
    printf("My PID: %d\tchild PID: %d\n",
      (int)getpid(), PID);
    wait(NULL);
  }

  return 0;
}

char *print_who_i_am(pid_t pid) {
  if(pid)
    return "father";
  else
    return "child";
}

int create_simple_process() {
  // Init var for the fork
  pid_t pid;
  int int_read, int_write;

  char *str = "Hello world";

  // Creat pipe BEFORE fork
  int tube[2];  // tube[0]: exit of the pipe
                // tube[1]: entry of the pipe
  if(pipe(tube)  !=  0) {
    printf("Error in 'create_simple_process()': can't create the pipe");
    return -1;
  }

  switch(pid  =  fork()) {  // Creat the process

    case -1:
      printf("Error in 'create_simple_process()': can't create the fork");
      return -1;

    case 0:  // Child process
      printf("\n\t  %s: \nstr            : %s\nmy pid         : %d\nmy father pid  : %d\n",
        print_who_i_am(pid), str, getpid(), getppid());

      // Init random number
      srand ( time(NULL) );
      int_write = rand() % 50;

      // Send message
      printf("\nChild send a message to her father (send: %d)\n", int_write);
      close(tube[0]);
      write(tube[1], &int_write, sizeof(int));
      break;

    default:  // Father process
      printf("\n\t  %s: \nstr            : %s\nmy pid         : %d\nmy child pid   : %d\n",
        print_who_i_am(pid), str, getpid(), pid);  // Child pid is the Value return by the fork

      // Receive message
      close(tube[1]);
      read(tube[0], &int_read, sizeof(int));
      printf("Father receive a message to her child: %d\n", int_read);

      // Close process
      wait(NULL); // Or wait(&status);
      break;
  }
  return 0;
}

int randint(const int rand_max) {
  if(rand_max  <=  0)
    return -1;

  int random = 1;
  do {
    srand ( time(NULL) );
    random = rand() % rand_max;
  } while(!random);

  return random;
}

void exercice2(const int num_child) {
  pid_t pid = 1;
  int wstatus;

  // test if pid > 0
  // pid == -1: error
  // pid == 0: father, don't need to creat an other
  // father, just many children

  for (int i=0; (i < num_child) && (pid > 0); i++) {
    pid = fork();
    assert(pid != -1);
    wait(&wstatus);
  }

  // Child process //
  if(!pid) {
    int second = 0;
    do {
      srand ( time(NULL) );
      second = rand() % 10;
    } while(!second);

    // Sleep the process
    printf("Wait: %ds\n", second);
    sleep(second);
    printf("Child : PID = %d\n", (int)getpid());
  }

  // Father process //
  else {
    printf("\n");
    if(WIFEXITED(wstatus))
      printf("My child: %d end\n", pid);
    printf("Father: PID = %d\n", (int)getpid());
    wait(&wstatus);
  }
}
