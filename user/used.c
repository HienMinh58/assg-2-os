#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[]){
    if(argc == 0 || argc >= 2){
        fprintf(2, "usage: used\n");
        exit(1);
    }
    fprintf(2, "Used memory: %d bytes\n", getusedmem());
    exit(0);
}