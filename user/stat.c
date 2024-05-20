#include "kernel/types.h"
#include "user/user.h"
#include "kernel/fcntl.h"
#include "kernel/stat.h"

int 
main(int argc, char const *argv[])
{
    if(argc != 2) {
        fprintf(2, "Usage: stat path\n");
        exit(1);
    }
    struct stat st;
    int fd = open(argv[1], O_RDONLY);
    if(fd == -1) {
        fprintf(2, "stat: cannot open %s\n", argv[1]);
        exit(-1);
    }
    if(stat(argv[1], &st) == -1) {
        fprintf(2, "stat: cannot stat %s\n", argv[1]);
        exit(-1);
    }
    //int dev;     // File system's disk device
    //uint ino;    // Inode number
    //short type;  // Type of file
    //short nlink; // Number of links to file
    //uint64 size; // Size of file in bytes
    fprintf(0, "File path:     %s\n", argv[1]);
    fprintf(0, "Disk device:   %d\n", st.dev);
    switch (st.type)
    {
    case 1:
        fprintf(0, "File type:     Directory\n" );
        break;
    case 2:
        fprintf(0, "File type:     File\n" );
        break;
    case 3:
        fprintf(0, "File type:     Device\n" );
        break;
    default:
        break;
    }
    fprintf(0, "Inode number:  %d\n", st.ino);
    fprintf(0, "Links number:  %d\n", st.nlink);
    fprintf(0, "File Size:     %l\n", st.size);
    exit(0); 
}
