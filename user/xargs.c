#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"

#define MAXLINE       1024  // 每一行的最大字节数

int readline(int fd, char *buf) {
    int len = 0;
    char *p = buf;
    while(read(fd, p, 1) > 0) {
        len ++;
        if(*p == '\n') break;
        if(len == MAXLINE) {
            fprintf(2, "xargs : more than 1024 Bytes in one line");
            exit(1);
        }
        p ++;
    }
    return len;        
}

char whitespace[] = " \t\r\n\v";

int
main(int argc, char *argv[])
{   
    char buf[MAXLINE];
    char * execargv[MAXARG];
    int execargc = argc - 1;
    for(int i = 1; i < argc; i ++)
        execargv[i - 1] = argv[i];
    int len;
    while((len = readline(0, buf)) > 0) {
        buf[len] = 0;
        execargc = argc - 1;
        for(int i = 0; i < len; i ++) {
            while(i != len && strchr(whitespace, buf[i])) i ++;
            if(i != len) {
                if(execargc == MAXARG) {
                    fprintf(2, "xargs : much more arguments then 32");
                    exit(1);
                }
                execargv[execargc ++] = buf + i;
                while(i != len && !strchr(whitespace, buf[i])) i ++;
                buf[i] = 0;
            }
        }
        execargv[execargc] = 0;
        if(fork() == 0) {
            exec(execargv[0], execargv);
        } else wait(0);
    }
    exit(0);
}


