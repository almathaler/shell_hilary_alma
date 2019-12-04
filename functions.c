#include "functions.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>

void print_prompt(){
  char *cwd = NULL;
  cwd = getcwd(cwd, 256);
  //for future reference for what you print, strsep by '/' and the print last token and at the
  //first do ~/ I guess
  if (errno){
    printf("error in print_prompt!\terrno:%d\tstrerror:%s\n", errno, strerror(errno));
  }
  printf("%s@\n", cwd);
}

int execute(){
  print_prompt();
  char input[256];
  char *error_catch = fgets(input, 256, stdin);
  if (error_catch == '\0'){
    printf("error in execute's fgets!\n");
  }
  printf("%s", input);
  //int type = type_arg(input);
  return 0;
}
