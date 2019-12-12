#include "functions.h"
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>
#include <ctype.h>
#include <fcntl.h>

int execute_type(char *input){
  //here choose which function to run based on input
  int type = type_arg(input);
  //printf("got type of \"%s\": %d\n", input, type);
  switch(type) {
    case 0:
      single_space(input);
      break;
    case 1:
      colon_(input);
      break;
    case 2:
      greater_than(input);
      break;
    case 3:
      //less_than();
      break;
    case 4:
      //pipe();
      break;
    case 5:
      double_greater_than(input);
      break;
    default:
      printf("Command format cannot be executed. Quitting...\n");
      exit(1);
  }
  return 0;
}

int strip_whitespace(char *input, char *output){
  //here remove the whitespace. ideal for < and > which deal with files
  if (input == NULL){
    return 0; //don't touch it
  }
  char *out = output, *in = input;
  while(*in != '\0'){
    if (!isspace(*in)){
      *out = *in;
      out++;
    }
    in++;
  }
  *out = '\0';
  //printf("input: \"%s\"\toutput: \"%s\"\n", input, output);
  return 0;
}

char ** parse_input(char *input, char *delimiter){
  //parses input based on delimiter, and if you have 2 delimiters in a row ("  "), remove the subsequentn addition of "" to
  //input_args

  //printf("in parse_input, input: \"%s\"\n", input);
  int size = 10;
  char **to_return = malloc(10 * sizeof(char *));
  char *checker = input;
  int i = 0;
  while(checker != NULL && *checker != '\0'){
    to_return[i] = strsep(&checker, delimiter);
    //printf("to_return[%d]: \"%s\"\tchecker: \"%s\"\n", i, to_return[i], checker);
    if (strcmp(to_return[i], "")){
      i++;
    }
    if (i >= 2){
      size = size * 2;
      to_return = realloc(to_return, size * sizeof(char *));
    }
  }
  to_return[i] = NULL;
  //printf("at end of parse_input\n");
  return to_return;
}

int execute(){
  //fgets then execute_type
  //getting input
  print_prompt();
  char input[256];
  char *error_catch = fgets(input, 256, stdin);
  if (*error_catch == '\0'){
    printf("error in execute's fgets!\n");
  }
  //doing what input says based on switch
  execute_type(input);
  return 0;
}

void print_prompt(){
  char *cwd = NULL;
  cwd = getcwd(cwd, 256);
  if (*cwd == '\0'){
    printf("error in print_prompt!\terrno:%d\tstrerror:%s\n", errno, strerror(errno));
  }
  printf("%s@", cwd);
}

int type_arg(char * input){
  //based on these numbers, 'execute_type' takes to a different funciton
  if(!(strchr(input, ';') == NULL)){
    return 1; //return this first so that if there's like ls|wc ; cd, won't accidentally go into | pile
  }else if(!(strchr(input, '>') == NULL)){ //return 2 or 5, yes not intuitive but added >> at end
    if (strchr(input, '>') == strrchr(input, '>')){
      return 2;
    }else{
      return 5; //this means there is >>, also can't do multiple redirects like a > b > c
    }
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
  char **input_args = parse_input(input, " \t\n");
  //first see if we are exiting or cding
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
  //just call single_space over and over
  char **input_args = parse_input(input, ";\n"); //this might give problems for "ls ; cd", bc if first char is delimiter, strsep makes it null

  int i = 0;
  while(input_args[i] != NULL){
    char copy[256];
    strcpy(copy, input_args[i]);
    execute_type(copy);
    i++;
  }
  return 0;
}

int greater_than(char *input) {
  //as of now, if '>>' then will just treat as '>'
  char **input_args = parse_input(input, ">\n");
  //should have 2 input_args, one that is command to be carried out and the other is file that output is written into
  //create filename char to hold filename after it's modified
  char *filename = malloc(strlen(input_args[1]) + 1);
  //do whitespace w input_args[1] and filename (must have given filename memory before hand)
  if(strip_whitespace(input_args[1], filename)){
    printf("uh oh, strip_whitespace failed...\n");
  }
  //open filename
  int check = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
  if (check == -1) {
    printf("opening your file failed, strerror: %s\n", strerror(errno));
  }
  //now make stdout's fd point to our specified file
  int backup = dup(1); //Duplicates stdout
  dup2(check, 1); //Turns stdout into this current process
  char command[256];
  strcpy(command, input_args[0]);
  //i don't remember why we made a char buffer, but yea carry out that command (stuff will be written into filename)
  single_space(command);
  //don't forget to switch back to normal!
  dup2(backup, 1);
  return 0;
}

int double_greater_than(char *input){
  //as of now, if '>>' then will just treat as '>'
  char **input_args = parse_input(input, ">\n");
  //should have 2 input_args, one that is command to be carried out and the other is file that output is written into
  //create filename char to hold filename after it's modified
  char *filename = malloc(strlen(input_args[1]) + 1);
  //do whitespace w input_args[1] and filename (must have given filename memory before hand)
  if(strip_whitespace(input_args[1], filename)){
    printf("uh oh, strip_whitespace failed...\n");
  }
  //open filename
  int check = open(filename, O_WRONLY | O_CREAT | O_EXCL | O_APPEND, 0644);
  //printf("opened: \"%s\"\tcheck: %d\n", filename, check);
  if (check == -1) {
    //printf("opening your file failed, strerror: %s\n", strerror(errno));
    //might mean file exists, try again without the O_CREAT and check again
    check = open(filename, O_WRONLY | O_APPEND);
    if (check == -1){
      printf("opening your file failed, strerror: %s\n", strerror(errno));
    }
  }
  //now make stdout's fd point to our specified file
  int backup = dup(1); //Duplicates stdout
  dup2(check, 1); //Turns stdout into this current process
  char command[256];
  strcpy(command, input_args[0]);
  //i don't remember why we made a char buffer, but yea carry out that command (stuff will be written into filename)
  single_space(command);
  //don't forget to switch back to normal!
  dup2(backup, 1);
  return 0;
}

int less_than(char *input) {
  //as of now, if '>>' then will just treat as '>'
  char **input_args = parse_input(input, "<\n");
  //should have 2 input_args, one that is command to be carried out and the other is file that output is written into
  //create filename char to hold filename after it's modified
  char *filename = malloc(strlen(input_args[1]) + 1);
  //do whitespace w input_args[1] and filename (must have given filename memory before hand)
  if(strip_whitespace(input_args[1], filename)){
    printf("uh oh, strip_whitespace failed...\n");
  }
  //open filename
  int check = open(filename, O_RDONLY, 0644);
  if (check == -1) {
    printf("opening your file failed, strerror: %s\n", strerror(errno));
  }
  //now make stdin's fd point to our specified file
  int backup = dup(0); //Duplicates stdout
  dup2(check, 0); //Turns stdout into this current process
  char command[256];
  strcpy(command, input_args[0]);
  single_space(command);
  //don't forget to switch back to normal!
  dup2(backup, 0);
  return 0;
}
