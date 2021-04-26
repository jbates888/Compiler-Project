#include "SymTab.h"
#include "stdlib.h"
#include "string.h"
#include "stdio.h"

int isPrime(int num);

/*
 *Creat a new table with a estimated size
 */
SymTab * createSymTab(int size){
  SymTab* table = (SymTab*)malloc(sizeof(SymTab));

  //round the size up to the next prime, leave aloine if the size is 2 or 3
  if(size == 1){
    size = 2;
  } else if(size != 2 || size != 3){
    while(isPrime(size) != 1){
      size++;
    }
  }


  //set the size of the table (need to round up to prime for performance)
  table->size = size;

  //create space for the array of size size
  SymEntry** entryArray;
  entryArray =(SymEntry**) malloc(size * sizeof(SymEntry*));
  int i;
  for(i = 0; i < size; i++){
    entryArray[i] = NULL;
  }
  table->contents = entryArray;
  table->current = NULL;

  //return the pointer to the new table
  return table;
}

/*
 * Free all the space used
 */
void destroySymTab(SymTab * table){
  int i;
  //loop though each index in the table array
  for(i = 0; i < table->size; i++){
    //if the current list is not null, go down it
    if(table->contents[i] != NULL){
      SymEntry * cur;
      //iterate over current list
      while(table->contents[i] != NULL){
	cur = table->contents[i];
	table->contents[i] = table->contents[i]->next;
	cur->name = NULL;
	free(cur->name);
	free(cur);
      }
    }
    table->contents[i] = NULL;
  }
  free(table->contents);
  table = NULL;
  free(table);
}


//insert new item in the table, pass in the table and name to enter
int enterName(SymTab * table, char * name){
  if(findName(table, name) == 0) {
    //Case 1: name not in table, make a copy of the name with malloc, add to table
    SymEntry* entry = (SymEntry*) malloc(sizeof(SymEntry));

    //calculate hash size
    int hashNum;
    int val = 0;
    int i;
    int len = strlen(name);
    for (i = 0; i < len; i++){
      val += name[i];
    }
    hashNum = val % table->size;


    //make a copy of the name and add it to the entry
    entry->name = strdup(name);

    //set current to the new entry
    table->current = entry;

    //set the new entries next to the current indexes current next
    entry->next = table->contents[hashNum];
    //set the current hash index to the new entry
    table->contents[hashNum] = entry;

    return 1;
  } else{
    //case 2: name already in the table, the call to findname() already set the current to the found entry
    return 0;
  }
}

/*
 *find the name in the table
 */
int findName(SymTab *table, char *name){
  //recalculaed hash function to get the correct list
  int hashNum;
  int val = 0;
  int i;
  int len = strlen(name);
  for (i = 0; i < len; i++){
    val += name[i];
  }
  hashNum = val % table->size;

  //make a dummy entry to iterate with
  SymEntry* start = table->contents[hashNum];
  //loop throguh the list
  while(start != NULL){
    if(strcmp(name, start->name) == 0){
      table->current = start;
      return 1;
    } else{
      start = start->next;
    }
  }
  //if here the name was not in the table
  return 0;
}

/*
 *Check if there is a current
 */
int hasCurrent(SymTab *table){
  //if current is null there is no current
  if(table->current != NULL){
    return 1;
  } else{
    //there is a current
    return 0;
  }
}

/*
 *set the attribute of the current entry
 */
void setCurrentAttr(SymTab *table, void * attr){
  table->current->attribute = attr;
}

/*
 * Return the value of the current entries attribute
 */
void * getCurrentAttr(SymTab *table){
  return table->current->attribute;
}

/*
 * return the name of the current entry
 */
char * getCurrentName(SymTab *table){
  return table->current->name;

}

/*
 *start the iterator at the first entry
 */
int startIterator(SymTab *table){
  //if there is no current the table is empty
  if(table->current == NULL){
    return 0;
  } else {
    //loop until a non empty list is found
    int i;
    for(i = 0; i < table->size; i++){
      if(table->contents[i] != NULL){
	//set current to the first item in the list and return
	table->current = table->contents[i];
	return 1;
      }
    }
    //should never get here
    return 0;
  }

}

/*
 * Advance the iterator one entry
 */
int nextEntry(SymTab *table){
  //if there is an empty table do nothing
  if(table->current == NULL){
    return 0;
  }
  //if the current list has another entry, move current to it
  if(table->current->next != NULL){
    table->current = table->current->next;
    return 1;
  }

  //get the curent index in contents
  int hashNum;
  int val = 0;
  int i;
  int len = strlen(table->current->name);
  for (i = 0; i < len; i++){
    val += table->current->name[i];
  }
  hashNum = val % table->size;
  //add one to the hash num to start iterating at the next list
  hashNum += 1;

  //loop util a non empty list is found
  while(hashNum < table->size){
    if(table->contents[hashNum] != NULL){
      //set current to the first item in the list
      table->current = table->contents[hashNum];
      return 1;
    }
    hashNum++;
  }
  //set current to the first item in the non empty list
  return 0;
}


/*
 * Hlper funtion to find a prime number
 */
int isPrime(int num) {
  int i;
  for (i = 2; i < num; i++) {
    if (num % i == 0 && i != num)
      return 0;
  }
  return 1;
}

