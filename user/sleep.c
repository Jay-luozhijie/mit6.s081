#include "user/user.h"

int
main(int argc, char *argv[])
{
  int i;

  if(argc != 2){
    fprintf(2, "error, sleep command can only have 2 args");
    exit(1);
  }

  int numOfTick = atoi(argv[1]);

  sleep(numOfTick);

  exit(0);
}