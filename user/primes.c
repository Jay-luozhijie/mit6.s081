#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define END 35
#define WRITEEND 1
#define READEND 0

void child(int[2]);

void main(int argc, char *argv[]){
    int p[2];
    pipe(p);
    if (fork() == 0){
        child(p);
    } else { 
        close(p[READEND]);
        for (int i = 2; i <= END; i++){
            write(p[WRITEEND], &i, sizeof(int));
        }
        close(p[WRITEEND]);
        wait((int*) 0);
    }
    exit(0);
}

void child(int pLeft[2]){
    int pRight[2];
    int number;

    close(pLeft[WRITEEND]);
    if(read(pLeft[READEND], &number, sizeof(int)) == 0){
        exit(0);
    }

    pipe(pRight);
    if (fork() == 0) {
        child(pRight);
    } else {
        printf("prime %d\n", number);
        int prime = number;
        close(pRight[READEND]);
        while(read(pLeft[READEND], &number, sizeof(int)) != 0){
            if (number % prime != 0){
                write(pRight[WRITEEND], &number, sizeof(int));
            }
        }
        close(pLeft[READEND]);
        close(pRight[WRITEEND]);
        wait((int*) 0);
    }
    exit(0);
}