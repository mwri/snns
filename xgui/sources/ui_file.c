/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/xgui/sources/ui_file.c,v $
  SHORTNAME      : file.c
  SNNS VERSION   : 4.2

  PURPOSE        : popups a window with all filenames. The user may alter 
		   the names and may request a save or load operation with 
		   the correspondent kind of file (NET, GUI, PAT, CFG, TXT).
  NOTES          : is called only during initialisation

  AUTHOR         : Ralf Huebner
  DATE           : 06.04.1992

  CHANGED BY     : Michael Vogt, Guenter Mamier
  RCS VERSION    : $Revision: 2.13 $ 
  LAST CHANGE    : $Date: 1998/03/13 16:31:54 $  

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG
             
******************************************************************************/
#include <config.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>

#include "ui.h"

#ifdef ultrix
#ifdef HAVE_SYS_FILE_H
#include <sys/file.h>
#endif
#else
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif
#endif

#ifdef NeXT
#ifdef HAVE_SYS_FILE_H
#include <sys/file.h>
#endif
#include <sys/dir.h>
#define dirent direct
#endif

#include <X11/Shell.h>
#include <X11/Xaw/Form.h>
#include <X11/Xaw/Command.h>
#include <X11/Xaw/Box.h>
#include <X11/Xaw/AsciiText.h>
#include <X11/Xaw/Label.h>
#include <X11/Xaw/Cardinals.h>

#include "ui_fileP.h"
#include "ui_xWidgets.h"
#include "ui_confirmer.h"
#include "ui_main.h"
#include "ui_mainP.h"

#include "ui_file.ph"


#ifndef S_ISDIR
#define S_ISDIR(mode)   (((mode) & (_S_IFMT)) == (_S_IFDIR))
#endif

#ifdef NeXT

char *tempnam(dir,pfx)
char *dir, *pfx;
{
char *filename,*start,*t;
int n,count;

filename = (char*)malloc(MAXPATHLEN);
if((dir == NULL)&&(pfx == NULL))
    tmpnam(filename);
if((dir == NULL)&&(pfx != NULL))
    {
    t=tmpnam(NULL);
    start = rindex(t,'/');
    *start = '\0';
    start++;
    sprintf(filename,"%s/%s%s",t,pfx,start);
    }
if(dir != NULL)
    {
    t=tmpnam(NULL);
    start = rindex(t,'/');
    *start = '\0';
    start++;
    if(pfx != NULL)
        {
        if(dir[strlen(dir)-1] != '/')
            sprintf(filename,"%s/%s%s",dir,pfx,start);
        else
            sprintf(filename,"%s%s%s",dir,pfx,start);
        }
    else
        {
        if(dir[strlen(dir)-1] != '/')
            sprintf(filename,"%s/%s",dir,start);
        else
            sprintf(filename,"%s%s",dir,start);
        }

    }
return(filename);
}
#endif /* NeXT */

/*****************************************************************************
  FUNCTION : ui_loadSelectedFile

  PURPOSE  : loads the current file
  RETURNS  : void
  NOTES    : 
 
  UPDATE   :
*****************************************************************************/

static void ui_loadSelectedFile (Widget w, Widget button, caddr_t call_data)

{
    char selectedName[SELECTED_NAME_LENGTH];

    ui_xStringFromAsciiWidget(fileBox, selectedName, SELECTED_NAME_LENGTH);
    ui_xStringFromAsciiWidget(ui_path, ui_pathname, MAX_NAME_LENGTH);
    switch (currentFileType) {
        case UI_FILE_NET: strcpy (ui_filenameNET, selectedName);
                          ui_file_loadNet(w, (XtPointer) button, call_data);
                          break;
	case UI_FILE_PAT: strcpy (ui_filenamePAT, selectedName);
                          ui_file_loadPatterns(w, (XtPointer) button, call_data);
                          break;
	case UI_FILE_RES: strcpy (ui_filenameRES, selectedName);
                          ui_file_loadResult(w, (XtPointer) button, call_data);
                          break;
	case UI_FILE_CFG: strcpy (ui_filenameCFG, selectedName);
                          ui_file_loadConfiguration(w, (XtPointer) button, call_data);
                          break;
	case UI_FILE_TXT: strcpy (ui_filenameTXT, selectedName);
                          ui_file_loadText(w, (XtPointer) button, call_data);
                          break;
    }
}


/*****************************************************************************
  FUNCTION : ui_saveSelectedFile

  PURPOSE  : saves the current file
  RETURNS  : void
  NOTES    : 

  UPDATE   :
*****************************************************************************/

static void ui_saveSelectedFile (Widget w, Widget button, caddr_t call_data)

{
    char selectedName[SELECTED_NAME_LENGTH];

    ui_xStringFromAsciiWidget(fileBox, selectedName, SELECTED_NAME_LENGTH);
    ui_xStringFromAsciiWidget(ui_path, ui_pathname, MAX_NAME_LENGTH);
    switch (currentFileType) {
        case UI_FILE_NET: strcpy (ui_filenameNET, selectedName);
                          ui_file_saveNet(w, (XtPointer) button, call_data);
                          break;
	case UI_FILE_PAT: strcpy (ui_filenamePAT, selectedName);
                          ui_file_savePatterns(w, (XtPointer) button, call_data);
                          break;
	case UI_FILE_RES: strcpy (ui_filenameRES, selectedName);
                          ui_file_saveResult(w, (XtPointer) button, call_data);
                          break;
	case UI_FILE_CFG: strcpy (ui_filenameCFG, selectedName);
                          ui_file_saveConfiguration(w, (XtPointer) button, call_data);
                          break;
	case UI_FILE_TXT: strcpy (ui_filenameTXT, selectedName);
                          ui_file_saveText(w, (XtPointer) button, call_data);
                          break;
    }
}


/*****************************************************************************
  FUNCTION : ui_expandPath

  PURPOSE  : expands to full pathname if path = dir/../file
  RETURNS  : void
  NOTES    : 

  UPDATE   :
*****************************************************************************/

static void ui_expandPath (char *path)

{
    char cwd[SELECTED_NAME_LENGTH];

    if (*path != '/') {
        getcwd(cwd, SELECTED_NAME_LENGTH);
        strcat(cwd, "/");
        strcat(cwd, path);
        strcpy(path, cwd);   
    }     
}


/*****************************************************************************
  FUNCTION : ui_checkPath

  PURPOSE  : checks the existance of a directory
  RETURNS  : current directory if failed
  NOTES    : 

  UPDATE   :
*****************************************************************************/

static void ui_checkPath (char *path)

{
    char cwd[SELECTED_NAME_LENGTH];
    char errorMsg[2048];
    DIR *dirp;

    dirp = opendir (path); 
    if (dirp == NULL) {
        getcwd(cwd, SELECTED_NAME_LENGTH);
        sprintf (errorMsg, "Error! Can't read path:\n%s\nchanging to\n%s\n", 
                   ui_pathname, cwd);
	ui_confirmOk(errorMsg);
        strcpy(path, cwd);
        ui_xSetString(ui_path, path);
    } else 	
        closedir (dirp);
}


/*****************************************************************************
  FUNCTION : ui_strSort

  PURPOSE  : sorts the current directory
  RETURNS  : void
  NOTES    : modified quicksort algorithm for strings

  UPDATE   :
*****************************************************************************/

static void ui_strSort (int left, int right)

{

    int i, j;
    char *px, *pw;

    do {
        px = dirPtr[(left+right) / 2];
        i = left;
        j = right;
        do {
            while (strcmp(dirPtr[i], px) < 0)
                i++;
            while (strcmp(dirPtr[j], px) > 0)
                j--;
            if (i > j)
                break;
            pw = dirPtr[i];
            dirPtr[i] = dirPtr[j];
            dirPtr[j] = pw;
	} 
        while (++i <= --j);
        if (j - left < right - i) {
            if (left < j)
                ui_strSort (left, j);
            left = i;
            j = right;
        } else {
            if (i < right)
                ui_strSort (i, right);
            right = j;
            i = left;
        }
    }
    while (left < right);   

}


/*****************************************************************************
  FUNCTION : ui_readDirectory

  PURPOSE  : reads the contents of the current directory
  RETURNS  : void
  NOTES    : puts 0x01 and 0x02 around the name if entry is a subdirectory

  UPDATE   :
*****************************************************************************/

static void ui_readDirectory (void)

{
    struct dirent *dp;   
    DIR *dirp;
    int i;
    char *sptr, *pptr;
    char newpath[MAX_NAME_LENGTH];
    char tempName[MAX_NAME_LENGTH];
    
    dirEntries[0] = '\0';
    dirp = opendir (ui_pathname); 
    stat_buf = (struct stat *) malloc (sizeof (struct stat)); 
    while((dp = readdir(dirp)) != NULL){
        strcpy (newpath, ui_pathname);
        strcat (newpath, "/");
        strcat (newpath, dp->d_name);
        stat (newpath, stat_buf);
        if (S_ISDIR(stat_buf->st_mode)   AND
            (access(newpath,R_OK | X_OK)==0)) {
            strcat (dirEntries, "\1");
            strcat (dirEntries, dp->d_name);
            strcat (dirEntries, "\2");
	    strcat (dirEntries, "\n");
        } else {
	    sptr = strrchr (dp->d_name, '.'); 
            if (sptr != NULL) {
                if (strcmp(sptr, extMask) == 0) {
   	            strcpy (tempName, dp->d_name);
	            pptr = strrchr (tempName, '.');
	            *pptr = '\0';
                    strcat (dirEntries, tempName);
                    strcat (dirEntries, "\n");
		}
	    }
	}
    }    
    closedir (dirp);
    free (stat_buf);
    maxEntries = 1;
    dirPtr[0] = dirEntries;
    for (sptr = dirEntries; *sptr != '\0'; sptr++) {
        if (*sptr == '\n') {
            *sptr = '\0';
            dirPtr[maxEntries] = sptr + 1;
            maxEntries++;
            if (maxEntries == MAX_DIR_ENTRIES) {
        	ui_confirmOk("Error! Directory too large");
                return;
	    }
	}
    }

    ui_strSort (0, maxEntries-2);

    sortedDirEntries[0] = '\0';
    for (i=0; i<maxEntries; i++) {
        strcat (sortedDirEntries, dirPtr[i]);
        strcat (sortedDirEntries, "\n");
    }

    for (sptr = sortedDirEntries; *sptr != '\0'; sptr++) {
        switch (*sptr) {
	    case '\1': *sptr = '[';
                       break;
	    case '\2': *sptr = ']';
                       break;
        }
    }

}


/*****************************************************************************
  FUNCTION : ui_setExtString

  PURPOSE  : sets the file extention to .txt, .pat, cfg or .txt
  RETURNS  : void
  NOTES    : 

  UPDATE   :
*****************************************************************************/

static void ui_setExtString (Widget w, int fileType, caddr_t call_data)

{
    Arg		args[2];
    Cardinal	n;

    ui_xSetToggleState (netToggle, FALSE);    
    ui_xSetToggleState (patToggle, FALSE);    
    ui_xSetToggleState (resToggle, FALSE);    
    ui_xSetToggleState (cfgToggle, FALSE);    
    ui_xSetToggleState (txtToggle, FALSE);    
    switch (fileType) {
        case UI_FILE_NET: ui_xSetToggleState (netToggle, TRUE);
                          ui_xSetString(fileBox, ui_filenameNET);
                          strcpy (extMask, ".net");
                          break;
        case UI_FILE_PAT: ui_xSetToggleState (patToggle, TRUE);
                          ui_xSetString(fileBox, ui_filenamePAT);
                          strcpy (extMask, ".pat");
                          break;
        case UI_FILE_RES: ui_xSetToggleState (resToggle, TRUE);
                          ui_xSetString(fileBox, ui_filenameRES);
                          strcpy (extMask, ".res");
                          break;
        case UI_FILE_CFG: ui_xSetToggleState (cfgToggle, TRUE);
                          ui_xSetString(fileBox, ui_filenameCFG);
                          strcpy (extMask, ".cfg");
                          break;
        case UI_FILE_TXT: ui_xSetToggleState (txtToggle, TRUE);
                          ui_xSetString(fileBox, ui_filenameTXT);
                          strcpy (extMask, ".txt");
                          break;
    }
    currentFileType = fileType;
    ui_xStringFromAsciiWidget(ui_path, ui_pathname, MAX_NAME_LENGTH);
    ui_checkPath(ui_pathname);
    ui_readDirectory ();
    n = 0;
    XtSetArg(args[n], XtNstring, sortedDirEntries); n++;
    XtSetValues (selectorBox, args, n);
}


/*****************************************************************************
  FUNCTION : ui_changeDirectory

  PURPOSE  : changes the current path for the input
  RETURNS  : void
  NOTES    : doesn't work with an auto mounter

  UPDATE   :
*****************************************************************************/

static void ui_changeDirectory (char dirName[])
	/* K&R-Style:  char dirName[MAX_NAME_LENGTH]; */
     
{
    char *sptr;
    Arg		args[2];
    Cardinal	n;

    if ((dirName[0] == '.') AND (dirName[1] == '\0'))
        return;
    if ((dirName[0] == '.') AND (dirName[1] == '.')) {
	if (strcmp (ui_pathname, "/") == 0)
	    return;
	sptr = strrchr(ui_pathname, '/');
	if (sptr != NULL){  
	    if (sptr == ui_pathname)
		sptr++;
	    *sptr = '\0';
	}
    } else {
	if (strcmp (ui_pathname, "/") != 0)
	    strcat(ui_pathname, "/");
	strcat(ui_pathname, dirName);
    }
    ui_checkPath(ui_pathname);
    ui_readDirectory();
    ui_xSetString(ui_path, ui_pathname);
    n = 0;
    XtSetArg(args[n], XtNstring, sortedDirEntries); n++;
    XtSetValues (selectorBox, args, n);
}


/*****************************************************************************
  FUNCTION : ui_selectionEventProc

  PURPOSE  : puts the current filename in the dialog wisgwt after the second
             mouseklick
  RETURNS  : void
  NOTES    : 

  UPDATE   :
*****************************************************************************/

static void ui_selectionEventProc (Widget w, Display *display, XEvent *event)

{
    Widget           src;
    XawTextBlock     textBlock;
    XawTextPosition  start, end, found;
    Cardinal         n;
    Arg              args[5];
    char             selectedName[SELECTED_NAME_LENGTH];
    char             realName[SELECTED_NAME_LENGTH];
    Boolean          nlStarts, nlEnds;
    int              i;

    selectedName[0] = '\0';
    switch (event->type) {
        case ButtonRelease:
            if (event->xbutton.button == 1) {
                 XawTextGetSelectionPos(selectorBox, &start, &end);
                 if (start != end) {
                     n = 0;
	             XtSetArg(args[n], XtNtextSource, &src); n++;
	             XtGetValues (selectorBox, args, n);
	             found = XawTextSourceRead(src, (int) start, &textBlock, 
                         (int) (end-start));
	             if (found > 0) {
                         nlStarts = nlEnds = FALSE;
	                 strncpy(selectedName, textBlock.ptr-1, 
				(unsigned int) (end-start+2));
                         selectedName[end-start+2] = '\0';
        	         if (selectedName[0] == '\n')
                             nlStarts = TRUE;
	                 if (selectedName[strlen(selectedName)-1] == '\n')
                             nlEnds = TRUE;
                         if (nlStarts AND nlEnds) {
                             for (i=0; i<end-start; i++)
                                 realName[i] = selectedName[i+1];
                             realName[end-start] = '\0';
                             if (strchr(realName, '\n') != NULL)
                                 return;
                             if (realName[0] != '[') {
                                 n = 0;
                                 XtSetArg (args[n], XtNstring, realName); n++;
                                 XtSetValues (fileBox, args, n);
			     } else {
			         for (i=0; i<strlen(realName)-2; i++)
                                      realName[i] = realName[i+1];
				 realName[strlen(realName)-2] = '\0';
				 ui_changeDirectory(realName);
			     }
		         }
		     }
		 }
	  }
    }
}


/*****************************************************************************
  FUNCTION : ui_xCreateFilePanel

  PURPOSE  : create the file selector panel
  RETURNS  : void
  NOTES    : the wigdet will be created in relative position to eachother

  UPDATE   :
*****************************************************************************/

void ui_xCreateFilePanel (Widget parent)
           /* the parent widget of the new form widget */

{
    int  fontWidth = 8;
    int  titelWidth  = 14 * fontWidth;
    int  nameWidth   = 15 * fontWidth;  

    Arg		args[20];
    Cardinal	n;
    Widget      dummy, button, label;

    ui_filePanel =
	XtCreateManagedWidget("fPanel", formWidgetClass, parent, NULL, ZERO);
    ui_expandPath(ui_pathname);
    ui_path =
	ui_xCreateDialogItem("path", ui_filePanel, ui_pathname, 0 /* free */, NULL, NULL);

    strcpy (extMask, ".net");
    currentFileType = UI_FILE_NET;

    ui_checkPath(ui_pathname);
    ui_readDirectory ();

    n = 0;
    XtSetArg(args[n], XtNwidth,  150); n++;
    XtSetArg(args[n], XtNheight, 245); n++;
    XtSetArg(args[n], XtNtype, XawAsciiString); n++;
    XtSetArg(args[n], XtNstring, sortedDirEntries); n++;
    XtSetArg(args[n], XtNscrollVertical, XawtextScrollAlways); n++;
    XtSetArg(args[n], XtNdisplayCaret, FALSE); n++;
    XtSetArg(args[n], XtNfromVert , ui_path);  n++;
    XtSetArg(args[n], XtNfromHoriz, NULL);  n++;
    XtSetArg(args[n], XtNleft  , XtChainLeft);   n++;
    XtSetArg(args[n], XtNright , XtChainLeft);   n++;
    XtSetArg(args[n], XtNtop   , XtChainTop);    n++;
    XtSetArg(args[n], XtNbottom, XtChainTop);    n++;	
    selectorBox = XtCreateManagedWidget("textWin", asciiTextWidgetClass, 
       ui_filePanel, args, n);

    XtAddEventHandler (selectorBox, ButtonReleaseMask, 
                        FALSE, (XtEventHandler) ui_selectionEventProc, 
			ui_display);

    fileBox =
	ui_xCreateDialogItem("filebox", ui_filePanel, "", nameWidth,
      			     selectorBox, ui_path);

    ui_xSetResize(fileBox, TRUE);
    ui_xSetString(fileBox, ui_filenameNET);

    dummy =
	ui_xCreateLabelItem(" ", ui_filePanel, 8, selectorBox, fileBox);

    dummy =
	ui_xCreateLabelItem(" ", ui_filePanel, 8, selectorBox, dummy);

    netToggle =
	ui_xCreateToggleItem("net", ui_filePanel, NULL,
        selectorBox, dummy);
    label =
	ui_xCreateLabelItem(" Network", ui_filePanel, titelWidth,
        netToggle, dummy);
    patToggle =
	ui_xCreateToggleItem("pat", ui_filePanel, NULL, 
        selectorBox , netToggle);
    label =
	ui_xCreateLabelItem(" Patterns", ui_filePanel, titelWidth,  
        patToggle, netToggle);
    resToggle =
	ui_xCreateToggleItem("res", ui_filePanel, NULL, 
        selectorBox , patToggle);
    label =
	ui_xCreateLabelItem(" Result File", ui_filePanel, titelWidth,  
        resToggle, patToggle);
    cfgToggle =
	ui_xCreateToggleItem("cfg", ui_filePanel, NULL,
        selectorBox , resToggle);
    label =
	ui_xCreateLabelItem(" Config", ui_filePanel, titelWidth,  
        cfgToggle, resToggle);
    txtToggle =
	ui_xCreateToggleItem("txt", ui_filePanel, NULL, 
        selectorBox , cfgToggle);
    label =
	ui_xCreateLabelItem(" Log File", ui_filePanel, titelWidth,  
        txtToggle, cfgToggle);

    XtAddCallback(netToggle, XtNcallback, (XtCallbackProc) ui_setExtString, 
		(caddr_t) UI_FILE_NET);
    XtAddCallback(patToggle, XtNcallback, (XtCallbackProc) ui_setExtString, 
		(caddr_t) UI_FILE_PAT);
    XtAddCallback(resToggle, XtNcallback, (XtCallbackProc) ui_setExtString, 
		(caddr_t) UI_FILE_RES);
    XtAddCallback(cfgToggle, XtNcallback, (XtCallbackProc) ui_setExtString, 
		(caddr_t) UI_FILE_CFG);
    XtAddCallback(txtToggle, XtNcallback, (XtCallbackProc) ui_setExtString, 
		(caddr_t) UI_FILE_TXT);

    dummy =
	ui_xCreateLabelItem(" ", ui_filePanel, 8, selectorBox, txtToggle);

    dummy =
	ui_xCreateLabelItem(" ", ui_filePanel, 8, selectorBox, dummy);

    dummy =
	ui_xCreateLabelItem(" ", ui_filePanel, 8, selectorBox, dummy);

    button =
	ui_xCreateButtonItem("load", ui_filePanel, selectorBox , dummy);

    XtAddCallback(button, XtNcallback,
        (XtCallbackProc) ui_loadSelectedFile, NULL);

    button =
	ui_xCreateButtonItem("save", ui_filePanel, button , dummy);

    XtAddCallback(button, XtNcallback,
        (XtCallbackProc) ui_saveSelectedFile, NULL);

    XawFormDoLayout(ui_filePanel, True);

    ui_xSetToggleState (netToggle, TRUE);    
 
}




/* end of file */
/* lines: 521 */
