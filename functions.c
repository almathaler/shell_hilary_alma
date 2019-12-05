#include "functions.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>


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

  //first parse
  // then check if input is exit or cd
  //then fork, do execvp in child and wait in parent

  int size = 1;
  char *input_args[20];//should we be mallocing more space so it's dynamic? I don't think more than 20 args will ever be inputted but we can change this later
  while (*input!='\0'){
    input_args[size-1] = strsep(&input, " \t\n");
    size++;
  }
  input_args[size-1] = NULL;

  //for exit and cd
  if (strcmp("exit", input_args[0]) == 0){
    exit(0); // exit the program
  }
  if (strcmp("cd", input_args[0])==0){
    //do this later
    printf("%s\n", input_args[1]);
    //!!!!!!
    chdir(input_args[1]); //DO ERROR CATCHING HERE
    //!!!!!
  }else{ //shouldn't try to do anything after the cd
    //fork, do execvp
    int f = fork(); //create child branch
    if (f){ //just wait
      int status; //for wait and error checking
      waitpid(f, &status, 0); //if options is 0, will work normally
      if (errno != 0){
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
  //ok so parse input based on ;, end up with array of single space inputs and last entry should be NULL
  //then, make iterator int and have loop that is while (arg[i] != NULL).
  //inside that loop, do single_space for each value inside that array

  //parse
  int size = 1;
  char *input_args[20];//should we be mallocing more space so it's dynamic? I don't think more than 20 args will ever be inputted but we can change this later
  printf("in colon_\n");
  while (input!='\0'){
    printf("input_args[%d]:\n", (size-1));
    input_args[size-1] = strsep(&input, ";"); //note be wary, might be that "ls -l ; cd ../" means empty char will be created?
    printf("%s\n", input_args[size-1]);
    printf("how input looks:\n");
    printf("%s\n", input);
    size++;
  }
  printf("trying to catch seg fault\n");
  input_args[size-1] = NULL;
  printf("size: %d\tinput_args[%d]: %s\n", size, (size-1), input_args[size-1]);
  //
  int i = 0;
  while(input_args[i] != NULL){
    printf("should be single_spacing: %s\n", input_args[i]);
    single_space(input_args[i]);
    i++;
  }
  return 0;
}
