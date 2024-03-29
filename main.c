#include "functions.h"
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>


static void sighandler(int signo){
  if (signo == SIGSEGV){
    printf("seg fault!\n");
    exit(0);
  }
}

int main(){
  signal(SIGSEGV, sighandler);
  while(1){
    //print_prompt();
    execute();
  }
  return 0;
}
