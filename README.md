AlHil Shell

by Hilary Zen and Alma Thaler

Features:

	* Can process any amount of whitespace (!!) and operators such as ;;
 	* simple commands without operators (cmd arg1 arg2 etc)
  	* commands of any form seperated by colon (cmd1<file;cmd2 args ; cmd3)
	* redirection (<, > and >>)
  	* piping (can pipe "cmd1 arg1 | cmd2" and other variants, does not just have to be argument-less! 
Attempted:

	* Intended to short the user/user/etc/etc@, didn't get to it

Bugs:

  	* did not find any bugs using input test

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
	Given the input string, creates a char buffer and strips all trailing and leadingwhitespace, then copies that into
	output char* 
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
	Returns: 0 on success
	Uses parse_input to split the input by its whitespace. Checks if the input is empty, exit, or cd, and runs special
	commands (exit or chdir). For other commands, the function forks and uses execvp to run the command.
	==============*/

	/* ======== int colon_(char *input); ========
	Inputs: char * input
	Returns: 0 on success
	Splits the input into its separate commands with parse_input. For each command, the function copies it into a char
	array and sends it to execute_type to handle it.
	==============*/

	/* ======== int greater_than(char *input); ========
	Inputs: char * input
	Returns: 0 on success
	Uses parse_input to split the input into the command and the file. After running strip_whitespace and opening the file
	(if the file already exists, we overwrite it), we use dup and dup2 to change stdout to our new file descriptor. After
	running the command with execute_type, we switch process 1 back to stdout.
	==============*/

	/* ======== int double_greater_than(char *input); ========
	Inputs: char * input
	Returns: 0 on success
	Follows the same steps as greater_than. However, the function appends to files instead of overwriting, and doesn't
	create a new file if one already exists.
	==============*/

	/* ======== int less_than(char *input); ========
	Inputs: char * input
	Returns: 0 on success
	Splits the input into the command and the filename, then opens the file with O_RDONLY. We then use dup and dup2 to
	switch stdin with the descriptor of the file we just opened. After using execute_type to run the command, we move
	stdin back to 0.
	==============*/

	/* ======== int pipe_(char *input); ========
	Inputs: char * input
	Returns: 0 on success
	The function uses parse_input to get the two commands, and removes the whitespace. (The commands cannot have
	arguments, because strip_whitespace removes the spaces in between them.) We then create an unnamed pipe, and use dup
	and dup2 to switch stdout with the pipe's write end. After executing command 1, which sends its output to the pipe, we
	move stdout back and close the write end. We then change stdin to the read end of the pipe and execute the second
	command, which takes its input from the pipe and prints the result to the terminal.
	==============*/

	/* ======== static void sighandler(int signo); ========
	Inputs: char * input
	Returns: Integer value determined by type of input
	One big switch statement. Colon takes precedence (because colon just splits into two calls of execute_type after you
	seperate based on ;), then checks for the other operators (<, >, >>, |) and finally checks for ' '. Uses strcmp.
	Default is return 0 (no input, ask for something again)
	==============*/


main.c:

	Just has main
	
	/* ======== int main(); ========
	Inputs:
	Returns: 0 on success
	while (1){
		execute();
	}
	==============*/
