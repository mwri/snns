/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/tools/sources/snns2clib.c,v $
  SHORTNAME      : snns2clib.c
  SNNS VERSION   : 4.2

  PURPOSE        : a Set Type with all needed applications

  AUTHOR         : Berward Kett
  DATE           : 30.01.95

  CHANGED BY     : Michael Vogt
  RCS VERSION    : $Revision: 1.10 $
  LAST CHANGE    : $Date: 1998/03/04 10:22:46 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/
#include <config.h>

#include <stdlib.h>
#include <string.h>
#include "glob_typ.h"

#ifndef NULL
#define NULL (void *)0
#endif

#define LIST_BLOCK_SIZE 10

/* Status (Error) Codes : OK = 0 (NO Error), ERR = 1, ...  */
typedef enum { OK, ERR, CANT_ADD, CANT_LOAD, MEM_ERR,
		   WRONG_PARAM, WRONG_ACT_FUNC, CANT_OPEN,
		   ILLEGAL_CYCLES, NO_CPN} Status;

/* Recordtype for Lists (Sets) */
typedef struct {
  int place;			/* No of Elements wich can be hold in the list */
  int no;			/* No of actual Elements in the list           */
  int *values;			/* Pointer to the Elements                     */
} tList, *pList;

#define NoOf(list) list->no
#define element(list, no) list->values[no]

/******************************************************************************
  pList newList(void)
  -----------------------------------------------------------------------------
  makes a new, empty List
  <- Pointer to the new list or NULL if an error occurs
  ******************************************************************************/
pList newList(void)
{
  pList list;

  list = (pList)malloc(sizeof(tList) );
  if (NULL == list) {
    return (NULL);
  }
  list->values = (int *)malloc(LIST_BLOCK_SIZE * sizeof(int) );
  if (list->values == NULL) {
    free(list);
    return(NULL);
  }
  list->place = LIST_BLOCK_SIZE;
  list->no    = 0;
  return(list);
}


/*****************************************************************************
  void killList(pList list)
  ----------------------------------------------------------------------------
  kills a list and releases memory
  
  -> pointer to the list
  *****************************************************************************/
void killList(pList list)
{
  free(list->values);
  free(list);
}


/*****************************************************************************
  int copyList(pList dest, pList source)
  ----------------------------------------------------------------------------
  copies the sources list in the dest list
  ->  source : original List;
  <-> dest   : destination List;
  <- (func) MEM_ERR : error occurs, dest leaves unchanged
  OK      : no problems
  *****************************************************************************/
int copyList(pList dest, pList source)
{
  int *newValues;
 
  newValues = (int *)malloc(source->place * sizeof(int) );
  if (!newValues) return(MEM_ERR);
  memcpy(newValues, source->values, NoOf(source) * sizeof(int) );

  free(dest->values);

  dest->no     = source->no;
  dest->place  = source->place;
  dest->values = newValues;
  return(OK);
}


/******************************************************************************
  int searchList(pList list, int member)
  -----------------------------------------------------------------------------
  searches an item in a list and returns the closest position. 
  e.g. if it is in the list it returns the position of the item,
  otherwise the position where it should be inserted
  if you only want to check if the item is member of the list use
  isMember()
  
  -> list   : pointer to the list
  member : item, wich is searched in the list
  <- (func) position of the item if it exist
  ******************************************************************************/
int searchList(pList list, int member)
{
  int low, high, pos = 0;
  low = 0;
  high = list->no -1;

  if (list->no == 0) {		/* empty list                  */
    list->values[0] = ++member;	/* must not be equal to member */
    return(0);
  }

  while (high > low) {
    pos = (high + low) / 2;	/* binary search because the values are sorted */
    if (member > list->values[pos]) {
      low = pos + 1;		/* in this order, you get the place were    */
    } /* member should be inserted if not present */
    else if (member < list->values[pos]) {
      high = pos;
    }
    else {
      return(pos);
    }
  }
  return(high); 
}


/*****************************************************************************
  int isMember(pList list, int member)
  ----------------------------------------------------------------------------
  checks, if member is part of a list
  
  -> list   : pointer to the list
  member : member wich is to be removed
  <- TRUE if it is member and FALSE otherwise
  *****************************************************************************/  
bool isMember(pList list, int member)
{
  int pos;
  pos = searchList(list, member);
  return(list->values[pos] == member);
}


/*****************************************************************************
  int addList(pList list, int member)
  ----------------------------------------------------------------------------
  inserts a new member in a list and reserves new memory if needed
  
  -> list   : pointer to the list
  member : new member, wich must be added
  <- (func)   (OK)       no problems
  (CANT_ADD) not enough memory for another element
  *****************************************************************************/ 
int addList(pList list, int member)
{
  int *oldptr, pos, j;
  pos = searchList(list, member);
  if (element(list, pos) == member) return(OK); /* no double entries         */

  if (list->no == list->place) { /* e.g. more space is needed */
    oldptr = list->values;
    list->place += LIST_BLOCK_SIZE;
    list->values = (int *)realloc( list->values, list->place * sizeof (int) );
    if (list->values == NULL) {	/* no more space available   */
      list->place -= LIST_BLOCK_SIZE; /* restore old state         */
      list->values = oldptr;
      return(CANT_ADD);
    }
  }
  
  if (list->values[list->no - 1] < member) {
    pos = list->no;		/* so append at the end */
  }
  for (j = list->no; j > pos; j--) { /* making place for new entry */
    list->values[j] = list->values[j-1];    
  }
  list->values[pos] = member;
  (list->no)++;

  return (OK);			/* everything all right       */
}


/*****************************************************************************
  void remList(pList list, int member)
  ----------------------------------------------------------------------------
  removes an item from a list if the item exists in it. Otherwise nothing
  happens. Releases also unneeded memory.
  
  -> list   : pointer to the list
  member : member wich is to be removed
  *****************************************************************************/
void remList(pList list, int member)
{
  int i, pos;
  pos = searchList(list, member); /* search for (possible) position */
  if (member == list->values[pos]) { /* really a member ?              */
    for (i = pos; i < list->no - 1; i++) { /* shift all following members    */
      list->values[i] = list->values[i+1];
    }
    (list->no)--;		/* loosed one member */
  }
  
  if (list->no < list->place - LIST_BLOCK_SIZE) { /* release unneeded memory */
    list->place -= LIST_BLOCK_SIZE;
    list->values = (int *)realloc(list->values, list->place *sizeof(int) );
  }
}


/*****************************************************************************
  void intersectList(pList dest, pList source)
  ----------------------------------------------------------------------------
  puts the intersection of dest and sources into dest
  ->  sources : unchanged list
  <-> dest    : list wich contains the intersection after intersect list 
  *****************************************************************************/
void intersectList(pList dest, pList source)
{
  int i = 0, j;
  while (i < NoOf(dest) ) {
    if (!isMember(source, element(dest, i) ) ) {
      ( NoOf(dest) )--;
      for (j = i; j < NoOf(dest); j++) { /* kill element by overwrite */
        element(dest, j) = element(dest, j + 1);
      }	/* so the same place must be checked */
    }
    else {
      i++;			/* element found, so check next one */
    }
  }
}


/*****************************************************************************
  int haveIntersection(pList list1, pList list2)
  ----------------------------------------------------------------------------
  checks if list1 and list2 have an identical member
  -> list1, list2 : Lists wich will be checked
  <- (func)  TRUE  : they have
  FALSE : they haven't
*****************************************************************************/
bool haveIntersection(pList list1, pList list2)
{
  int i;
  for(i = 0; i < NoOf(list1); i++) {
    if (isMember(list2, element(list1, i) ) ) return (TRUE);
  }
  return (FALSE);
}


/*****************************************************************************
  int mergeList(pList destList, pList sourcesList)
  ----------------------------------------------------------------------------
  puts all members of sourceList in destList
  -> source     : unchanged List
  <-> destList  : List wich contains all Elements after 'mergeList'
  <-  (func)  (OK)       no problems
  (MEM_ERR)  not enough memory for another element
  *****************************************************************************/
int mergeList(pList dest, pList source)
{
  int   sc = 0, dc = 0, bc = 0; /* counters for source, dest, big */
  pList big;                    /* list for all Elements          */
  int   *old, place;            /* helppointer and needed memory  */

  big = newList();              /* create new List */
  if (!big) return(MEM_ERR);

  /** allocate (maximum)Memory for the new List **/
  place = NoOf(source) + NoOf(dest);
  /* align to LIST_BLOCK_SIZE */
  place = ( (place / LIST_BLOCK_SIZE) + 1) * LIST_BLOCK_SIZE;
  old = big->values;
  big->place  = place;
  big->values = (int *)realloc(big->values, place * sizeof (int) );
  if (big->values == NULL) {
    big->values = old;		/* for killing the whole list */
    killList(big);
    return(MEM_ERR);
  }

  while ( (sc < NoOf(source) ) && (dc < NoOf(dest) ) )
      {
	if ( element(source, sc) < element(dest, dc) ) {
	  element(big, bc++) = element(source, sc++);
	}
	else {
	  element(big, bc++) = element(dest, dc++);
	  /* if elements of dource and dest were equal */
	  if (element(source, sc) == element(big, bc -1) ) sc++;
	}

      }

  /* now only one list may have elements */
  while (sc < NoOf(source) ) {
    element(big, bc++) = element(source, sc++);
  }
  while (dc < NoOf(dest) ) {
    element(big, bc++) = element(dest, dc++);
  }

  big->no = bc;

  while (NoOf(big) + LIST_BLOCK_SIZE < big->place) {
    big->place -= LIST_BLOCK_SIZE;
    big->values = (int *)realloc(big->values, big->place * sizeof(int) );
  }
 
  /* move contens of big in dest */
  free(dest->values);
  dest->values = big->values;
  dest->no     = big->no;
  dest->place  = big->place;  
  free(big);

  return(OK);
}

/****************************************************************************
  int CompareSources( pList dest, pList source)
 ----------------------------------------------------------------------------
  compares two sources sets 1 = equal
*****************************************************************************/

int CompareSources( pList dest, pList source)
{
  int i = 0;
  while (i < NoOf(dest) ){
    if (!isMember(source, element(dest, i) ) )
      return 0;
    i++;
  }

  i = 0;
  while (i < NoOf(source) ){
    if (!isMember(dest, element(source, i) ) )
      return 0;
    i++;
  }
  
  return 1;
}
