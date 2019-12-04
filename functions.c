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
  printf("%s@", cwd);
}

int type_arg(char * input){
  //The functions strchr() and strrchr() return a pointer to the located character,
  //   or NULL if the character does not appear in the string.
  if(!(strchr(input, ';') == NULL)){
    return 1; //return this first so that if there's like ls|wc ; cd, won't accidentally go into | pile
  }else if(!(strchr(input, '>') == NULL)){
    return 2;
  }else if(!(strchr(input, '<') == NULL)){
    return 3;
  }else if(!(strchr(input, '|') == NULL)){
    return 4;
  }else{ //have single_space deal w/ exit and cd
    //so there's no operators, just single space line
    return 0;
  }
}

int execute(){
  //getting input
  print_prompt();
  char input[256];
  char *error_catch = fgets(input, 256, stdin);
  if (error_catch == '\0'){
    printf("error in execute's fgets!\n");
  }
  //doing what input says based on switch
  printf("this is input: %s", input);
  int type = type_arg(input);
  printf("this is the type of input: %d\n", type);
  return 0;
}
