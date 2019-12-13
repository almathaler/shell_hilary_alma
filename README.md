AlHil Shell
by Hilary Zen and Alma Thaler

Features:
	* Can process any amount of whitespace (!!) and operators such as ;;
  * simple commands without operators (cmd arg1 arg2 etc)
  * commands of any form seperated by colon (cmd1<file;cmd2 args ; cmd3)
  * redirection (<, > and >>)
  * piping (only one at a time though please)
Attempted:
	Intended to shorten cwd@, didn't get to it

Bugs:
  * can only pipe cmd1 | cmd2, no args please
	Redirecting to a file that does not exist occasionally does not work.

Files & Function Headers:
parse.c
	Handles all line parsing fucntions
	/*======== int count_tokens() ==========
	Inputs:  char *line
        	  char delim
	Returns: Number of tokens in line separated by delim

	Counts the number of times the character delim appears in the
	 string line
	The number of tokens is 1 more than the number of appearences
	of delim
	If delim does not appear, 1 is returned
	====================*/

	/*======== char ** parse_line() ==========
	Inputs:  char *line
	Returns: Array of strings where each entry is a token
	separated by delim

	If line contains multiple tokens separated by delim, this
	function will put each token into an array of strings
	====================*/

	/*======== char * trim() ==========
	Inputs:  char *line
	Returns: Pointer to the beginning of line

	Removes leading and trailing whitespace on the string line.
	Terminating '\0' is placed at a new location if necessary.
	====================*/

dwsh.c
	Handles the forking an executing of commands...

magical.c
	UNOCORNS!
