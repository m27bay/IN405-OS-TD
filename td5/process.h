#include "stdlib.h"
#include "stdio.h"

#include "unistd.h"
#include "errno.h" // For error
#include "time.h"  //  For random number

#include "sys/types.h"
#include "sys/wait.h"
#include <signal.h>

#include <assert.h>

int who_i_am();
int who_i_am_v2();

int randint(const int rand_max);

int manip_process();

char *print_who_i_am(pid_t pid);

int create_simple_process();

void exercice2(const int num_child);
