#include <stdlib.h>
#include <stdio.h>

#include <sys/times.h>

#include <signal.h>

#include "process2.h"

int main(int argc, char const *argv[])
{
  printf("\n");
  /*
  clock_t start = times(NULL);
  exercice6_1();
  clock_t end = times(NULL);
  float time_exe = (float)(end-start);
  printf("time_exe = %.2f ms\n", time_exe);
  */

  exercice6_1_v2();

  // exercice6_3();

  return 0;
}
