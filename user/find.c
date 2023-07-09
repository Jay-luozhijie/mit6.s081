#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

void find(char *dir, char *filename){
    int fd;
    struct stat st;
    struct dirent de;
    char buf[512] ,*p;
    if ((fd = open(dir, 0)) < 0){
        fprintf(2, "find: dir path cannot open %s\n", dir);
        return;
    }
    if(fstat(fd, &st) < 0 ){
        fprintf(2, "find: cannot open stat %s\n", dir);
        close(fd);
        return;
    }
    if (st.type != T_DIR){
        fprintf(2, "find: path not a directory %s\n", dir);
        close(fd);
        return;
    }


    strcpy(buf, dir);
    p = buf + strlen(buf);
    *p++ = '/';
    while (read(fd, &de, sizeof(de)) == sizeof(de)){
        if (de.inum == 0 || strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0) continue;
        memmove(p, de.name, DIRSIZ);
        p[DIRSIZ] = 0;
        if (strcmp (de.name, filename) == 0){
            printf("find the file: %s/%s\n", dir, filename);
        }
        
        if (stat(buf, &st) < 0){
            printf("find: cannot stat %s\n", buf);
            continue;
        }

        
        if (st.type == T_DIR){
            char newBuf[512];
            char fn[sizeof(filename)];
            strcpy(fn, filename);
            strcpy(newBuf, buf);
            printf("enter dir %s, try to find the file %s\n", newBuf, fn);

            find(newBuf, fn);
            
        }

        // printf("newBuf: %s\n", newBuf);
        // if (st.type == T_DIR){
        //     find(newBuf, fn);
        // }
    }
    close(fd);
}



void main(int argc, char *argv[]){
    if(argc != 3){
        fprintf(2, "error, can't recognize the command\n");
    } else {
        find(argv[1], argv[2]);
    }

    exit(0);
}