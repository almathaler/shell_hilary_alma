#include "functions.h"
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>
#include <fcntl.h>

int main(){
  char* input1 = "abc";
  char* input2 = " abc";
  char* input3 = "abc ";
  char* input4 = "ab cd ef";
  char* input5 = " ab cd ef";
  char* input6 = "ab cd ef ";
  parse_input(input1, " \n\t");
  parse_input(input2, " \n\t");
  parse_input(input3, " \n\t");
  parse_input(input4, " \n\t");
  parse_input(input5, " \n\t");
  parse_input(input6, " \n\t");
  return 0;
}


char ** parse_input(char *input, char *delimiter){
  int size = 10;
  char **to_return = malloc(10 * sizeof(char *));
  char checker[256];
  strcpy(checker, input);
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
    index_checker = 0;
    int other_index = -1;
    while(checker[index_checker] != '\0'){
       if(checker[index_checker] != ' ' && checker[index_checker] != '\t' && checker[index_checker] != '\n'){
           other_index = index_checker;
       }
       index_checker++;
    }
    checker[other_index + 1] = '\0';
    char checker_buffer[256];
    index_checker = 0;
    while(checker[index_checker] != '\0'){
      checker_buffer[index_checker] = checker[index_checker];
      index_checker++;
    }
    checker_buffer[index_checker] = '\0';
    input = checker_buffer;
    to_return[i] = strsep(&input, delimiter);
    char buffer[256] = "\0";
    if (input!= NULL && *input != '\0'){
      strcpy(buffer, input);
    }
    int k = 0;
    while(buffer[k]!='\0'){
      checker[k] = buffer[k];
      k++;
    }
    checker[k] = '\0';
    //
    //printf("checker: \"%s\", %p\tinput: \"%s\", %p\n", checker, &checker, input, &input);
    printf("to_return[%d]: \"%s\"\tchecker: \"%s\"\n", i, to_return[i], checker);
    i++;
    if (i >= 2){
      size = size * 2;
      to_return = realloc(to_return, size * sizeof(char *));
    }
  }
  to_return[i] = NULL;
  printf("at end of parse_input\n");
  return to_return;
}
