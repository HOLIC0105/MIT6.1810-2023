#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define output pipefd[1]

int 
main(int argc, char const *argv[])
{
    int input, pipefd[2];
    pipe(pipefd);
    if(fork()) {
        close(pipefd[0]);
        for(int i = 2; i <= 35; i ++) {
            write(output, &i, sizeof(i));
        }
        close(output);
    } else {
        PRIME:
            close(output);
            input = pipefd[0];
            int p;
            if(read(input, &p, sizeof(p)) > 0) { 
                fprintf(1, "prime %d\n", p);
                pipe(pipefd);
                if(fork() == 0) goto PRIME;
                close(pipefd[0]);
                int tmpval;
                while(read(input, &tmpval, sizeof(tmpval)) > 0) {
                    if(tmpval % p) {
                        write(output, &tmpval, sizeof(tmpval));
                    }
                }
                close(output);
            } 
        close(input);
    }
    wait(0);
    exit(0);
}
