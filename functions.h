#ifndef functions_h
#define functions_h

//hopefully will help with colon
char ** parse_input(char *input, char *delimiter); //for parsing
char ** remove_whitespace(char **args);
//REMEMBER 0 IS FALSE AND 1 IS TRUE
/*
the main execute function, has big switch so after getting and typing input, directs
to certain function
outline:
printf("(cwd)/# ") <- this can be a function in itself
*char input;
fgets(*input, 256, stdin);
int type_arg;
type_arg = type_arg(input);
then a big switch based on return val of type_arg w all the 5 functions
** there should be a wait in all those functions right after fork (in that if statement thing)
don't have to say print_f("cwd") again bc it'll be looped over again
*/
int execute();
/*
probably because getting current working directory is kinda difficult like is it just print .?
*/
void print_prompt();
/*
so this will take in the input (checks to see what type of
argument you put in, then returns an int to represent it)
0 - single space
1 - there's a colon ("ls -l; cd dir")
2 - the > argument
3 - the < argument
4 - the |
5 - cd -- ACTUALLY NO SINGLE_SPACE WILL DEAL W THIS
6 - exit
12/3 CURRENTLY VERY BASIC ONLY ONE OPERATOR PLS UNLESS SEPERATED BY ;!
*/
int type_arg(char * input);

/*
1 - single space. returns 0 just to see if it works
*/
int single_space(char * input);
/*
2 - colon, should just do single_space twice
but question, how will we wait? or i guess what we'll do is have signle space function and then the other.
well actually I guess we do have to. wait no there is a wait() in each function. so what we could have
is like a double fork. fork once for the colon, and then do single_space which will make that child process
wait. then at the end of that, the original colon function will take over and do another single space. and then
since at the start of this function there was a wait() to the parent function, after the colon's kid is done, the colon's fork will be done
and we will go back to execute()
*/
int colon_(char *input); //REMEMBER THAT COLON STUFF (sorry for caps) might have other operators within them. so do type_args again w/ individual pieces
/*
*/
int greater_than(char *input);
/*
*/
int less_than(char *input);
/*
*/
int pipe_(char *input);
/*
*/
static void sighandler(int signo);
/*
*/
int execute_type(char *input);
/*


int change_directory(char *input);
int exit_this(char *input);

*/
#endif
