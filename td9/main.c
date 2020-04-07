#include <stdlib.h>
#include <stdio.h>

#include "thread.h"

int main(int argc, char const *argv[])
{
  main1();
  main2(1000);
  main3(1000);
  main4();

  return 0;
}
