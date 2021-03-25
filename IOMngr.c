#include "IOMngr.h"
#include "string.h"

char line[MAXLINE + 1];
bool hasListFile = false;
FILE * srcFilePointer;
FILE * listFilePointer;
int curLine = 0;
int curCol = 0;
int lastLineCalled = 0;

int calcOffset(int curLine);
/*
 *passed in names of files, listing name can be null. Open src file and listing file if not null.
 *if listing file is null, output to stdout
 */
int openFiles(char * sourceName, char * listingName){
  //set the boolean to true if there is a list file passed in
  if(listingName != NULL){
    hasListFile = true;
  }

  //open the src file passed in
  srcFilePointer = fopen(sourceName, "r");

  //if there is a listing file, open it
  if(hasListFile == true){
    listFilePointer = fopen(listingName, "w+");
    //check if list file was opend sucessfuly
    if(listFilePointer == NULL){
      return 0;
    }
  }

  //check if src file was opended sucesfully
  if(srcFilePointer == NULL){
    return 0;
  } else{
    return 1;
  }
}

/*
 *close the source file and listing file if there was one
 */
void closeFiles(){
  fclose(srcFilePointer);
  if(hasListFile == true){
    fclose(listFilePointer);
  }
}

/*
 * return the next source char. This function echos lines in the src
 * file to the listing file if it exsits. Lines in listng file should be numbered
 * return EOF when endof src file is reached
 */
char getNextSourceChar(){
  //if the end of the line was reached, reset current column to 0 again
  if(line[curCol + 1] == '\0'){
    curCol = 0;
  }

  //if curCol is 0, its either the first call or it was resest due to the end of the line being reached, read line into array with \
  fgets()
    if(curCol == 0){
      //advance curLine one
      curLine++;
      //clear the line array
      memset(line, 0, sizeof line);
      //read line into line array
      char* result = fgets(line, MAXLINE, srcFilePointer);

      //if fgets() returns null, we are at the end of the file so return EOF
      if(result == NULL){
	return EOF;
      }

      //if there is a listing file, echo line with line num to list file
      if(hasListFile == true){
	//convert curLine to string
	char str[12];
	sprintf(str, "%d", curLine);
	fprintf(listFilePointer,"%s", str);
	fprintf(listFilePointer, ": ");
	fputs(line, listFilePointer);
      }
    }
  //return the next char in the line/array and advance to next col
  return line[curCol++];
}


/*
 * write a line with a single '^' in the idedcated column
 * if no listing file, current line should be echoed to stdout the first time
 * for that line that writeIdecator/writeMessage is called.
 */
void writeIndicator(int column){
  int i;
  int offset = calcOffset(curLine);
  //if there is no listing file, print line to stdout
  if(hasListFile == false){
    //compare curLine to the line when this method was called last
    if(lastLineCalled != curLine){
      //if the line has not been called, set the lastCall to it and print
      lastLineCalled = curLine;
      printf("%d%s%s", curLine, ": ", line);
    }
    //loop to ofest the ^ to the correct position
    for(i = 0; i < column + offset; i++){
      printf(" ");
    }
    printf("^\n");
  } else{
    //loop to ofest the ^ to the correct position
    for(i = 0; i < column + offset; i++){
      fprintf(listFilePointer, " ");
    }
    fprintf(listFilePointer, "^\n");
  }
}

/*
 * Write the error message on a new line
 */
void writeMessage(char * message){
  //if there is no listing file, print line to stdout
  if(hasListFile == false){
    //compare curLine to the line when this method was called last
    if(lastLineCalled != curLine){
      //if the line has not been called, set the lastCall to it and print
      lastLineCalled = curLine;
      printf("%d%s%s", curLine, ": ", line);
    }
    printf("%s\n", message);
  } else{
    fprintf(listFilePointer,"%s\n", message);
  }
}

/*
 *Return the current line number
 */
int getCurrentLineNum(){
  return curLine;
}

/*
 *Return the current column number in the current line
 */
int getCurrentColumnNum(){
  return curCol;
}

/*
 *calculate the offset for write idecator based on the
 * number of digits in curline and the ":" symbol
 */
int calcOffset(int lineNum){
  int digits = 0;
  while(lineNum != 0){
    lineNum /= 10;
    ++digits;
  }
  return digits + 1;
}

