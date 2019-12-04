#ifndef functions_h
#define functions_h
//REMEMBER 0 IS FALSE AND 1 IS TRUE
/*
the main execute function, does everything from prompt to fgets to type_arg, then
actually doing the command (fork with wait at the end or something else for cd or exit)
. will be in a loop in main that is while(0).
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
1 - single space
2 - there's a colon ("ls -l; cd dir")
3 - the > argument
4 - the < argument
5 - the |
6 - cd
7 - exit
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
int colon_(char *input);
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
int change_directory(char *input);
/*
*/
int exit_this(char *input);
#endif
