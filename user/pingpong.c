#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#define READEND 0
#define WRITEEND 1

void main(int argc, char *argv[]){
    int p1[2];
    int p2[2];
    pipe(p1);
    pipe(p2);
    if(fork() == 0){ //this is child
        char buf[4];
        // close(p1[WRITEEND]);
        
        read(p1[READEND], buf, 1);
        fprintf(1, "%d: received ping\n", getpid());
        write(p2[WRITEEND], buf, 1);
        // close(p1[READEND]);
        // close(p2[WRITEEND]);
        // close(p2[READEND]);
        exit(0);
    } else {
        char c = 'a';
        // close(p1[READEND]);
        // close(p2[WRITEEND]);
        write(p1[WRITEEND], &c, 1);
        read(p2[READEND], &c, 1);
        fprintf(1, "%d: received pong\n", getpid());
        // close(p1[WRITEEND]);
        // close(p2[READEND]);
        exit(0);
    }
    
}