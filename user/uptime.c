
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char const *argv[])
{
    if(argc != 1) {
        fprintf(2, "uptime: lnvalid argument\n");
        exit(1);
    }
    fprintf(1, "%l\n", uptime());
    exit(0);
}
