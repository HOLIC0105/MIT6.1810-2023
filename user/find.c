#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"

void
find(const char *path, const char *filename) {
    char buf[512], *p;
    int fd;
    struct dirent de;
    struct stat st;
    if((fd = open(path, O_RDONLY)) < 0){
        fprintf(2, "find: cannot open %s\n", path);
        exit(1);
    }
    if(fstat(fd, &st) < 0){
        fprintf(2, "find: cannot stat %s\n", path);
        close(fd);
        exit(1); 
    }   
    if(st.type != T_DIR) return;
    if(strlen(path) + 1 + DIRSIZ + 1 > sizeof(buf)) {
        printf("fd: path too long\n");
        exit(1);
    }
    strcpy(buf, path);
    p = buf + strlen(buf);
    *p++ = '/';
    while(read(fd, &de, sizeof(de)) == sizeof(de)) {
        if(de.inum == 0) continue;
        memmove(p, de.name, DIRSIZ);
        p[DIRSIZ] = 0;
        if(stat(buf, &st) < 0){
            printf("find: cannot stat %s\n", buf);
            continue;
        }
        switch (st.type)
        {
        case T_DEVICE:
        case T_FILE:
            if(strcmp(de.name, filename) == 0) {
                fprintf(1, "%s\n", buf);
            }
            break;
        case T_DIR:
            if(strcmp(de.name, ".") && strcmp(de.name, ".."))
                find(buf, filename);
            break;
        default:
            break;
        }
    }
}

int
main(int argc, char const *argv[])
{
    if(argc != 3){
        fprintf(2, "Usage: find path... files...\n");
        exit(1);
    }
    find(argv[1], argv[2]);
    exit(0);
}


