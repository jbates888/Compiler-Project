#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

//the longest line the src file can have in it
#define MAXLINE 1024

/*
 *passed in names of files, listing name can be null. Open src file and listing file if not null.
 *if listing file is null, output to stdout
 */
int openFiles(char * sourceName, char * listingName);

/*
 *close the source fileand listing file if there was one
 */
void closeFiles();

/*
 * return the next source char. This function echos lines in the src
 * file to the listing file if it exsits. Lines in listng fiel should be numbered
 * return EOF when endof src file is reached
 */
char getNextSourceChar();

/*
 * write a line with a single '^' in the idedcated colum
 * if no listing file, current line should be echoed to stdout the first time
 * for that line that writeIdecator/writeMessage is called.
 */
void writeIndicator(int column);

/*
 * Write the error message on a new line
 */
void writeMessage(char * message);

/*
 *Return the current line number
 */
int getCurrentLineNum();

/*
 *Return the current column number in the current line
 */
int getCurrentColumnNum();



