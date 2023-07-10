#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/param.h"

#define MAX_LEN 100
#define READ_FD 0

// char* readTill(char *p, char seperator[4]){
//     char *pCopy = p;
//     char *word;
//     while (*pCopy != seperator){
//         *word++ = *pCopy++;
//     }
//     *word = 0;
//     return word;
// }

void main(int argc, char *argv[]){
    char paramv[MAXARG][MAX_LEN];
    int argCount = argc - 1;

    while (1) {
        memset(paramv, 0, MAXARG * MAX_LEN);
        int argIndex = 0;
        for (; argIndex < argCount; argIndex++){
            strcpy(paramv[argIndex], argv[argIndex + 1]);
        }
        
        char buf;
        int readResult;
        int cursor = 0;
        //if read end with some space, argIndex = total arg + 1, else argIndex = total arg, so we need total arg
        int totalArg = argCount;
        int flag = 1;//whether the previous location is ' '
        while ((readResult = read(READ_FD, &buf, 1)) != 0 && buf != '\n'){
            if (buf == ' '){
                //prev location is not space, so this is the end of one word
                if (flag == 0){
                    paramv[argIndex][cursor] = 0;
                    cursor = 0;
                    argIndex++;
                    flag = 1;
                }
            } else {
                //at the very beginning of word
                if (flag == 1){
                    totalArg++;
                }
                flag = 0;
                paramv[argIndex][cursor++] = buf;
            }
        }
        if (readResult == 0){
            break;
        }
        char *modifiedParamv[MAXARG];
        for(int i = 0; i < totalArg; i++){
            modifiedParamv[i] = paramv[i];
        }
        if (fork() == 0){
            exec(paramv[0], modifiedParamv);
        } else {
            wait((int*)0);
        }
        // read(0, buf, 512);
        // printf("this the the buf: %s\n", buf);
        // if (fork() == 0){
        //     char *newArgv[MAXARG];
        //     printf("argc: %d\n", argc);
        //     int i = 1;
        //     int count = 0;
        //     for (; i < argc; i++){
        //         newArgv[i - 1] = argv[i];
        //         count++;
        //     }
        //     // char *prevResult;
        //     // while ((prevResult = readTill(prevResult, ' ')) != 0){
        //     //     newArgv[i++] = prevResult;
        //     // }
        //     char *prevResult = buf;
        //     char* word;
        //     while ((word = readTill(prevResult, seperator)) != ){

        //     }
        //     newArgv[i - 1] = buf;
        //     for (i = 0; i <= count; i++){
        //         printf("arg%d: %s\n", i, newArgv[i]);
        //     }
            
        //     exec(argv[1], newArgv);
        // } else {
        //     wait((int*)0);
        // }
    } 
    exit(0);
}