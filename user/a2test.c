#include "kernel/types.h"
#include "user/user.h"

#define PAGE 4096

static volatile char *
new_page(void)
{
    char *raw = sbrk(2 * PAGE);
    if(raw == (char *)-1)
        return 0;
    uint64 aligned = ((uint64)raw + PAGE - 1) & ~((uint64)PAGE - 1);
    return (volatile char *)aligned;
}

int
main(int argc, char *argv[])
{
    if(argc != 2){
        printf("Usage: a2test used|protect|readonly|nullread|nullwrite\n");
        exit(1);
    }
    // Task 1
    if(strcmp(argv[1], "used") == 0){
        int bytes = getusedmem();
        printf("getusedmem = %d bytes\n", bytes);

        if(bytes > 0 && bytes % PAGE == 0)
        printf("PASS: positive number of 4 KiB pages\n");
        else
        printf("CHECK: unexpected memory value\n");

        exit(0);
    }

    // Task 2
    if(strcmp(argv[1], "protect") == 0){
        volatile char *p = new_page();
        if(p == 0){
            printf("FAIL: sbrk\n");
            exit(1);
        }

        *p = 18;
        int protect_result = mprotect((void *)p);
        int read_result = *p;
        int unprotect_result = munprotect((void *)p);

        *p = 86;
        int write_result = *p;

        int bad_protect = mprotect((void *)(p + 1));
        int bad_unprotect = munprotect((void *)(p + 1));
        int bad_far_address = mprotect((void *)-1);

        printf("mprotect=%d, read=%d\n", protect_result, read_result);
        printf("munprotect=%d, after_write=%d\n",
            unprotect_result, write_result);
        printf("invalid addresses: %d, %d, %d\n",
            bad_protect, bad_unprotect, bad_far_address);

        if(protect_result == 0 && read_result == 18 &&
        unprotect_result == 0 && write_result == 86 &&
        bad_protect == -1 && bad_unprotect == -1 &&
        bad_far_address == -1)
        printf("PASS\n");
        else
        printf("FAIL\n");

        exit(0);
    }

    // Task 2: test read only
    if(strcmp(argv[1], "readonly") == 0){
        volatile char *p = new_page();
        if(p == 0){
        printf("FAIL: sbrk\n");
        exit(1);
        }

        *p = 18;
        if(mprotect((void *)p) != 0){
        printf("FAIL: mprotect\n");
        exit(1);
        }

        *p = 52;  
        printf("FAIL: write to read-only page succeeded\n");
        exit(1);
    }
    // Task 3: read from null
    if(strcmp(argv[1], "nullread") == 0){
        volatile uint64 address = 0;
        volatile int *p = (volatile int *)address;
        int value = *p;  
        printf("FAIL: NULL read succeeded: %d\n", value);
        exit(1);
    }

    // Task 3: write to null
    if(strcmp(argv[1], "nullwrite") == 0){
        volatile uint64 address = 0;
        volatile int *p = (volatile int *)address;
        *p = 123;  
        printf("FAIL: NULL write succeeded\n");
        exit(1);
    }
    printf("Unknown test\n");
    exit(1);
}