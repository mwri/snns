/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/xgui/sources/ui_xWidgets.ph,v $
  SHORTNAME      : xWidgets 
  SNNS VERSION   : 4.2

  PURPOSE        : Private Header file of correspondent '.c' file
  NOTES          :

  AUTHOR         : Tilman Sommer 
  DATE           : 13.7.1990

  CHANGED BY     : Sven Doering
  RCS VERSION    : $Revision: 2.9 $
  LAST CHANGE    : $Date: 1998/02/25 15:22:56 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/
#ifndef _UI_XWIDGETS_DEFINED_
#define _UI_XWIDGETS_DEFINED_

/* begin global definition section */

/*Bool     ui_xIsRealized();*/
void     ui_xSetResize(Widget widget,Boolean state);
void     ui_xSetLabel(Widget widget,char *newLabel);
void     ui_xSetString(Widget widget,char *contents);
void     ui_xStringFromAsciiWidget(Widget widget,char *stringVar,int maxLen);
float    ui_xFloatFromAsciiWidget(Widget widget);
int      ui_xIntFromAsciiWidget(Widget widget);
void     ui_xGetDimensions(Widget widget,Dimension *width,Dimension *height);
Bool     ui_xGetToggleState(Widget w);
void     ui_xSetToggleState(Widget w,Bool state);
void     ui_setToggles(int index, int *new, int max, Widget widgets[]);
void     ui_xAddListEntry(struct SimpleListType *listDescriptorPtr,String string);
void 	 ui_checkWindowPosition(Widget w);

Widget   ui_xCreateLabelItem(char *name,Widget parent,Dimension width,Widget left,Widget top);
Widget   ui_xCreateFrameLabelItem(char *name,Widget parent,Dimension width,Widget left,Widget top);

Widget ui_xCreateFrameLabelItemCentered (char *name, Widget parent,
					 Dimension width,
					 Widget left, Widget top);

Widget   ui_xCreateDialogItem(char *name,Widget parent,char *contents,Dimension  width,Widget left,Widget top);     /* AsciiText string edit */
Widget   ui_xCreateButtonItem(char *name,Widget parent,Widget left,Widget top);
Widget   ui_xCreateMenuButtonItem(char *name,Widget parent,Widget left,Widget top);
Widget   ui_xCreateToggleItem(char *name,Widget  parent,Widget  group,Widget  left,Widget  top);
Widget   ui_xCreateScrollbarItem(char *name,Widget parent,Dimension length,float topOfThumb,Widget left,Widget top);
void ui_xDontResizeWidget (Widget w);


/* end global definition section */

/* begin private definition section */
static void ui_noreturn(Widget w,Widget pannel,caddr_t call_data);
static void ui_moveup(Widget w,Widget pannel,caddr_t call_data);
static void ui_movedown(Widget w,Widget pannel,caddr_t call_data);
static void ui_moveleft(Widget w,Widget pannel,caddr_t call_data);
static void ui_moveright(Widget w,Widget pannel,caddr_t call_data);

#ifdef _UNUSED_FUNCTIONS_
static void ui_addString(char **arrayPtr,int *numItems, int *allocatedItems,char *string);
#endif


#endif
/* end of file */
/* lines: 60*/




