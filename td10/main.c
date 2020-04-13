#include "exo2.h"

int main(int argc, char const *argv[])
{
  // Test mutex with three threads
  mutex1(3);

  // Test mutex with ten threads
  mutex1(10);

  // progam's end
  return 0;
}
