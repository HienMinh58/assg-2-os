#include "kernel/types.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  volatile int *pointer = (volatile int *)0;

  // Attempt to read virtual address 0.
  *pointer = 123;

  // This line must never be reached.
  printf("ERROR: process was not killed\n");

  exit(0);
}