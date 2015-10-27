/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/xgui/sources/ui_xWidgets.c,v $
  SHORTNAME      : xWidgets 
  SNNS VERSION   : 4.2

  PURPOSE        : easy creating of labels, string-edit, buttons, radio-buttons
  NOTES          :

  AUTHOR         : Tilman Sommer 
  DATE           : 13.7.1990

  CHANGED BY     : Michael Vogt, Guenter Mamier
  RCS VERSION    : $Revision: 2.30 $ 
  LAST CHANGE    : $Date: 1998/03/03 14:10:49 $  

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/
#include <config.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "ui.h"
#include "ui_action.h"
#include "ui_main.h"
#include "ui_mainP.h"

#include <X11/Shell.h>
#include <X11/Xaw/Form.h>
#include <X11/Xaw/List.h>
#include <X11/Xaw/Command.h>
#include <X11/Xaw/Toggle.h>
#include <X11/Xaw/AsciiText.h>
#include <X11/Xaw/AsciiSrc.h>
#include <X11/Xaw/Label.h>
#include <X11/Xaw/MenuButton.h>
#include <X11/Xaw/Scrollbar.h>

#ifndef Widgets
#include <X11/Intrinsic.h>
#endif

#include "error.xbm"
#include "windows.xbm"
#include "selected.xbm"
#include "flags.xbm"
#include "ok.xbm"
#include "yes.xbm"
#include "no.xbm"
#include "save.xbm"
#include "load.xbm"
#include "first.xbm"
#include "prev.xbm"
#include "next.xbm"
#include "last.xbm"
#include "def.xbm"
#include "set.xbm"
#include "done.xbm"
#include "select.xbm"
#include "add.xbm"
#include "delete.xbm"
#include "delete_cand_units.xbm"
#include "choose.xbm"
#include "new.xbm"
#include "newL.xbm"
#include "look.xbm"
#include "more.xbm"
#include "topics.xbm"
#include "step.xbm"
#include "test.xbm"
#include "stop.xbm"
#include "reset.xbm"
#include "info.xbm"
#include "single.xbm"
#include "all.xbm"
#include "layers.xbm"
#include "setup.xbm"
#include "freeze.xbm"
#include "icon.xbm"
#include "show.xbm"
#include "mode.xbm"
#include "options.xbm"
#include "on.xbm"
#include "off.xbm"
#include "value.xbm"
#include "direction.xbm"
#include "shuffle.xbm"
#include "colors.xbm"
#include "modify.xbm"
#include "goto.xbm"
#include "init.xbm"
#include "connect.xbm"
#include "disconnect.xbm"
#include "standard.xbm"
#include "feedforward.xbm"
#include "net.xbm"
#include "pat.xbm"
#include "cfg.xbm"
#include "txt.xbm"
#include "help.xbm"
#include "print.xbm"
#include "printer.xbm"
#include "postscript.xbm"
#include "eps.xbm"
#include "dina4.xbm"
#include "dina3.xbm"
#include "usletter.xbm"
#include "file.xbm"
#include "portrait.xbm"
#include "landscape.xbm"
#include "network.xbm"
#include "fixed.xbm"
#include "value2.xbm"
#include "rectangle.xbm"
#include "circle.xbm"
#include "solid2.xbm"
#include "trans.xbm"
#include "cancel.xbm"
#include "create.xbm"
#include "clear.xbm"
#include "append.xbm"
#include "res.xbm"
#include "default.xbm"
#include "text.xbm"
#include "selection.xbm"
#include "background.xbm"
#include "maspar.xbm"
#include "view.xbm"
#include "norm.xbm"
#include "super3d.xbm"
#include "zoomin.xbm"
#include "zoomout.xbm"
#include "winner.xbm"
#include "use.xbm"
#include "delSet.xbm"
#include "subpat.xbm"
#include "parallel.xbm"
#include "valid.xbm"
#include "sel_func.xbm"
#include "fileP.xbm"
#include "controlP.xbm"
#include "infoP.xbm"
#include "display2.xbm"
#include "display3d.xbm"
#include "graph.xbm"
#include "bignet.xbm"
#include "pruning.xbm"
#include "print2.xbm"
#include "cascade.xbm"
#include "analyser.xbm"
#include "kohonen2.xbm"
#include "weights.xbm"
#include "inversion.xbm"
#include "projection.xbm"
#include "help2.xbm"
#include "quit.xbm"
#include "grid.xbm"
#include "enter.xbm"
#include "insert.xbm"
#include "overwrite.xbm"
#include "plane_to_edit.xbm"
#include "link_to_edit.xbm"
#include "type.xbm"
#include "pos.xbm"
#include "full_connection.xbm"
#include "shortcut_connection.xbm"
#include "create_td_net.xbm"
#include "create_net.xbm"
#include "jog.xbm"
#include "editors.xbm"
#include "classesP.xbm"

/* ui_x...FromAsciiWidget */
#define MAX_INPUT_BUFLEN 20

#include "ui_xWidgets.ph"


#ifdef _UNUSED_FUNCTIONS_
/*****************************************************************************
  FUNCTION :  ui_addString

  PURPOSE  : adds a string to a specified array of (char *)
  NOTES    : 
  RETURNS  :
  UPDATE   : 1.5.1990
******************************************************************************/



void ui_addString(char **arrayPtr,int *numItems, int *allocatedItems,char *string)
{
    if (*numItems == *allocatedItems) {
	*allocatedItems += 10;
	arrayPtr = 
	    (char **) XtRealloc(*arrayPtr, sizeof(String) * (*allocatedItems));
    }
    arrayPtr[(*numItems)++] = XtNewString(string); 
}

#endif

/*****************************************************************************
  FUNCTION :  ui_xAddListEntry

  PURPOSE  : adds a list entry to the specified list-widget
  NOTES    : Changed version of the function Okay in the example program
             xwidgets.c of the Athena Toolkit.
  RETURNS  : void
  UPDATE   : 1.5.1990
******************************************************************************/



void ui_xAddListEntry(struct SimpleListType *listDescriptorPtr,String string)
{
    if (listDescriptorPtr->noOfItems == listDescriptorPtr->noOfAllocItems) {
	listDescriptorPtr->noOfAllocItems += 10;
	 (listDescriptorPtr->listPtr) = 
	    (String *) XtRealloc((char *) listDescriptorPtr->listPtr, 
				 (unsigned int) sizeof(String *) * listDescriptorPtr->noOfAllocItems);
    }
    (listDescriptorPtr->listPtr)[(listDescriptorPtr->noOfItems)++] = 
	XtNewString(string); 
    XawListChange(listDescriptorPtr->listWidget, listDescriptorPtr->listPtr, 
		  listDescriptorPtr->noOfItems, 0, True);
}






/*****************************************************************************
  FUNCTION :                   ui_xSetResize

  PURPOSE  : used to set the resize resource of a asciiText widget
  NOTES    :
  RETURNS  : void
  UPDATE   :
*****************************************************************************/

void ui_xSetResize(Widget widget,Boolean state)
{
    Cardinal      n;
    Arg           args[3];

    n = 0;
    if (state) 
	XtSetArg(args[n], XtNresize, XawtextResizeWidth);
    else 
	XtSetArg(args[n], XtNresize, XawtextResizeNever);
    
    n++;
    XtSetArg(args[n], XtNresizable, state); n++;
    XtSetValues(widget, args, n);
}


/*****************************************************************************
  FUNCTION :                   ui_xSetLabel

  PURPOSE  : used to set the label of a widget
  NOTES    :
  RETURNS  : void
  UPDATE   :
*****************************************************************************/

void ui_xSetLabel(Widget widget,char *newLabel)
{
    Cardinal      n;
    Arg           args[2];

    n = 0;
    XtSetArg(args[n], XtNlabel, newLabel); n++;
    XtSetValues(widget, args, n);
}



/*****************************************************************************
  FUNCTION :                   ui_xSetString

  PURPOSE  : used to set the string resource of a widget
  NOTES    :
  RETURNS  : void
  UPDATE   :
*****************************************************************************/

void ui_xSetString(Widget widget,char *contents)
{
    Cardinal      n;
    Arg           args[3];
    uint           lenContents;
    char          editBuf[MAXPATHLEN+1];

    if (contents == NULL)
	lenContents = 0;
    else {
	lenContents = strlen(contents);
	if (lenContents > MAXPATHLEN) lenContents = MAXPATHLEN;
	strncpy(editBuf,contents,lenContents);
    }

    editBuf[lenContents] = '\0';

    n = 0;
    XtSetArg(args[n], XtNstring,editBuf); n++;
    XtSetArg(args[n], XtNinsertPosition, lenContents); n++;
    XtSetValues(widget, args, n);
}




/*****************************************************************************
  FUNCTION : ui_xStringFromAsciiWidget

  PURPOSE  : used to get a string from an ascii widget
  NOTES    :
  RETURNS  : void
  UPDATE   :
*****************************************************************************/

void ui_xStringFromAsciiWidget(Widget widget,char *stringVar,int maxLen)
{
    Widget        asciiSrc;
    XawTextBlock  textBlock;
    Cardinal      n;
    Arg           args[5];

    n = 0;
    XtSetArg(args[n], XtNtextSource, &asciiSrc); n++;
    XtGetValues(widget, args, n);
    
    (void) XawTextSourceRead(asciiSrc, 0, &textBlock, maxLen-1);

    if (textBlock.length > maxLen-1) 
	textBlock.length = maxLen-1;
	
    strncpy(stringVar, textBlock.ptr,(uint) textBlock.length);
    stringVar[textBlock.length] = '\0';  /* this is for safeness */
}


	



/*****************************************************************************
  FUNCTION : ui_xFloatFromAsciiWidget

  PURPOSE  : used to get a string from a PANEL_TEXT item and transform it
             to an float.
  NOTES    :
  RETURNS  : The numerical value hold from panelItem
  UPDATE   :
*****************************************************************************/

float ui_xFloatFromAsciiWidget(Widget widget)
{
    Widget        asciiSrc;
    XawTextBlock  textBlock;
    char          buf[MAX_INPUT_BUFLEN + 1];
    Cardinal      n;
    Arg           args[5];
    float         rVal;

    n = 0;
    XtSetArg(args[n], XtNtextSource, &asciiSrc); n++;
    XtGetValues(widget, args, n);
    
    (void) XawTextSourceRead(asciiSrc, 0, &textBlock, MAX_INPUT_BUFLEN);

    if (textBlock.length > MAX_INPUT_BUFLEN) 
	textBlock.length = MAX_INPUT_BUFLEN;
	
    strncpy(buf, textBlock.ptr, (uint) textBlock.length);
    buf[textBlock.length] = '\0';  /* this is for safeness */
    if (strlen(buf) == 0) return(0.0);
    else {
	sscanf(buf,"%g", &rVal);
	return(rVal);
    }
}


/*****************************************************************************
  FUNCTION : ui_xUtilIntFromAsciiWidget

  PURPOSE  : used to get a string from a PANEL_TEXT item and transform it
             to an integer.
  NOTES    :
  RETURNS  : The numerical value hold from panelItem
  UPDATE   :
*****************************************************************************/

int ui_xIntFromAsciiWidget(Widget widget)
{
    Widget        asciiSrc;
    XawTextBlock  textBlock;
    char          buf[MAX_INPUT_BUFLEN + 1];
    Cardinal      n;
    Arg           args[5];

    n = 0;
    XtSetArg(args[n], XtNtextSource, &asciiSrc); n++;
    XtGetValues(widget, args, n);
    
    (void) XawTextSourceRead(asciiSrc, 0, &textBlock, MAX_INPUT_BUFLEN);

    if (textBlock.length > MAX_INPUT_BUFLEN) 
	textBlock.length = MAX_INPUT_BUFLEN;
	
    strncpy(buf, textBlock.ptr,(uint) textBlock.length);
    buf[textBlock.length] = '\0';  /* this is for safeness */
    if (strlen(buf) == 0) return(0);
    else return(atoi(buf));
}





/*****************************************************************************
  FUNCTION : ui_xGetDimensions

  PURPOSE  : get width and height of specified widget
  NOTES    :
  RETURNS  : void
  UPDATE   : 1.2.1990
******************************************************************************/

void ui_xGetDimensions(Widget widget,Dimension *width,Dimension *height)
{
    Cardinal   n;
    Arg	       args[15];

    n = 0;
    XtSetArg(args[n], XtNwidth,  width); n++;
    XtSetArg(args[n], XtNheight, height); n++;
    XtGetValues(widget, args, n);
}



/*****************************************************************************
  FUNCTION : ui_xGetToggleState

  PURPOSE  : returns the state of the specified toggle
  NOTES    : TRUE means PUSHED;

  RETURNS  : State of the Toggle
  UPDATE   : 26.8.1990
******************************************************************************/

Bool ui_xGetToggleState(Widget w)
{
    Cardinal  n;
    Arg       args[2];
    Boolean   state;

    n=0;
    XtSetArg(args[n], XtNstate, &state); n++;
    XtGetValues(w, args, n);
    return(state == TRUE); /* prevents compatibiltiy problems */
}




/*****************************************************************************
  FUNCTION : ui_xSetToggleState

  PURPOSE  : sets the state of the specified toggle
  NOTES    : 
  RETURNS  : void
  UPDATE   : 26.8.1990
******************************************************************************/

void ui_xSetToggleState(Widget w,Bool state)
{
    Cardinal n;
    Arg       args[2];

    n=0;
    XtSetArg(args[n], XtNstate, (Boolean) state); n++;
    XtSetValues(w, args, n);
}



/*****************************************************************************
  FUNCTION : ui_setToggles

  PURPOSE  : toggles one of the radio button groups. Also resets all the other
             buttons in the group
  RETURNS  : void
  NOTES    : sets the toggle variable to the new value

  UPDATE   : 14.11.97
*****************************************************************************/
void ui_setToggles (int index, int *new, int max, Widget widgets[])
{
   int i;

   for(i=0; i<max; i++)
       ui_xSetToggleState(widgets[i], FALSE);
   ui_xSetToggleState(widgets[index], TRUE);
   *new = index;
}




/*****************************************************************************
  FUNCTION : ui_xCreateLabelItem

  PURPOSE  : create a label widget
  NOTES    :
  RETURNS  : The Widgetnumber of the Label
  UPDATE   : 15.5.1990
******************************************************************************/


Widget   ui_xCreateLabelItem(char *name,Widget parent,Dimension width,Widget left,Widget top)
{ 
    Cardinal n;
    Widget   w;
    Arg	     args[15];

    n = 0;
    if (strcmp(name,"noSel") == 0) {
	XtSetArg(args[n], XtNbitmap, 
		 (Pixmap) XCreateBitmapFromData(ui_display, 
						XDefaultRootWindow(ui_display),
						selected_bits, selected_width,
						selected_height)); n++;
    }
    if (strcmp(name,"flags") == 0) {
	XtSetArg(args[n], XtNbitmap, 
		 (Pixmap) XCreateBitmapFromData(ui_display, 
						XDefaultRootWindow(ui_display),
						flags_bits, flags_width,
						flags_height)); n++;
    }    
    XtSetArg(args[n], XtNborderWidth, 0); n++;
    if (width != 0) {
	XtSetArg(args[n], XtNwidth, (XtArgVal) width); n++;
	XtSetArg(args[n], XtNresize, FALSE); n++;
    } else {
	XtSetArg(args[n], XtNresizable, True); n++;	
	XtSetArg(args[n], XtNresize,    TRUE); n++;
    }
    XtSetArg(args[n], XtNjustify, XtJustifyLeft); n++;
/*    XtSetArg(args[n], XtNinternalHeight, 1); n++;
    XtSetArg(args[n], XtNinternalWidth , 1); n++;
*/    XtSetArg(args[n], XtNfromHoriz, left); n++;
    XtSetArg(args[n], XtNfromVert , top);  n++;
    XtSetArg(args[n], XtNleft  , XtChainLeft); n++;
    XtSetArg(args[n], XtNright , XtChainLeft); n++;
    XtSetArg(args[n], XtNtop   , XtChainTop); n++;
    XtSetArg(args[n], XtNbottom, XtChainTop); n++;
    w = XtCreateManagedWidget(name, labelWidgetClass, parent, args, n);
    return(w);
}

/*****************************************************************************
  FUNCTION : ui_xCreateFrameLabelItem

  PURPOSE  : create a label widget
  NOTES    :
  RETURNS  : The Widgetnumber of the Label
  UPDATE   : 15.5.1990
******************************************************************************/
Widget   ui_xCreateFrameLabelItem(char *name,Widget parent,Dimension width,Widget left,Widget top)
{ 
    Cardinal n;
    Widget   w;
    Arg	     args[15];

    n = 0;
    if (strcmp(name,"noSel") == 0) {
	XtSetArg(args[n], XtNbitmap, 
		 (Pixmap) XCreateBitmapFromData(ui_display, 
						XDefaultRootWindow(ui_display),
						selected_bits, selected_width,
						selected_height)); n++;
    }
    if (strcmp(name,"flags") == 0) {
	XtSetArg(args[n], XtNbitmap, 
		 (Pixmap) XCreateBitmapFromData(ui_display, 
						XDefaultRootWindow(ui_display),
						flags_bits, flags_width,
						flags_height)); n++;
    }    
    XtSetArg(args[n], XtNborderWidth, 1); n++;
    if (width != 0) {
	XtSetArg(args[n], XtNheight, 17); n++;
	XtSetArg(args[n], XtNwidth, (XtArgVal) width); n++;
	XtSetArg(args[n], XtNresize, FALSE); n++;
    } else {
	XtSetArg(args[n], XtNresizable, True); n++;	
	XtSetArg(args[n], XtNresize,    TRUE); n++;
    }
    XtSetArg(args[n], XtNjustify, XtJustifyLeft); n++;
    XtSetArg(args[n], XtNinternalHeight, 3); n++;
    XtSetArg(args[n], XtNinternalWidth , 4); n++;
    XtSetArg(args[n], XtNfromHoriz, left); n++;
    XtSetArg(args[n], XtNfromVert , top);  n++;
    XtSetArg(args[n], XtNleft  , XtChainLeft); n++;
    XtSetArg(args[n], XtNright , XtChainLeft); n++;
    XtSetArg(args[n], XtNtop   , XtChainTop); n++;
    XtSetArg(args[n], XtNbottom, XtChainTop); n++;
    w = XtCreateManagedWidget(name, labelWidgetClass, parent, args, n);
    return(w);
}


/*****************************************************************************
  FUNCTION : ui_xCreateFrameLabelItemCentered

  PURPOSE  : create a label widget
  NOTES    : text is centered
  RETURNS  : The Widgetnumber of the Label
  UPDATE   : 15.5.1990
******************************************************************************/

Widget ui_xCreateFrameLabelItemCentered (char *name, Widget parent,
					 Dimension width,
					 Widget left, Widget top)

{
 
    Cardinal n;
    Widget   w;
    Arg	     args[15];
    
    n = 0;
    XtSetArg (args [n], XtNborderWidth, 1); n++;
    if (width != 0)
    {
	XtSetArg (args [n], XtNheight, 17); n++;
	XtSetArg (args [n], XtNwidth, (XtArgVal) width); n++;
	XtSetArg (args [n], XtNresize, FALSE); n++;
    }
    else
    {
	XtSetArg (args [n], XtNresizable, True); n++;	
	XtSetArg (args [n], XtNresize, TRUE); n++;
    }
    XtSetArg (args [n], XtNjustify, XtJustifyCenter); n++;
    XtSetArg (args [n], XtNinternalHeight, 3); n++;
    XtSetArg (args [n], XtNinternalWidth, 4); n++;
    XtSetArg (args [n], XtNfromHoriz, left); n++;
    XtSetArg (args [n], XtNfromVert, top);  n++;
    XtSetArg (args [n], XtNleft, XtChainLeft); n++;
    XtSetArg (args [n], XtNright, XtChainLeft); n++;
    XtSetArg (args [n], XtNtop, XtChainTop); n++;
    XtSetArg (args [n], XtNbottom, XtChainTop); n++;

    w = XtCreateManagedWidget (name, labelWidgetClass, parent, args, n);

    return(w);

}


/*****************************************************************************
  FUNCTION : ui_xCreateDialogItem

  PURPOSE  : create an ascii text widget (edit type = string)
  NOTES    :
  RETURNS  : The Widgetnumber of the text Widget
  UPDATE   : 1.2.1990
******************************************************************************/



Widget  ui_xCreateDialogItem(char *name,Widget parent,char *contents,Dimension  width,Widget left,Widget top)
{ 
    Cardinal        n;
    Widget          w;
    Arg	            args[25];
    char            buf[MAXPATHLEN+1];
    static XtActionsRec  
	ActionRec [] = {{"ui_noreturn",(XtActionProc) ui_noreturn},
			{"ui_moveup",(XtActionProc) ui_moveup},
			{"ui_movedown",(XtActionProc) ui_movedown},
			{"ui_moveleft",(XtActionProc) ui_moveleft},
			{"ui_moveright",(XtActionProc) ui_moveright}};
    XtTranslations Translation;

    uint lenContents;

    if (contents == NULL) 
	lenContents = 0;
    else {
	lenContents = strlen(contents);
	if (lenContents > MAXPATHLEN) lenContents = MAXPATHLEN;
    }
    strncpy(buf,contents,lenContents);
    buf[lenContents] = '\0';


    n = 0;
    XtSetArg(args[n], XtNborderWidth,  1); n++;
    XtSetArg(args[n], XtNstring   , buf); n++;
    if (width != 0) {
	XtSetArg(args[n], XtNresize   , XawtextResizeNever); n++;
	XtSetArg(args[n], XtNwidth    , width);  n++;
    } else {
	XtSetArg(args[n], XtNwidth    , 8*lenContents + 20);  n++;
	XtSetArg(args[n], XtNresizable, True); n++;	
	XtSetArg(args[n], XtNresize   , XawtextResizeWidth); n++;
    }	
    XtSetArg(args[n], XtNscrollHorizontal, XawtextScrollNever); n++;
    XtSetArg(args[n], XtNscrollVertical  , XawtextScrollNever); n++;
    XtSetArg(args[n], XtNinsertPosition  , lenContents);  n++;
    XtSetArg(args[n], XtNeditType , XawtextEdit);  n++;
    XtSetArg(args[n], XtNtype     , XawAsciiString);  n++;
    XtSetArg(args[n], XtNfromVert , top);  n++;
    XtSetArg(args[n], XtNfromHoriz, left);  n++;
    XtSetArg(args[n], XtNleft  , XtChainLeft); n++;
    XtSetArg(args[n], XtNright , XtChainLeft); n++;
    XtSetArg(args[n], XtNtop   , XtChainTop); n++;
    XtSetArg(args[n], XtNbottom, XtChainTop); n++;
    w = XtCreateManagedWidget(name, asciiTextWidgetClass, parent, args, n);
#ifndef _HPUX_SOURCE
    XtAppAddActions (ui_appContext, ActionRec, XtNumber (ActionRec));
    Translation = XtParseTranslationTable ("<Key>Return: ui_noreturn()");
    XtOverrideTranslations (w, Translation);
    Translation = XtParseTranslationTable ("<Key>Up: ui_moveup()");
    XtOverrideTranslations (w, Translation);
    Translation = XtParseTranslationTable ("<Key>Down: ui_movedown()");
    XtOverrideTranslations (w, Translation);
/*
    Translation = XtParseTranslationTable ("<Key>Tab: ui_moveleft()");
    XtOverrideTranslations (w, Translation);
*/
    Translation = XtParseTranslationTable ("<Key>Tab: ui_moveright()");
    XtOverrideTranslations (w, Translation);
#endif
    return(w);
}



/*****************************************************************************
  FUNCTION : ui_xCreateButtonItem

  PURPOSE  : create a command widget
  NOTES    :
  RETURNS  : The Widgetnumber of the command widget
  UPDATE   : 18.5.1990
******************************************************************************/

Widget  ui_xCreateButtonItem(char *name,Widget parent,Widget left,Widget top)
{ 
    Cardinal n;
    Widget   w;
    Arg	     args[15];

    n = 0;
    if (strcmp(name,"3dP") == 0) {
	XtSetArg(args[n], XtNbitmap, 
		 (Pixmap) XCreateBitmapFromData(ui_display, 
						XDefaultRootWindow(ui_display),
						display3d_bits, display2_width,
						display2_height)); n++;
    }
    else if (strcmp(name,"add") == 0) {
	XtSetArg(args[n], XtNbitmap, 
		 (Pixmap) XCreateBitmapFromData(ui_display, 
						XDefaultRootWindow(ui_display),
						add_bits, add_width,
						add_height)); n++;
    }
    else if (strcmp(name,"all") == 0) {
	XtSetArg(args[n], XtNbitmap, 
		 (Pixmap) XCreateBitmapFromData(ui_display, 
						XDefaultRootWindow(ui_display),
						all_bits, all_width,
						all_height)); n++;
    }
    else if (strcmp(name,"analyzeP") == 0) {
	XtSetArg(args[n], XtNbitmap, 
		 (Pixmap) XCreateBitmapFromData(ui_display, 
						XDefaultRootWindow(ui_display),
						analyser_bits, display2_width,
						display2_height)); n++;
    }
    else if (strcmp(name,"cancel") == 0) {
        XtSetArg(args[n], XtNbitmap, 
                 (Pixmap) XCreateBitmapFromData(ui_display, 
                                                XDefaultRootWindow(ui_display),
                                                cancel_bits, cancel_width,
                                                cancel_height)); n++;
    }
    else if (strcmp(name,"cascadeP") == 0) {
	XtSetArg(args[n], XtNbitmap, 
		 (Pixmap) XCreateBitmapFromData(ui_display, 
						XDefaultRootWindow(ui_display),
						cascade_bits, display2_width,
						display2_height)); n++;
    }
    else if (strcmp(name,"choose") == 0) {
	XtSetArg(args[n], XtNbitmap, 
		 (Pixmap) XCreateBitmapFromData(ui_display, 
						XDefaultRootWindow(ui_display),
						choose_bits, choose_width,
						choose_height)); n++;
    }
    else if (strcmp(name,"classesP") == 0) {
	XtSetArg(args[n], XtNbitmap, 
		 (Pixmap) XCreateBitmapFromData(ui_display, 
						XDefaultRootWindow(ui_display),
						classesP_bits, classesP_width,
						classesP_height)); n++;
    }
    else if (strcmp (name, "clear") == 0){  
        XtSetArg(args[n], XtNbitmap, 
		 (Pixmap) XCreateBitmapFromData(ui_display,
						XDefaultRootWindow(ui_display),
						clear_bits, clear_width, 
						clear_height)); n++;
    } 
    else if (strcmp(name,"color") == 0) {
	XtSetArg(args[n], XtNbitmap, 
		 (Pixmap) XCreateBitmapFromData(ui_display, 
						XDefaultRootWindow(ui_display),
						color_bits, color_width,
						color_height)); n++;
    }
    else if (strcmp(name,"controlP") == 0) {
	XtSetArg(args[n], XtNbitmap, 
		 (Pixmap) XCreateBitmapFromData(ui_display, 
						XDefaultRootWindow(ui_display),
						controlP_bits, controlP_width,
						controlP_height)); n++;
    }
    else if (strcmp (name, "create_net") == 0) {  
        XtSetArg(args[n], XtNbitmap, 
		 (Pixmap) XCreateBitmapFromData(ui_display,
						XDefaultRootWindow(ui_display),
						create_net_bits,
						create_net_width,
						create_net_height)); n++;
    } 
    else if (strcmp (name, "create_td_net") == 0) {  
        XtSetArg(args[n], XtNbitmap, 
		 (Pixmap) XCreateBitmapFromData(ui_display,
						XDefaultRootWindow(ui_display),
						create_td_net_bits,
						create_td_net_width,
						create_td_net_height)); n++;
    } 
    else if (strcmp(name,"default") == 0) {
	XtSetArg(args[n], XtNbitmap, 
		 (Pixmap) XCreateBitmapFromData(ui_display, 
						XDefaultRootWindow(ui_display),
						def_bits, def_width,
						def_height)); n++;
    }
    else if (strcmp(name,"default2") == 0) {
        XtSetArg(args[n], XtNbitmap, 
                 (Pixmap) XCreateBitmapFromData(ui_display, 
                                                XDefaultRootWindow(ui_display),
                                                default_bits, default_width,
						default_height)); n++;
    }
    else if (strcmp(name,"delete") == 0) {
	XtSetArg(args[n], XtNbitmap, 
		 (Pixmap) XCreateBitmapFromData(ui_display, 
						XDefaultRootWindow(ui_display),
						delete_bits, delete_width,
						delete_height)); n++;
    }
    else if (strcmp (name, "delete_cand_units") == 0){  
        XtSetArg(args[n], XtNbitmap, 
		 (Pixmap)XCreateBitmapFromData(ui_display,
					      XDefaultRootWindow (ui_display), 
					      delete_cand_units_bits, 
					      delete_cand_units_width, 
					      delete_cand_units_height)); n++;
    } 
    else if (strcmp(name,"displayP") == 0) {
	XtSetArg(args[n], XtNbitmap, 
		 (Pixmap) XCreateBitmapFromData(ui_display, 
						XDefaultRootWindow(ui_display),
						display2_bits, display2_width,
						display2_height)); n++;
    }
    else if (strcmp(name,"done") == 0) {
	XtSetArg(args[n], XtNbitmap, 
		 (Pixmap) XCreateBitmapFromData(ui_display, 
						XDefaultRootWindow(ui_display),
						done_bits, done_width,
						done_height)); n++;
    }
    else if (strcmp (name, "enter") == 0) {  
        XtSetArg(args[n], XtNbitmap, 
		 (Pixmap) XCreateBitmapFromData(ui_display,
						XDefaultRootWindow(ui_display), 
						enter_bits, enter_width, 
						enter_height)); n++;
    } 
    else if (strcmp(name,"error") == 0) {
	XtSetArg(args[n], XtNbitmap, 
		 (Pixmap) XCreateBitmapFromData(ui_display, 
						XDefaultRootWindow(ui_display),
						error_bits, error_width,
						error_height)); n++;
    }
    else if (strcmp(name,"fileP") == 0) {
	XtSetArg(args[n], XtNbitmap, 
		 (Pixmap) XCreateBitmapFromData(ui_display, 
						XDefaultRootWindow(ui_display),
						fileP_bits, fileP_width,
						fileP_height)); n++;
    }
    else if (strcmp(name,"first") == 0) {
	XtSetArg(args[n], XtNbitmap, 
		 (Pixmap) XCreateBitmapFromData(ui_display, 
						XDefaultRootWindow(ui_display),
						first_bits, first_width,
						first_height)); n++;
    }
    else if (strcmp (name, "full_connection") == 0) {  
        XtSetArg(args[n], XtNbitmap, 
		 (Pixmap) XCreateBitmapFromData(ui_display,
						XDefaultRootWindow(ui_display), 
						full_connection_bits,
						full_connection_width,
						full_connection_height)); n++;
    } 
    else if (strcmp(name,"goto") == 0) {
	XtSetArg(args[n], XtNbitmap, 
		 (Pixmap) XCreateBitmapFromData(ui_display, 
						XDefaultRootWindow(ui_display),
						goto_bits, goto_width,
						goto_height)); n++;
    }
    else if (strcmp(name,"graphP") == 0) {
	XtSetArg(args[n], XtNbitmap, 
		 (Pixmap) XCreateBitmapFromData(ui_display, 
						XDefaultRootWindow(ui_display),
						graph_bits, display2_width,
						display2_height)); n++;
    }
    else if (strcmp(name,"help") == 0) {
	XtSetArg(args[n], XtNbitmap, 
		 (Pixmap) XCreateBitmapFromData(ui_display, 
						XDefaultRootWindow(ui_display),
						help_bits, help_width,
						help_height)); n++;
    }
    else if (strcmp(name,"helpP") == 0) {
	XtSetArg(args[n], XtNbitmap, 
		 (Pixmap) XCreateBitmapFromData(ui_display, 
						XDefaultRootWindow(ui_display),
						help2_bits, display2_width,
						display2_height)); n++;
    }
    else if (strcmp(name,"info") == 0) {
	XtSetArg(args[n], XtNbitmap, 
		 (Pixmap) XCreateBitmapFromData(ui_display, 
						XDefaultRootWindow(ui_display),
						info_bits, info_width,
						info_height)); n++;
    }
    else if (strcmp(name,"infoP") == 0) {
	XtSetArg(args[n], XtNbitmap, 
		 (Pixmap) XCreateBitmapFromData(ui_display, 
						XDefaultRootWindow(ui_display),
						infoP_bits, infoP_width,
						infoP_height)); n++;
    }
    else if (strcmp(name,"init") == 0) {
	XtSetArg(args[n], XtNbitmap, 
		 (Pixmap) XCreateBitmapFromData(ui_display, 
						XDefaultRootWindow(ui_display),
						init_bits, init_width,
						init_height)); n++;
    }
    else if (strcmp (name, "insert") == 0){  
        XtSetArg(args[n], XtNbitmap, 
		 (Pixmap) XCreateBitmapFromData(ui_display,
						XDefaultRootWindow(ui_display), 
						insert_bits,insert_width,
						insert_height)); n++;
    } 
    else if (strcmp(name,"inversionP") == 0) {
	XtSetArg(args[n], XtNbitmap, 
		 (Pixmap) XCreateBitmapFromData(ui_display, 
						XDefaultRootWindow(ui_display),
						inversion_bits, inversion_width,
						inversion_height)); n++;
    }
    else if (strcmp(name,"jog") == 0) {
	XtSetArg(args[n], XtNbitmap, 
		 (Pixmap) XCreateBitmapFromData(ui_display, 
						XDefaultRootWindow(ui_display),
						jog_bits, jog_width,
						jog_height)); n++;
    }
    else if (strcmp(name,"kohonenP") == 0) {
	XtSetArg(args[n], XtNbitmap, 
		 (Pixmap) XCreateBitmapFromData(ui_display, 
						XDefaultRootWindow(ui_display),
						kohonen2_bits, kohonen2_width,
						kohonen2_height)); n++;
    }
    else if (strcmp(name,"last") == 0) {
	XtSetArg(args[n], XtNbitmap, 
		 (Pixmap) XCreateBitmapFromData(ui_display, 
						XDefaultRootWindow(ui_display),
						last_bits, last_width,
						last_height)); n++;
    }
    else if (strcmp(name,"layers") == 0) {
	XtSetArg(args[n], XtNbitmap, 
		 (Pixmap) XCreateBitmapFromData(ui_display, 
						XDefaultRootWindow(ui_display),
						layers_bits, layers_width,
						layers_height)); n++;
    }
    else if (strcmp (name, "link_to_edit") == 0){  
        XtSetArg(args[n], XtNbitmap, 
		 (Pixmap) XCreateBitmapFromData(ui_display,
						XDefaultRootWindow(ui_display), 
						link_to_edit_bits,
						link_to_edit_width,
						link_to_edit_height)); n++;
    } 
    else if (strcmp(name,"load") == 0) {
	XtSetArg(args[n], XtNbitmap, 
		 (Pixmap) XCreateBitmapFromData(ui_display, 
						XDefaultRootWindow(ui_display),
						load_bits, load_width,
						load_height)); n++;
    }
    else if (strcmp(name,"look") == 0) {
	XtSetArg(args[n], XtNbitmap, 
		 (Pixmap) XCreateBitmapFromData(ui_display, 
						XDefaultRootWindow(ui_display),
						look_bits, look_width,
						look_height)); n++;
    }
    else if (strcmp(name,"maspar") == 0) {
	XtSetArg(args[n], XtNbitmap, 
		 (Pixmap) XCreateBitmapFromData(ui_display, 
						XDefaultRootWindow(ui_display),
						maspar_bits, maspar_width,
						maspar_height)); n++;
    }
    else if (strcmp(name,"modify") == 0) {
	XtSetArg(args[n], XtNbitmap, 
		 (Pixmap) XCreateBitmapFromData(ui_display, 
						XDefaultRootWindow(ui_display),
						modify_bits, modify_width,
						modify_height)); n++;
    }
    else if (strcmp(name,"more") == 0) {
	XtSetArg(args[n], XtNbitmap, 
		 (Pixmap) XCreateBitmapFromData(ui_display, 
						XDefaultRootWindow(ui_display),
						more_bits, more_width,
						more_height)); n++;
    }
    else if (strcmp(name,"multiStep") == 0) {
	XtSetArg(args[n], XtNbitmap, 
		 (Pixmap) XCreateBitmapFromData(ui_display, 
						XDefaultRootWindow(ui_display),
						step_bits, step_width,
						step_height)); n++;
    }
    else if (strcmp(name,"network") == 0) {
	XtSetArg(args[n], XtNbitmap, 
		 (Pixmap) XCreateBitmapFromData(ui_display, 
						XDefaultRootWindow(ui_display),
						network_bits, network_width,
						network_height)); n++;
    }
    else if (strcmp(name,"new") == 0) {
	XtSetArg(args[n], XtNbitmap, 
		 (Pixmap) XCreateBitmapFromData(ui_display, 
						XDefaultRootWindow(ui_display),
						new_bits, new_width,
						new_height)); n++;
    }
    else if (strcmp(name,"newL") == 0) {
	XtSetArg(args[n], XtNbitmap, 
		 (Pixmap) XCreateBitmapFromData(ui_display, 
						XDefaultRootWindow(ui_display),
						newL_bits, newL_width,
						newL_height)); n++;
    }
    else if (strcmp(name,"next") == 0) {
	XtSetArg(args[n], XtNbitmap, 
		 (Pixmap) XCreateBitmapFromData(ui_display, 
						XDefaultRootWindow(ui_display),
						next_bits, next_width,
						next_height)); n++;
    }
    else if (strcmp(name,"no") == 0) {
	XtSetArg(args[n], XtNbitmap, 
		 (Pixmap) XCreateBitmapFromData(ui_display, 
						XDefaultRootWindow(ui_display),
						no_bits, no_width,
						no_height)); n++;
    }
    else if (strcmp(name,"ok") == 0) {
	XtSetArg(args[n], XtNbitmap, 
		 (Pixmap) XCreateBitmapFromData(ui_display, 
						XDefaultRootWindow(ui_display),
						ok_bits, ok_width,
						ok_height)); n++;
    }
    else if (strcmp (name, "overwrite") == 0){  
        XtSetArg(args[n], XtNbitmap, 
		 (Pixmap) XCreateBitmapFromData(ui_display,
						XDefaultRootWindow(ui_display), 
						overwrite_bits,overwrite_width,
						overwrite_height)); n++;
    } 
    else if (strcmp (name, "plane_to_edit") == 0){  
        XtSetArg(args[n], XtNbitmap, 
		 (Pixmap) XCreateBitmapFromData(ui_display,
						XDefaultRootWindow(ui_display), 
						plane_to_edit_bits,
						plane_to_edit_width,
						plane_to_edit_height)); n++;
    } 
    else if (strcmp (name, "pos") == 0){  
        XtSetArg(args[n], XtNbitmap, 
		 (Pixmap) XCreateBitmapFromData(ui_display,
						XDefaultRootWindow(ui_display), 
						pos_bits,pos_width,
						pos_height)); n++;
    } 
    else if (strcmp(name,"prev") == 0) {
	XtSetArg(args[n], XtNbitmap, 
		 (Pixmap) XCreateBitmapFromData(ui_display, 
						XDefaultRootWindow(ui_display),
						prev_bits, prev_width,
						prev_height)); n++;
    }
    else if (strcmp(name,"print") == 0) {
	XtSetArg(args[n], XtNbitmap, 
		 (Pixmap) XCreateBitmapFromData(ui_display, 
						XDefaultRootWindow(ui_display),
						print_bits, print_width,
						print_height)); n++;
    }
    else if (strcmp(name,"printP") == 0) {
	XtSetArg(args[n], XtNbitmap, 
		 (Pixmap) XCreateBitmapFromData(ui_display, 
						XDefaultRootWindow(ui_display),
						print2_bits, display2_width,
						display2_height)); n++;
    }
    else if (strcmp(name,"projectP") == 0) {
	XtSetArg(args[n], XtNbitmap, 
		 (Pixmap) XCreateBitmapFromData(ui_display, 
						XDefaultRootWindow(ui_display),
						projection_bits, display2_width,
						display2_height)); n++;
    }
    else if (strcmp(name,"prunP") == 0) {
	XtSetArg(args[n], XtNbitmap, 
		 (Pixmap) XCreateBitmapFromData(ui_display, 
						XDefaultRootWindow(ui_display),
						pruning_bits, display2_width,
						display2_height)); n++;
    }
    else if (strcmp(name,"quitP") == 0) {
	XtSetArg(args[n], XtNbitmap, 
		 (Pixmap) XCreateBitmapFromData(ui_display, 
						XDefaultRootWindow(ui_display),
						quit_bits, display2_width,
						display2_height)); n++;
    }
    else if (strcmp(name,"reset") == 0) {
	XtSetArg(args[n], XtNbitmap, 
		 (Pixmap) XCreateBitmapFromData(ui_display, 
						XDefaultRootWindow(ui_display),
						reset_bits, reset_width,
						reset_height)); n++;
    }
    else if (strcmp(name,"save") == 0) {
	XtSetArg(args[n], XtNbitmap, 
		 (Pixmap) XCreateBitmapFromData(ui_display, 
						XDefaultRootWindow(ui_display),
						save_bits, save_width,
						save_height)); n++;
    }
    else if (strcmp(name,"select") == 0) {
	XtSetArg(args[n], XtNbitmap, 
		 (Pixmap) XCreateBitmapFromData(ui_display, 
						XDefaultRootWindow(ui_display),
						select_bits, select_width,
						select_height)); n++;
    }
    else if (strcmp(name,"set") == 0) {
	XtSetArg(args[n], XtNbitmap, 
		 (Pixmap) XCreateBitmapFromData(ui_display, 
						XDefaultRootWindow(ui_display),
						set_bits, set_width,
						set_height)); n++;
    }
    else if (strcmp(name,"setup") == 0) {
	XtSetArg(args[n], XtNbitmap, 
		 (Pixmap) XCreateBitmapFromData(ui_display, 
						XDefaultRootWindow(ui_display),
						setup_bits, setup_width,
						setup_height)); n++;
    }
    else if (strcmp (name, "shortcut_connection") == 0) {  
        XtSetArg(args[n], XtNbitmap, 
		 (Pixmap) XCreateBitmapFromData(ui_display,
						XDefaultRootWindow(ui_display), 
						shortcut_connection_bits,
						shortcut_connection_width,
						shortcut_connection_height));
	n++;
    } 
    else if (strcmp(name,"single") == 0) {
	XtSetArg(args[n], XtNbitmap, 
		 (Pixmap) XCreateBitmapFromData(ui_display, 
						XDefaultRootWindow(ui_display),
						single_bits, single_width,
						single_height)); n++;
    }
    else if (strcmp(name,"stop") == 0) {
	XtSetArg(args[n], XtNbitmap, 
		 (Pixmap) XCreateBitmapFromData(ui_display, 
						XDefaultRootWindow(ui_display),
						stop_bits, stop_width,
						stop_height)); n++;
    }
    else if (strcmp(name,"STree") == 0) {
	XtSetArg(args[n], XtNbitmap, 
		 (Pixmap) XCreateBitmapFromData(ui_display, 
						XDefaultRootWindow(ui_display),
						stree_bits, stree_width,
						stree_height)); n++;
    }
    else if (strcmp(name,"subpat") == 0) {
	XtSetArg(args[n], XtNbitmap, 
		 (Pixmap) XCreateBitmapFromData(ui_display, 
						XDefaultRootWindow(ui_display),
						subpat_bits, subpat_width,
						subpat_height)); n++;
    }
    else if (strcmp(name,"super3d") == 0) {
	XtSetArg(args[n], XtNbitmap, 
		 (Pixmap) XCreateBitmapFromData(ui_display, 
						XDefaultRootWindow(ui_display),
						super3d_bits, super3d_width,
						super3d_height)); n++;
    }
    else if (strcmp(name,"test") == 0) {
	XtSetArg(args[n], XtNbitmap, 
		 (Pixmap) XCreateBitmapFromData(ui_display, 
						XDefaultRootWindow(ui_display),
						test_bits, test_width,
						test_height)); n++;
    }
    else if (strcmp(name,"topics") == 0) {
	XtSetArg(args[n], XtNbitmap, 
		 (Pixmap) XCreateBitmapFromData(ui_display, 
						XDefaultRootWindow(ui_display),
						topics_bits, topics_width,
						topics_height)); n++;
    }
    else if (strcmp (name, "type") == 0){  
        XtSetArg(args[n], XtNbitmap, 
		 (Pixmap) XCreateBitmapFromData(ui_display,
						XDefaultRootWindow(ui_display), 
						type_bits,type_width,
						type_height)); n++;
    } 
    else if (strcmp(name,"validate") == 0) {
	XtSetArg(args[n], XtNbitmap, 
		 (Pixmap) XCreateBitmapFromData(ui_display, 
						XDefaultRootWindow(ui_display),
						valid_bits, valid_width,
						valid_height)); n++;
    }
    else if (strcmp(name,"view") == 0) {
	XtSetArg(args[n], XtNbitmap, 
		 (Pixmap) XCreateBitmapFromData(ui_display, 
						XDefaultRootWindow(ui_display),
						view_bits, view_width,
						view_height)); n++;
    }
    else if (strcmp(name,"weightsP") == 0) {
	XtSetArg(args[n], XtNbitmap, 
		 (Pixmap) XCreateBitmapFromData(ui_display, 
						XDefaultRootWindow(ui_display),
						weights_bits, display2_width,
						display2_height)); n++;
    }
    else if (strcmp(name,"yes") == 0) {
	XtSetArg(args[n], XtNbitmap, 
		 (Pixmap) XCreateBitmapFromData(ui_display, 
						XDefaultRootWindow(ui_display),
						yes_bits, yes_width,
						yes_height)); n++;
    }
    else if (strcmp(name,"zoomin") == 0) {
	XtSetArg(args[n], XtNbitmap, 
		 (Pixmap) XCreateBitmapFromData(ui_display, 
						XDefaultRootWindow(ui_display),
						zoomin_bits, zoomin_width,
						zoomin_height)); n++;
    }
    else if (strcmp(name,"zoomout") == 0) {
	XtSetArg(args[n], XtNbitmap, 
		 (Pixmap) XCreateBitmapFromData(ui_display, 
						XDefaultRootWindow(ui_display),
						zoomout_bits, zoomout_width,
						zoomout_height)); n++;
    }
    XtSetArg(args[n], XtNborderWidth, 0 /*!*/); n++;
    XtSetArg(args[n], XtNinternalHeight, 1); n++;
    XtSetArg(args[n], XtNinternalWidth , 1); n++;
    XtSetArg(args[n], XtNfromVert , top);  n++;
    XtSetArg(args[n], XtNfromHoriz, left);  n++;
    XtSetArg(args[n], XtNleft  , XtChainLeft); n++;
    XtSetArg(args[n], XtNright , XtChainLeft); n++;
    XtSetArg(args[n], XtNtop   , XtChainTop); n++;
    XtSetArg(args[n], XtNbottom, XtChainTop); n++;
    w = XtCreateManagedWidget(name, commandWidgetClass, parent, args, n);
    return(w);
}



/*****************************************************************************
  FUNCTION : ui_xCreateMenuButtonItem

  PURPOSE  : create a menu button widget
  NOTES    :
  RETURNS  : The Widgetnumber
  UPDATE   : 13.7.1990
******************************************************************************/

Widget  ui_xCreateMenuButtonItem(char *name,Widget parent,Widget left,Widget top)
{ 
    Cardinal n;
    Widget   w;
    Arg	     args[15];

    n = 0;
    XtSetArg(args[n], XtNborderWidth, 0 /*!*/); n++;
    if (strcmp(name,"color") == 0) {
	XtSetArg(args[n], XtNbitmap, 
		 (Pixmap) XCreateBitmapFromData(ui_display, 
						XDefaultRootWindow(ui_display),
						color_bits, color_width,
						color_height)); n++;
    }
    else if (strcmp(name,"editors") == 0) {
	XtSetArg(args[n], XtNbitmap, 
		 (Pixmap) XCreateBitmapFromData(ui_display, 
						XDefaultRootWindow(ui_display),
						editors_bits, editors_width,
						editors_height)); n++;
    }
    else if (strcmp(name,"windows") == 0) {
	XtSetArg(args[n], XtNbitmap, 
		 (Pixmap) XCreateBitmapFromData(ui_display, 
						XDefaultRootWindow(ui_display),
						windows_bits, windows_width,
						windows_height)); n++;
    }
    else if (strcmp(name,"icon") == 0) {
	XtSetArg(args[n], XtNbitmap, 
		 (Pixmap) XCreateBitmapFromData(ui_display, 
						XDefaultRootWindow(ui_display),
						icon_bits, icon_width,
						icon_height)); n++;
    }
    else if (strcmp(name,"show") == 0) {
	XtSetArg(args[n], XtNbitmap, 
		 (Pixmap) XCreateBitmapFromData(ui_display, 
						XDefaultRootWindow(ui_display),
						show_bits, show_width,
						show_height)); n++;
    }
    else if (strcmp(name,"mode") == 0) {
	XtSetArg(args[n], XtNbitmap, 
		 (Pixmap) XCreateBitmapFromData(ui_display, 
						XDefaultRootWindow(ui_display),
						mode_bits, mode_width,
						mode_height)); n++;
    }
    else if (strcmp(name,"options") == 0) {
	XtSetArg(args[n], XtNbitmap, 
		 (Pixmap) XCreateBitmapFromData(ui_display, 
						XDefaultRootWindow(ui_display),
						options_bits, options_width,
						options_height)); n++;
    }
    else if (strcmp(name,"use") == 0) {
	XtSetArg(args[n], XtNbitmap, 
		 (Pixmap) XCreateBitmapFromData(ui_display, 
						XDefaultRootWindow(ui_display),
						use_bits, use_width,
						use_height)); n++;
    }
    else if (strcmp(name,"del") == 0) {
	XtSetArg(args[n], XtNbitmap, 
		 (Pixmap) XCreateBitmapFromData(ui_display, 
						XDefaultRootWindow(ui_display),
						delSet_bits, delSet_width,
						delSet_height)); n++;
    }
    else if (strcmp(name,"func") == 0) {
	XtSetArg(args[n], XtNbitmap, 
		 (Pixmap) XCreateBitmapFromData(ui_display, 
						XDefaultRootWindow(ui_display),
						sel_func_bits, sel_func_width,
						sel_func_height)); n++;
    }
    else if (strcmp(name,"bignetP") == 0) {
	XtSetArg(args[n], XtNbitmap, 
		 (Pixmap) XCreateBitmapFromData(ui_display, 
						XDefaultRootWindow(ui_display),
						bignet_bits, display2_width,
						display2_height)); n++;
    }
    else{
	    XtSetArg(args[n], XtNborderWidth, 1 /*!*/); n++;
    }

    XtSetArg(args[n], XtNinternalHeight, 1); n++;
    XtSetArg(args[n], XtNinternalWidth , 1); n++;
    XtSetArg(args[n], XtNfromVert , top);  n++;
    XtSetArg(args[n], XtNfromHoriz, left);  n++;
    XtSetArg(args[n], XtNleft  , XtChainLeft); n++;
    XtSetArg(args[n], XtNright , XtChainLeft); n++;
    XtSetArg(args[n], XtNtop   , XtChainTop); n++;
    XtSetArg(args[n], XtNbottom, XtChainTop); n++;
    w = XtCreateManagedWidget(name, menuButtonWidgetClass, parent, args, n);
    return(w);
}



/*****************************************************************************
  FUNCTION : ui_xCreateToggleItem

  PURPOSE  : create a toggle widget
  NOTES    :
  RETURNS  : The Widgetnumber
  UPDATE   : 26.6.1990
******************************************************************************/

Widget  ui_xCreateToggleItem(char *name,Widget  parent,Widget  group,Widget  left,Widget  top)
{ 
    Cardinal n;
    Widget   w;
    Arg	     args[15];

    n = 0;
    if (strcmp(name,"on") == 0) {
	XtSetArg(args[n], XtNbitmap, 
		 (Pixmap) XCreateBitmapFromData(ui_display, 
						XDefaultRootWindow(ui_display),
						on_bits, on_width,
						on_height)); n++;
    }
    if (strcmp(name,"off") == 0) {
	XtSetArg(args[n], XtNbitmap, 
		 (Pixmap) XCreateBitmapFromData(ui_display, 
						XDefaultRootWindow(ui_display),
						off_bits, off_width,
						off_height)); n++;
    }
    if (strcmp(name,"value") == 0) {
	XtSetArg(args[n], XtNbitmap, 
		 (Pixmap) XCreateBitmapFromData(ui_display, 
						XDefaultRootWindow(ui_display),
						value_bits, value_width,
						value_height)); n++;
    }
    if (strcmp(name,"direction") == 0) {
	XtSetArg(args[n], XtNbitmap, 
		 (Pixmap) XCreateBitmapFromData(ui_display, 
						XDefaultRootWindow(ui_display),
						direction_bits, direction_width,
						direction_height)); n++;
    }
    if (strcmp(name,"shuffle") == 0) {
	XtSetArg(args[n], XtNbitmap, 
		 (Pixmap) XCreateBitmapFromData(ui_display, 
						XDefaultRootWindow(ui_display),
						shuffle_bits, shuffle_width,
						shuffle_height)); n++;
    }
    if (strcmp(name,"freeze") == 0) {
	XtSetArg(args[n], XtNbitmap, 
		 (Pixmap) XCreateBitmapFromData(ui_display, 
						XDefaultRootWindow(ui_display),
						freeze_bits, freeze_width,
						freeze_height)); n++;
    }
    if (strcmp(name,"connect") == 0) {
	XtSetArg(args[n], XtNbitmap, 
		 (Pixmap) XCreateBitmapFromData(ui_display, 
						XDefaultRootWindow(ui_display),
						connect_bits, connect_width,
						connect_height)); n++;
    }
    if (strcmp(name,"disconnect") == 0) {
	XtSetArg(args[n], XtNbitmap, 
		 (Pixmap) XCreateBitmapFromData(ui_display, 
						XDefaultRootWindow(ui_display),
						disconnect_bits, disconnect_width,
						disconnect_height)); n++;
    }
    if (strcmp(name,"standard") == 0) {
	XtSetArg(args[n], XtNbitmap, 
		 (Pixmap) XCreateBitmapFromData(ui_display, 
						XDefaultRootWindow(ui_display),
						standard_bits, standard_width,
						standard_height)); n++;
    }
    if (strcmp(name,"feedforward") == 0) {
	XtSetArg(args[n], XtNbitmap, 
		 (Pixmap) XCreateBitmapFromData(ui_display, 
						XDefaultRootWindow(ui_display),
						feedforward_bits, feedforward_width,
						feedforward_height)); n++;
    }
    if (strcmp(name,"net") == 0) {
	XtSetArg(args[n], XtNbitmap, 
		 (Pixmap) XCreateBitmapFromData(ui_display, 
						XDefaultRootWindow(ui_display),
						net_bits, net_width,
						net_height)); n++;
    }
    if (strcmp(name,"pat") == 0) {
	XtSetArg(args[n], XtNbitmap, 
		 (Pixmap) XCreateBitmapFromData(ui_display, 
						XDefaultRootWindow(ui_display),
						pat_bits, pat_width,
						pat_height)); n++;
    }
    if (strcmp(name,"cfg") == 0) {
	XtSetArg(args[n], XtNbitmap, 
		 (Pixmap) XCreateBitmapFromData(ui_display, 
						XDefaultRootWindow(ui_display),
						cfg_bits, cfg_width,
						cfg_height)); n++;
    }
    if (strcmp(name,"txt") == 0) {
	XtSetArg(args[n], XtNbitmap, 
		 (Pixmap) XCreateBitmapFromData(ui_display, 
						XDefaultRootWindow(ui_display),
						txt_bits, txt_width,
						txt_height)); n++;
    }
    if (strcmp(name,"printer") == 0) {
	XtSetArg(args[n], XtNbitmap, 
		 (Pixmap) XCreateBitmapFromData(ui_display, 
						XDefaultRootWindow(ui_display),
						printer_bits, printer_width,
						printer_height)); n++;
    }
    if (strcmp(name,"file") == 0) {
	XtSetArg(args[n], XtNbitmap, 
		 (Pixmap) XCreateBitmapFromData(ui_display, 
						XDefaultRootWindow(ui_display),
						file_bits, file_width,
						file_height)); n++;
    }
    if (strcmp(name,"postscript") == 0) {
	XtSetArg(args[n], XtNbitmap, 
		 (Pixmap) XCreateBitmapFromData(ui_display, 
						XDefaultRootWindow(ui_display),
						postscript_bits, postscript_width,
						postscript_height)); n++;
    }
    if (strcmp(name,"eps") == 0) {
	XtSetArg(args[n], XtNbitmap, 
		 (Pixmap) XCreateBitmapFromData(ui_display, 
						XDefaultRootWindow(ui_display),
						eps_bits, eps_width,
						eps_height)); n++;
    }
    if (strcmp(name,"dina4") == 0) {
	XtSetArg(args[n], XtNbitmap, 
		 (Pixmap) XCreateBitmapFromData(ui_display, 
						XDefaultRootWindow(ui_display),
						dina4_bits, dina4_width,
						dina4_height)); n++;
    }
    if (strcmp(name,"dina3") == 0) {
	XtSetArg(args[n], XtNbitmap, 
		 (Pixmap) XCreateBitmapFromData(ui_display, 
						XDefaultRootWindow(ui_display),
						dina3_bits, dina3_width,
						dina3_height)); n++;
    }
    if (strcmp(name,"usletter") == 0) {
	XtSetArg(args[n], XtNbitmap, 
		 (Pixmap) XCreateBitmapFromData(ui_display, 
						XDefaultRootWindow(ui_display),
						usletter_bits, usletter_width,
						usletter_height)); n++;
    }
    if (strcmp(name,"portrait") == 0) {
	XtSetArg(args[n], XtNbitmap, 
		 (Pixmap) XCreateBitmapFromData(ui_display, 
						XDefaultRootWindow(ui_display),
						portrait_bits, portrait_width,
						portrait_height)); n++;
    }
    if (strcmp(name,"landscape") == 0) {
	XtSetArg(args[n], XtNbitmap, 
		 (Pixmap) XCreateBitmapFromData(ui_display, 
						XDefaultRootWindow(ui_display),
						landscape_bits, landscape_width,
						landscape_height)); n++;
    }
    if (strcmp(name,"fixed") == 0) {
	XtSetArg(args[n], XtNbitmap, 
		 (Pixmap) XCreateBitmapFromData(ui_display, 
						XDefaultRootWindow(ui_display),
						fixed_bits, fixed_width,
						fixed_height)); n++;
    }
    if (strcmp(name,"value2") == 0) {
	XtSetArg(args[n], XtNbitmap, 
		 (Pixmap) XCreateBitmapFromData(ui_display, 
						XDefaultRootWindow(ui_display),
						value2_bits, value2_width,
						value2_height)); n++;
    }
    if (strcmp(name,"rectangle") == 0) {
	XtSetArg(args[n], XtNbitmap, 
		 (Pixmap) XCreateBitmapFromData(ui_display, 
						XDefaultRootWindow(ui_display),
						rectangle_bits, rectangle_width,
						rectangle_height)); n++;
    }
    if (strcmp(name,"circle") == 0) {
	XtSetArg(args[n], XtNbitmap, 
		 (Pixmap) XCreateBitmapFromData(ui_display, 
						XDefaultRootWindow(ui_display),
						circle_bits, circle_width,
						circle_height)); n++;
    }
    if (strcmp(name,"solid2") == 0) {
	XtSetArg(args[n], XtNbitmap, 
		 (Pixmap) XCreateBitmapFromData(ui_display, 
						XDefaultRootWindow(ui_display),
						solid2_bits, solid2_width,
						solid2_height)); n++;
    }
    if (strcmp(name,"trans") == 0) {
	XtSetArg(args[n], XtNbitmap, 
		 (Pixmap) XCreateBitmapFromData(ui_display, 
						XDefaultRootWindow(ui_display),
						trans_bits, trans_width,
						trans_height)); n++;
    }
    if (strcmp(name,"create") == 0) {
        XtSetArg(args[n], XtNbitmap, 
                 (Pixmap) XCreateBitmapFromData(ui_display,                                                 XDefaultRootWindow(ui_display),
                                                create_bits, create_width,
                                                create_height)); n++;
    }
    if (strcmp(name,"append") == 0) {
        XtSetArg(args[n], XtNbitmap, 
                 (Pixmap) XCreateBitmapFromData(ui_display, 
                                                XDefaultRootWindow(ui_display),
                                                append_bits, append_width,
                                                append_height)); n++;
    }
    if (strcmp(name,"res") == 0) {
        XtSetArg(args[n], XtNbitmap, 
                 (Pixmap) XCreateBitmapFromData(ui_display, 
                                                XDefaultRootWindow(ui_display),
                                                res_bits, res_width,
                                                res_height)); n++;
    }
    if (strcmp(name,"yes") == 0) {
        XtSetArg(args[n], XtNbitmap, 
                 (Pixmap) XCreateBitmapFromData(ui_display, 
                                                XDefaultRootWindow(ui_display),
                                                yes_bits, yes_width,
                                                yes_height)); n++;
    }
    if (strcmp(name,"no") == 0) {
        XtSetArg(args[n], XtNbitmap, 
                 (Pixmap) XCreateBitmapFromData(ui_display, 
                                                XDefaultRootWindow(ui_display),
                                                no_bits, no_width,
                                                no_height)); n++;
    }
    if (strcmp(name,"text") == 0) {
        XtSetArg(args[n], XtNbitmap, 
                 (Pixmap) XCreateBitmapFromData(ui_display, 
                                                XDefaultRootWindow(ui_display),
                                                text_bits, text_width,
                                                text_height)); n++;
    }
    if (strcmp(name,"selection") == 0) {
        XtSetArg(args[n], XtNbitmap, 
                 (Pixmap) XCreateBitmapFromData(ui_display, 
                                                XDefaultRootWindow(ui_display),
                                                selection_bits, selection_width,
                                                selection_height)); n++;
    }
    if (strcmp(name,"background") == 0) {
        XtSetArg(args[n], XtNbitmap, 
                 (Pixmap) XCreateBitmapFromData(ui_display, 
                                                XDefaultRootWindow(ui_display),
                                                background_bits, background_width,
                                                background_height)); n++;
    }
    if (strcmp(name,"norm") == 0) {
	XtSetArg(args[n], XtNbitmap, 
		 (Pixmap) XCreateBitmapFromData(ui_display, 
						XDefaultRootWindow(ui_display),
						norm_bits, norm_width,
						norm_height)); n++;
    }
    if (strcmp(name,"parallel") == 0) {
	XtSetArg(args[n], XtNbitmap, 
		 (Pixmap) XCreateBitmapFromData(ui_display, 
						XDefaultRootWindow(ui_display),
						parallel_bits, parallel_width,
						parallel_height)); n++;
    }
    if (strcmp (name, "grid") == 0) { 
	XtSetArg (args[n], XtNbitmap, 
		  (Pixmap) XCreateBitmapFromData(ui_display,
						 XDefaultRootWindow(ui_display),
						 grid_bits, grid_width, 
						 grid_height)); n++; 
    }

    XtSetArg(args[n], XtNborderWidth, 0 /*!*/); n++;
    XtSetArg(args[n], XtNradioGroup, group); n++;
    XtSetArg(args[n], XtNinternalHeight, 1); n++;
    XtSetArg(args[n], XtNinternalWidth , 1); n++;
    XtSetArg(args[n], XtNfromVert , top);  n++;
    XtSetArg(args[n], XtNfromHoriz, left);  n++;
    XtSetArg(args[n], XtNleft  , XtChainLeft); n++;
    XtSetArg(args[n], XtNright , XtChainLeft); n++;
    XtSetArg(args[n], XtNtop   , XtChainTop); n++;
    XtSetArg(args[n], XtNbottom, XtChainTop); n++;
    w = XtCreateManagedWidget(name, toggleWidgetClass, parent, args, n);
    return(w);
}



/*****************************************************************************
  FUNCTION : ui_xCreateScrollbarItem

  PURPOSE  : create a scrollbar widget
  NOTES    :
  RETURNS  : THE Widgetnumber
  UPDATE   : 2.8.1990
******************************************************************************/

Widget  ui_xCreateScrollbarItem(char *name,Widget parent,Dimension length,float topOfThumb,Widget left,Widget top)
{ 
    Cardinal n;
    Widget   w;
    Arg	     args[20];


    n = 0;
    XtSetArg(args[n], XtNborderWidth, 1); n++;
    XtSetArg(args[n], XtNlength, length); n++; 
    XtSetArg(args[n], XtNthickness, 15); n++;

    /* see X11R4, Athena, pg. 37: setting float resources */
    /* But it doesn't work ! */
  /*  if (sizeof(float) > sizeof(XtArgVal)) {
	XtSetArg(args[n], XtNtopOfThumb, &topOfThumb); n++; 
    } else {
	XtArgVal *l_top = (XtArgVal *) &topOfThumb;
	XtSetArg(args[n], XtNtopOfThumb, *l_top); n++;
    } */
    

/*    XtSetArg(args[n], XtNinternalHeight, 1); n++;
    XtSetArg(args[n], XtNinternalWidth , 1); n++;
*/
    XtSetArg(args[n], XtNfromVert , top);  n++;
    XtSetArg(args[n], XtNfromHoriz, left);  n++;
    XtSetArg(args[n], XtNleft  , XtChainLeft); n++;
    XtSetArg(args[n], XtNright , XtChainLeft); n++;
    XtSetArg(args[n], XtNtop   , XtChainTop); n++;
    XtSetArg(args[n], XtNbottom, XtChainTop); n++;
    w = XtCreateManagedWidget(name, scrollbarWidgetClass, parent, args, n);
    XawScrollbarSetThumb(w,topOfThumb,-1.0);
    return(w);
}



/*****************************************************************************
  FUNCTION : ui_noreturn

  PURPOSE  : dummy function to suppress the execution of RETURN in a Dialog
             widget
  NOTES    :
  RETURNS  : 
  UPDATE   : 07.12.1994
******************************************************************************/
static void ui_noreturn(Widget w,Widget pannel,caddr_t call_data)
{
    /* do nothing */
}



/*****************************************************************************
  FUNCTION : ui_moveup

  PURPOSE  : move the mouse pointer to the widget above the current one
  NOTES    :
  RETURNS  : 
  UPDATE   : 07.12.1994
******************************************************************************/
static void ui_moveup(Widget w,Widget pannel,caddr_t call_data)
{
    Dimension h;
    Arg       args[2];

    XtSetArg    (args[0], XtNheight, &h);
    XtGetValues (w, args, 1);
    XWarpPointer(ui_display, DefaultRootWindow(ui_display), None,
		 0, 0, 0, 0, 0, -h-4);
}


/*****************************************************************************
  FUNCTION : ui_movedown

  PURPOSE  : move the mouse pointer to the widget below the current one
  NOTES    :
  RETURNS  : 
  UPDATE   : 07.12.1994
******************************************************************************/
static void ui_movedown(Widget w,Widget pannel,caddr_t call_data)
{
    Dimension h;
    Arg       args[2];

    XtSetArg    (args[0], XtNheight, &h);
    XtGetValues (w, args, 1);
    XWarpPointer(ui_display,DefaultRootWindow(ui_display),None,0,0,0,0,0,h+4);

}


/*****************************************************************************
  FUNCTION : ui_moveleft

  PURPOSE  : move the mouse pointer to the widget to the left of the current one
  NOTES    :
  RETURNS  :
  UPDATE   : 07.12.1994
******************************************************************************/
static void ui_moveleft(Widget w,Widget pannel,caddr_t call_data)
{
    Position  nx,ny;
    Widget    leftWidget;
    int       pos=-1;
    Arg       args[4];
    Cardinal  n;

    XtSetArg    (args[0], XtNfromHoriz, &leftWidget);
    XtGetValues (w, args, 1);
    n=0;
    XtSetArg    (args[n], XtNx, &nx);n++;
    XtSetArg    (args[n], XtNy, &ny);n++;
    XtSetArg    (args[n], XtNinsertPosition, &pos);n++;
    XtGetValues (leftWidget, args, n);
    XWarpPointer(ui_display, None, DefaultRootWindow(ui_display),
		 0, 0, 0, 0, nx+8*pos, ny);
}



/*****************************************************************************
  FUNCTION : ui_moveright

  PURPOSE  : move the mouse pointer to the widget to the right of the current 
             one
  NOTES    :
  RETURNS  : 
  UPDATE   : 07.12.1994
******************************************************************************/
static void ui_moveright(Widget w,Widget pannel,caddr_t call_data)
{
    Dimension wi;
    Arg       args[2];

    XtSetArg    (args[0], XtNwidth, &wi);
    XtGetValues (w, args, 1);
    XWarpPointer(ui_display,DefaultRootWindow(ui_display),None,0,0,0,0,wi+5,0);
}



/*****************************************************************************
  FUNCTION : ui_checkWindowPosition

  PURPOSE  : checks, if the widget is completely visible an moves the
             window if necessary
  NOTES    : 
  RETURNS  : void
  UPDATE   :
******************************************************************************/

void ui_checkWindowPosition(Widget w)

{
    int                DisplayX, DisplayY;
    int                n;
    Arg                args[15];
    Dimension          WindowX, WindowY;
    Dimension          WindowWidth, WindowHeight;

#define WINDOW_HEADER 30

    /* Getting the size of the screen */
    DisplayX = DisplayWidth (ui_display, DefaultScreen(ui_display));
    DisplayY = DisplayHeight (ui_display, DefaultScreen(ui_display));
    
    /* Getting the size and position of the window */
    XtRealizeWidget(w);
    n = 0;
    XtSetArg(args[n], XtNx, &WindowX);       n++;
    XtSetArg(args[n], XtNy, &WindowY);       n++;
    XtSetArg(args[n], XtNwidth, &WindowWidth);   n++;
    XtSetArg(args[n], XtNheight, &WindowHeight);  n++;
    XtGetValues(w, args, n);

    /* Calculating the new position */
    if ((WindowWidth+WindowX) > DisplayX)  
	WindowX = DisplayX-WindowWidth;
    if (DisplayX-WindowWidth < 0)  
	WindowX = 0;
    if ((WindowHeight+WindowY+WINDOW_HEADER) > DisplayY)
        WindowY = DisplayY-WindowHeight-WINDOW_HEADER;
    if (DisplayY-WindowHeight-WINDOW_HEADER < 0)
 	WindowY = 0;

    /* Setting the new position */
    n = 0;
    XtSetArg(args[n], XtNx, WindowX);       n++;
    XtSetArg(args[n], XtNy, WindowY);       n++;
    XtSetValues(w, args, n);
}


/*****************************************************************************
  FUNCTION : ui_xDontResizeWidget

  PURPOSE  : reads the current height and width of the widget and sets it
             as min and max 
  NOTES    :
  RETURNS  : void

  UPDATE   :
******************************************************************************/

void ui_xDontResizeWidget (Widget w)

{
  int n;
  Arg args[5];
  Position x, y;

  n=0;
  XtSetArg (args[n], XtNwidth, &x);   n++;
  XtSetArg (args[n], XtNheight, &y);  n++;
  XtGetValues  (w, args,  n);
  n=0;
  XtSetArg (args[n], XtNmaxWidth, x);   n++;
  XtSetArg (args[n], XtNmaxHeight, y);  n++;
  XtSetArg (args[n], XtNminWidth, x);   n++;
  XtSetArg (args[n], XtNminHeight, y);  n++;
  XtSetValues  (w, args,  n);
}
/* end of file */



