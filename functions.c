#include "functions.h"
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>

char ** parse_input(char *input, char *delimiter){
  int size = 10;
  char **to_return = malloc(10 * sizeof(char *));
  char *checker = input;
  int i = 0;
  while(*checker != '\0'){
    to_return[i] = strsep(&checker, delimiter);
    i++;
    if (i >= 2){
      size = size * 2;
      to_return = realloc(to_return, size * sizeof(char *));
    }
  }
  to_return[i] = NULL;
  return to_return;
}

int execute(){
  //getting input
  print_prompt();
  char input[256];
  char *error_catch = fgets(input, 256, stdin);
  if (*error_catch == '\0'){
    printf("error in execute's fgets!\n");
  }
  //doing what input says based on switch
  int type = type_arg(input);
  switch(type) {
    case 0:
      single_space(input);
      break;
    case 1:
      colon_(input);
      break;
    case 2:
      //greater_than();
      break;
    case 3:
      //less_than();
      break;
    case 4:
      //pipe();
      break;
    default:
      printf("Command format cannot be executed. Quitting...\n");
      exit(1);
  }
  return 0;
}

void print_prompt(){
  char *cwd = NULL;
  //errno = 0;
  cwd = getcwd(cwd, 256);
  //for future reference for what you print, strsep by '/' a nd the print last token and at the
  //first do ~/ I guess
  //printf("*cwd: %s\n", cwd);
  if (*cwd == '\0'){
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

int single_space(char * input){
  printf("(SS)input:\"%s\"\n", input);

  char **input_args = parse_input(input, " \t\n");

  if (strcmp("exit", input_args[0]) == 0){
    exit(0); // exit the program
  }
  if (strcmp("cd", input_args[0])==0){
    int cd_check = chdir(input_args[1]); //DO ERROR CATCHING HERE
    if (cd_check == -1) {
      printf("cd failed, check if your directory exists!\n");
    }
  }else{
    int f = fork(); //create child branch
    if (f){ //just wait
      int status; //for wait and error checking
      //if options is 0, will work normally
      if (waitpid(f, &status, 0) == -1){
        printf("uh oh! child exited wonkily. Reaping...\n");
        int exited = WIFEXITED(status);
        int return_val_child = WEXITSTATUS(status);
        printf("exited normally (0 means no)? %d\treturn val: %d\n", exited, return_val_child);
      }
    }else{
      if (execvp(input_args[0], input_args) == -1){
        printf("Something wrong with execvp! errno:%d\tstrerror:%s\n", errno, strerror(errno));
      }
    }
  }
  return 0;
}

int colon_(char *input){
  char **input_args = parse_input(input, ";\n"); //this might give problems for "ls ; cd", bc if first char is delimiter, strsep makes it null

  int i = 0;
/*
  while (input_args[i] != NULL){
    printf("input_args[%d]: %s\n", i, input_args[i]);
    i++;
  }
*/
  while(input_args[i] != NULL){
    char *copy;
    strcpy(copy, input_args[i]);
    printf("should be single_spacing:%s\n", copy);
    single_space(copy);
    i++;
  }
  return 0;
}
