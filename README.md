# MyShell
## Authors
- Mahmoud Hesham Almodalal 
- Mahmoud Farag Tayeh
- Hazem Mohammed Oukal
## Overview

MyShell is a basic command-line shell implemented in C. It supports execution of internal and external commands, input/output redirection, background processing, and batch mode via a script file.

## Features

- Command prompt showing current working directory
- Internal commands like `cd`, `clr`, `dir`, `environ`, `echo`, `help`, `pwd`,`pause`, `quit`
   executable external file can include these commands as well.
   Type "help" to get the detail usage of each command.
   e.g.  help
- Redirection support: `>`, `>>`, `<`

myshell supports i/o-redirection on either or both stdin(standard input) and/or stdout(standard output). i.e. the command line:
e.g.   programname arg1 arg2 < inputfile > outputfile
this will execute the program programname with arguments arg1 and arg2,
the stdin FILE stream replaced by inputfile and the stdout FILE stream replaced by outputfile.
The file can be given by the full path as:
/home/username/test.txt

Input redirection --> i.e. stdin redirection --> is possible for the internal commands: cd, dir, echo.
Output redirection --> i.e. stdout redirection --> is possible for the internal commands: pwd, dir, environ, echo, help.

=> when the redirection token is < then the inputfile is opened if it exists, or a "Path Error" wil be reported in the screen.
e.g.     cd  <  test.txt
=> when the redirection token is > then the outputfile is created if it does not exist and truncated if it does.
e.g.      ls  >  test.txt
=> when the redirection token is >> then outputfile is created if it does not exist and appended to if it does.
e.g.     environ  >>  test.txt
Sometimes you can use more than one token to open several files as input or/and  output.
e.g.   echo < m.txt > n.txt
dir  > m.txt > n.txt
If the file cannot opened due to read permission or write permission, an "Open Error" wil be reported in the screen.

- Background execution using `&`

Normally, when a command is execution, you have to wait for the completion and type another command. Especially, when the comand is executed from a batchfile, much time is wasted in waiting. Background execution allows you not to wait for the execution. An ampersand (&) at the end of the command line indicates that the shell should execute the command in background.
myshell supports background execution of programs. An ampersand (&) at the end of the command line indicates that the shell should return to the command line prompt immediately after launching that program.

- Batch file support

myshell is able to take its command line input from a batchfile.
i.e. if the shell is invoked with a command line argument:
e.g.   myshell test.bat
then the batchfile [test.bat] is assumed to contain a set of command lines for the shell to process.
When the end of file is reached, the shell should exit.
Obviously, if the shell is invoked without a command line argument,
it solicits input from the user via a prompt on the display.

## Internal Command Descriptions

### `cd [dir]`

Format:    cd [directory]
=> change the current default directory to [directory].
e.g.    cd /home
This command also change the PWD environment variable (use the command "pwd" to see it).
If the [directory] argument is not present, report the current directory.
If the directory does not exist, a "Path Error" wil be reported in the screen.
Moreover, you can use a directory path writen in a file as:
e.g.    cd < test.txt
The file can be given by the full path as:
cd < /home/username/test.txt

### `clr`

Format:    clr
or         clear
-> clear the screen, no arguments is needed.

### `dir [dir]`

Format:    dir [directory]
-> list the contents of directory [directory]
e.g.   dir  /home
This command  is different from "cd", it change neither the current default directory nor the PWD environment variable(use the command "pwd" to see it).
As to the directory path, type "help path" to get more information.
If the [directory] argument is not present, list the contents of the current directory. If the directory does not exist, a "Path Error" wil be reported in the screen.

Moreover, you can use a directory path written in a file as:
e.g.   dir < a.txt
And you can list the contents into a file or more than one file as:
e.g.   dir > b.txt      or    dir > b.txt > c.txt
The token ">" can be replaced by ">>", type "help" to see the difference of ">" and ">>".
You can use both input redirection and output redirection as:
e.g.   dir < a.txt > b.txt > c.txt
The file can be given by the full path as:
dir < /home/username/a.txt

### `environ`

Format:    environ
-> list all the environment strings in screen or into one file or more than one file as:
e.g.   envieron     or     environ > b.txt     or    environ > b.txt > c.txt
The token ">" can be replaced by ">>", type "help" to see the difference of ">" and ">>".
The file can be given by the full path as:
/home/a.txt

### `echo [args...]`

Format:    echo [comment]
-> display [comment] on the display or output files followed by a new line. And multiple spaces/tabs will be reduced to a single space.
e.g.   echo hello    world
The words "hello world" will display in the screen.
[comment] can be multiple words either typed from keyboard or read from one or more input files.
e.g.   echo < a.txt   or   echo < a.txt < b.txt
And you can display or output the contents into one file or more than one file:
e.g.   echo hello world > c.txt  or  echo hello world > c.txt > d.txt
The token ">" can be replaced by ">>", type "help" to see the difference of ">" and ">>".
You can use both input redirection and output redirection:
e.g.   echo < a.txt < b.txt > c.txt > d.txt
The file can be given by the full path as:
echo < /home/a.txt  > /home/b.txt

### `help`

Format:    help
-> display the user manual. Type "help [command]" to get the detail usage of a command as:
e.g.    help dir	or	? dir
Type "help command" to see the internal commands.
And you can display  or output the help information into one file or more than one file as:
e.g.    help dir > c.txt     or    help dir > c.txt > d.txt
The token ">" can be replaced by ">>", type "help" to see the difference of ">" and ">>".
The file can be given by the full path as:
help dir > /home/a.txt

### `pwd`

Format:     pwd
-> show the PWD environment variable. If you want to list all the environment strings, use command "environ", type "help environ" to see.
You can display or output the execution results into one file or more than one file as:
e.g.     pwd test.bat > c.txt      or    pwd test.bat > c.txt > d.txt
The token ">" can be replaced by ">>", type "help" to see the difference of ">" and ">>".
The file can be given by the full path as:
e.g.     pwd > /home/a.txt

### `pause`

Format:     pause
-> display "Press Enter to continue..." and pause operation of the shell until the 'Enter' key is pressed (ignore any intervening non-'Enter' input).

### `quit`

Format:    quit    or   exit  
-> Exit myshell, no argument is needed.

## File: main()

Initializes the shell, sets the shell path environment variable, checks for batch mode, loops over command input, and dispatches commands to execution.

## Function: execute_command()

Handles the parsing and execution of both internal and external commands. Supports:

- Redirection (`>`, `>>`, `<`)
- Background execution (`&`)
- Setting the `parent` environment variable

## Function: internal_cd()

Handles changing the directory using `chdir()` and updates `PWD` environment variable.

## Function: internal_clr()

Clears the screen using escape codes `[2J[H`.

## Function: internal_dir()

Opens the specified directory (or current one), and prints its contents using `readdir()`.

## Function: internal_environ()

Iterates over `environ` to print all environment variables.

## Function: internal_echo()

Prints all arguments passed to it starting from the second one.

## Function: internal_help()

Uses `more` to display the `readme` file content. Forks a process to run `more` and redirects the help file as standard input.

## Function: internal_pause()

Prompts the user to press Enter and waits for it using `getchar()`.

## Function: internal_quit()

Simply calls `exit(EXIT_SUCCESS)` to terminate the shell.

## Function: show_pwd()

Prints the `PWD` environment variable to stdout.

## Dependencies

- `stdio.h`, `stdlib.h`, `string.h`, `unistd.h`
- `fcntl.h`, `sys/wait.h`, `sys/stat.h`, `dirent.h`

## Compilation

```bash
gcc -o myshell myshell.c
```

## Usage

```bash
./myshell             # Interactive mode
./myshell script.txt  # Batch mode using script file
```














