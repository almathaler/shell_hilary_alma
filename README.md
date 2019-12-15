AlHil Shell
by Hilary Zen and Alma Thaler

Features:
	* Can process any amount of whitespace (!!) and operators such as ;;
 	* simple commands without operators (cmd arg1 arg2 etc)
  	* commands of any form seperated by colon (cmd1<file;cmd2 args ; cmd3)
	* redirection (<, > and >>)
  	* piping (only one at a time though please)
Attempted:
	* Intended to short the user/user/etc/etc@, didn't get to it
	* Intended to fix pipe bug described below

Bugs:
  	* can only pipe cmd1 | cmd2, no args please
	
Files & Function Headers:
functions.c
	Just does everything
	
	/* ======== char ** parse_input() ========
	Inputs: char *input, char *delimiter
	Returns: Pointer to array of inputs seperated by given delimiter
	==============*/

	/* ======== int strip_whitespace() ========
	Inputs: char *input, char *output
	Returns: 0 upon success
	Given the input string, creates a char buffer and strips all whitespace, then copies that into output char* (should be
	run after parsing input, parses all
	so what's inbetween words too. this is why we are having issues with pipe where the parsed inputs shouldn't have the
	whitespace removed. For colon for some reason it's fine to just parse and then call single_space on the args, because
	single_space calls this on it's individual args)
	==============*/
	
	/* ======== int execute() ========
	Inputs: 
	Returns: 0 upon success
	Very simple, just uses fgets to take in input and the calls execute_type. Is the main function that process returns to
	after other methods conclude (well in main.c, main has a while(1) loop that continuously calls execute)
	==============*/

	/* ======== void print_prompt() ========
	Inputs: 
	Returns: 
	Prints the CWD followed by @
	==============*/
	
	/* ======== int execute_type() ========
	Inputs: char *input
	Returns: 0 on success
	One big switch statement. Runs type_arg, then chooses which function to call based on the type.  
	==============*/
	
	/* ======== int type_arg() ========
	Inputs: char * input
	Returns: Integer value determined by type of input
	One big switch statement. Colon takes precedence (because colon just splits into two calls of execute_type after you
	seperate based on ;), then checks for the other operators (<, >, >>, |) and finally checks for ' '. Uses strcmp.
	Default is return 0 (no input, ask for something again) 
	==============*/

	/* ======== int single_space(char * input); ========
	Inputs: char * input
	Returns: Integer value determined by type of input
	One big switch statement. Colon takes precedence (because colon just splits into two calls of execute_type after you
	seperate based on ;), then checks for the other operators (<, >, >>, |) and finally checks for ' '. Uses strcmp.
	Default is return 0 (no input, ask for something again) 
	==============*/
	
	/* ======== int colon_(char *input);   ========
	Inputs: char * input
	Returns: Integer value determined by type of input
	One big switch statement. Colon takes precedence (because colon just splits into two calls of execute_type after you
	seperate based on ;), then checks for the other operators (<, >, >>, |) and finally checks for ' '. Uses strcmp.
	Default is return 0 (no input, ask for something again) 
	==============*/
	
	/* ======== int greater_than(char *input); ========
	Inputs: char * input
	Returns: Integer value determined by type of input
	One big switch statement. Colon takes precedence (because colon just splits into two calls of execute_type after you
	seperate based on ;), then checks for the other operators (<, >, >>, |) and finally checks for ' '. Uses strcmp.
	Default is return 0 (no input, ask for something again) 
	==============*/
	
	/* ======== int double_greater_than(char *input); ========
	Inputs: char * input
	Returns: Integer value determined by type of input
	One big switch statement. Colon takes precedence (because colon just splits into two calls of execute_type after you
	seperate based on ;), then checks for the other operators (<, >, >>, |) and finally checks for ' '. Uses strcmp.
	Default is return 0 (no input, ask for something again) 
	==============*/
	
	/* ======== int less_than(char *input); ========
	Inputs: char * input
	Returns: Integer value determined by type of input
	One big switch statement. Colon takes precedence (because colon just splits into two calls of execute_type after you
	seperate based on ;), then checks for the other operators (<, >, >>, |) and finally checks for ' '. Uses strcmp.
	Default is return 0 (no input, ask for something again) 
	==============*/
	
	/* ======== int pipe_(char *input); ========
	Inputs: char * input
	Returns: Integer value determined by type of input
	One big switch statement. Colon takes precedence (because colon just splits into two calls of execute_type after you
	seperate based on ;), then checks for the other operators (<, >, >>, |) and finally checks for ' '. Uses strcmp.
	Default is return 0 (no input, ask for something again) 
	==============*/
	
	/* ======== static void sighandler(int signo); ========
	Inputs: char * input
	Returns: Integer value determined by type of input
	One big switch statement. Colon takes precedence (because colon just splits into two calls of execute_type after you
	seperate based on ;), then checks for the other operators (<, >, >>, |) and finally checks for ' '. Uses strcmp.
	Default is return 0 (no input, ask for something again) 
	==============*/


main.c
	Just has main
	/* ======== int main(); ========
	Inputs: 
	Returns: 0 on success
	while (1){
		execute();
	}
	==============*/
	

