# shell_hilary_alma
our shell.git project

     
   ### A description of what features your shell implements
    * command input of any whitespace (cmd1 ;cmd2, cmd1   ;  cmd2, etc)
    * simple single spaced commands (ls -a -l) [but can be ls   -a -l if you want]
    * semicoloned commands
    * > and >> redirection
   ### A description of what features you attempted to implement but were unsuccessful
    as of 12/11:
    * <
    * pipes
   ### Any bugs or things you want me to know about your program
        
   ### A copy of every function header
    char ** parse_input(char *input, char *delimiter);
    int strip_whitespace(char *input, char *output);
    int execute();
    void print_prompt();
    int type_arg(char * input);
    int single_space(char * input);
    int colon_(char *input); 
    int greater_than(char *input);
    int double_greater_than(char *input);
    int less_than(char *input);
    int pipe_(char *input);
    static void sighandler(int signo);
    int execute_type(char *input);
    
   
