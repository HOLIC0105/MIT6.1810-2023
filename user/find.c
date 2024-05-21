#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"

int match(const char*, const char*);

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
            if(match(filename, de.name)) {
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
        fprintf(2, "Usage: find path files\n");
        exit(1);
    }
    find(argv[1], argv[2]);
    exit(0);
}


int matchhere(const char*, const char*);
int matchstar(int, const char*, const char*);

int
match(const char *re, const char *text)
{
  if(re[0] == '^')
    return matchhere(re+1, text);
  do{  // must look at empty string
    if(matchhere(re, text))
      return 1;
  }while(*text++ != '\0');
  return 0;
}

// matchhere: search for re at beginning of text
int matchhere(const char *re, const char *text)
{
  if(re[0] == '\0')
    return 1;
  if(re[1] == '*')
    return matchstar(re[0], re+2, text);
  if(re[0] == '$' && re[1] == '\0')
    return *text == '\0';
  if(*text!='\0' && (re[0]=='.' || re[0]==*text))
    return matchhere(re+1, text+1);
  return 0;
}

// matchstar: search for c*re at beginning of text
int matchstar(int c, const char *re, const char *text)
{
  do{  // a * matches zero or more instances
    if(matchhere(re, text))
      return 1;
  }while(*text!='\0' && (*text++==c || c=='.'));
  return 0;
}


