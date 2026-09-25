#include "kernel/types.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  char *address = sbrk(100);

  if(address == (char *)-1){
    printf("sbrk failed\n");
    exit(1);
  }

  *address = 0x12;

  printf("mprotect: %d\n", mprotect(address));
  printf("Before: %d\n", (int)*address);

  //*address = 0x34;

  printf("munprotect: %d\n", munprotect(address));

  *address = 0x56;
  printf("After: %d\n", (int)*address);

  printf("Invalid: %d\n", mprotect(address + 1));

  exit(0);
}