/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/tools/sources/snns2clib.h,v $
  SHORTNAME      : snns2clib.h
  SNNS VERSION   : 4.2

  PURPOSE        : Headerfile for including the snns2c-library

  AUTHOR         : Berward Kett
  DATE           : 30.01.95

  CHANGED BY     : Michael Vogt
  RCS VERSION    : $Revision: 1.5 $
  LAST CHANGE    : $Date: 1998/02/25 15:34:50 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/

/* Recordtype for Lists (Sets) */

typedef struct {
  int place;			/* No of Elements wich can be hold in the list */
  int no;			/* No of actual Elements in the list           */
  int *values;			/* Pointer to the Elements                     */
} tList, *pList;

#define NoOf(list) list->no
#define element(list, no) list->values[no]

extern pList newList(void);                      /* creates a new set */
extern void  killList(pList list);               /* deletes a set     */
extern int   copyList(pList dest, pList source); /* copies a set      */
extern int   searchList(pList list, int member); /* searches for a member */
extern int   isMember(pList list, int member);   /* checks if member is element of list */
extern int   addList(pList list, int member);    /* adds a new element */
extern void  remList(pList list, int member);    /* removes an element */
extern void  intersectList(pList dest, pList source); /* calculates the intersection */
extern int   haveIntersection(pList list1, pList list2); /* does intersection exists */
extern int   mergeList(pList dest, pList source); /* calculates the union of two sets */
extern int   CompareSources( pList dest, pList source); /* compares two elements, wether their sources are the same */
