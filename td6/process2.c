#include "process2.h"

#include <sys/types.h>
#include <sys/wait.h>

#include <stdlib.h>
#include <stdio.h>

#include <unistd.h>
#include <string.h>

#include <errno.h>
#include <assert.h>

#define SIZE_MAX 1024

int print_current_process() {
  // List of arguments
  char *arg[] = { "ps -fux", NULL };

  // Execute an other program
  if(execv("/bin/ps", arg)  ==  -1) {
    perror("Error in 'print_current_process/execv()'");
    return errno;
  }
  return 0;
}

int print_list_files(char *path, char *option) {
  if(!path) {
    printf("Error in 'print_list_files()': path invalid");
    return -1;
  }

  // List of arguments
  if(!strcmp(path, "here")) {
    char path2[SIZE_MAX];
    getcwd(path2, SIZE_MAX);
    path = path2;
  }

  char *arg[] = { "ls", option, path, NULL };

  if(execv("/bin/ls", arg)  == -1){
    perror("Error in 'print_list_files/execv()'");
    return errno;
  }

  return 0;
}

int print_list_files_reg(char *path) {
  if(!path) {
    printf("Error in 'print_list_files()': path invalid");
    return -1;
  }

  // List of arguments
  if(!strcmp(path, "here")) {
    char path2[SIZE_MAX];
    getcwd(path2, SIZE_MAX);
    path = path2;
  }

  // find by the type some files in the dir, f for file reg, d for dir
  // char *arg[] = { "find", "-type", "f", path, NULL };
  // char *arg[] = { "find", "-type", "d", path, NULL };

  // if(execv("/bin/find", arg)  == -1){
  if(execlp("find", "find", path, "-type", "d")  == -1){
    perror("Error in 'print_list_files_reg/execv()'");
    return errno;
  }

  return 0;
}

int exercice6_1() {
  char *who_i_am = "Father";
  pid_t PID_child;

  switch(PID_child = fork()) {
    case -1:
      perror("Error in 'fork()'");
      return errno;

    case 0:
      who_i_am = "child";
      printf("%s,  PID: %d\n\n", who_i_am, (int)getpid());

      // print_current_process();

      // print_list_files("here", "-l");
      print_list_files("/home/user", "-lR");
      break;

    default:
      printf("%s, PID: %d\n", who_i_am, (int)getpid());
      wait(NULL);
  }
  return 0;
}

int exercice6_1_v2() {
  char *who_i_am = "Father";
  pid_t PID_child;

  switch(PID_child = fork()) {
    case -1:
      perror("Error in 'fork()'");
      return errno;

    case 0:
      who_i_am = "child";
      printf("%s,  PID: %d\n\n", who_i_am, (int)getpid());

      // print_current_process();

      // print_list_files("here", "-l");
      print_list_files("/home/user", "-l");
      break;

    default:
      printf("%s, PID: %d\n", who_i_am, (int)getpid());

      switch(PID_child = fork()) {
        case -1:
          perror("Error in 'fork()'");
          return errno;

        case 0:
          who_i_am = "Other child";
          printf("%s,  PID: %d\n\n", who_i_am, (int)getpid());

          // print_list_files("here", "-l");
          print_list_files_reg("/home/user");
          break;

        default:
          wait(NULL);
      }
      wait(NULL);
  }
  return 0;
}


int exercice6_3() {
  char *who_i_am = "Father";
  pid_t PID_child;

  switch(PID_child = fork()) {
    case -1:
      perror("Error in 'fork()'");
      return errno;

    case 0:
      who_i_am = "child";
      printf("%s,  PID: %d\n\n", who_i_am, (int)getpid());

      int stay_sleep = 5;
      printf("%s: start sleeping\n\n", who_i_am);
      for(int i=1; i < 6; i++) {
        if(i  ==  3) {
          kill((int)getpid, SIGSTOP);
        }
        printf("sleep(%d)\n", stay_sleep);
        sleep(1);
        stay_sleep--;
      }
      printf("\n%s end sleeping\n", who_i_am);



      break;

    default:
      printf("%s, PID: %d\n", who_i_am, (int)getpid());
      wait(NULL);
  }
  return 0;
}
