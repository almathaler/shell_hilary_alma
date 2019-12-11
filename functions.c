#include "functions.h"
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>
#include <fcntl.h>

int execute_type(char *input){
  int type = type_arg(input);
  printf("got type of \"%s\": %d\n", input, type);
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
    default:
      printf("Command format cannot be executed. Quitting...\n");
      exit(1);
  }
  return 0;
}

char ** parse_input(char *input, char *delimiter){
  int size = 10;
  char **to_return = malloc(10 * sizeof(char *));
  char checker[256];
  strcpy(checker, input);
  char *copy_input = input;
  int i = 0;
  while(checker != NULL && *checker != '\0'){
    //check for leading whitespace
    int index_checker = 0;
    while(checker[index_checker] != '\0' &&
         (checker[index_checker] == '\n' || checker[index_checker] == '\t' || checker[index_checker] == ' ')){
      index_checker++;
    }
    int not_null = 0;
    while(checker[not_null + index_checker]!='\0'){
      checker[not_null] = checker[index_checker + not_null];
      not_null++;
    }
    checker[not_null] = '\0';
    //trailing whitespace
    index_checker = 0;
    int other_index = -1;
    while(checker[index_checker] != '\0'){
       if(checker[index_checker] != ' ' && checker[index_checker] != '\t' && checker[index_checker] != '\n'){
           other_index = index_checker;
       }
       index_checker++;
    }
    checker[other_index + 1] = '\0';
    //another buffer so checker isn't bothered (u can't modify checker if to_return points to something in it)
    char checker_buffer[256];
    index_checker = 0;
    while(checker[index_checker] != '\0'){
      checker_buffer[index_checker] = checker[index_checker];
      index_checker++;
    }
    checker_buffer[index_checker] = '\0';
    copy_input = checker_buffer;
    to_return[i] = strsep(&copy_input, delimiter);
    char buffer[256] = "\0";
    if (copy_input!= NULL && *copy_input != '\0'){
      strcpy(buffer, copy_input);
    }
    int k = 0;
    while(buffer[k]!='\0'){
      checker[k] = buffer[k];
      k++;
    }
    checker[k] = '\0';
    //
    //printf("checker: \"%s\", %p\tinput: \"%s\", %p\n", checker, &checker, input, &input);
    //printf("to_return[%d]: \"%s\"\tchecker: \"%s\"\n", i, to_return[i], checker);
    i++;
    if (i >= 2){
      size = size * 2;
      to_return = realloc(to_return, size * sizeof(char *));
    }
  }
  to_return[i] = NULL;
  int goingthrureturn = 0;
  while (to_return[goingthrureturn] != NULL){
    printf("to_return[%d]: \"%s\"\n", goingthrureturn, to_return[goingthrureturn]);
    goingthrureturn++;
  }
  printf("at end of parse_input\n");
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
  //printf("(SS)input:\"%s\"\n", input);

  char **input_args = parse_input(input, " \t\n");
  printf("Now in single space\n");
  printf("input_args[0]: \'%s\'\n", input_args[0]);

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
  while(input_args[i] != NULL){
    char copy[256];
    strcpy(copy, input_args[i]);
    execute_type(copy);
    i++;
  }
  return 0;
}

int greater_than(char *input) {
  printf("in greater than\n");
  char **input_args = parse_input(input, ">\n");
  printf("parsed\n");
  int i = 0;
  //to check that there are no extra spaces in between
  while(input_args[i] != NULL){
    char copy[256];
    strcpy(copy, input_args[i]);
    if (strchr(copy, ' ') != strrchr(copy, ' ')){
      printf("please format your redirected input as \"cmd1>cmd2\"\n");
      return 0;
    }
    printf("input_args[%d]:\"%s\"\n", i, input_args[i]);
    i++;
  }
  //Open file
  int check = open(input_args[1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
  printf("check: %d\n", check);
  if (check == -1) {
    printf("opening your file failed, strerror: %s\n", strerror(errno));
  }

  int backup = dup(1); //Duplicates stdout
  dup2(check, 1); //Turns stdout into this current process
  char file_name[256];
  strcpy(file_name, input_args[0]);

  //strcat(file_name, " ");

  single_space(file_name);
  dup2(backup, 1);
  return 0;
}
