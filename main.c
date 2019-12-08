#include "functions.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>

static void sighandler(int signo){
  if (signo == SIGSEGV){
    printf("seg fault! have only put this after \"*input != \'null char\'\" \n");
    exit(0);
  }
}

int main(){
  signal(SIGSEGV, sighandler);
  while(1){
    execute();
  }
  return 0;
}
