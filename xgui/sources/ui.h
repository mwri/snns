/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/xgui/sources/ui.h,v $
  SHORTNAME      : ui.h
  SNNS VERSION   : 4.2

  PURPOSE        : general header file 
  NOTES          : This file is to include in all ui_*.c files.
                   There is another "general" include file for use of files
	            using X.

  AUTHOR         : Tilman Sommer
  VERSION        : 2.1
  DATE           : 18.5.1990

  CHANGED BY     : Sven Doering
  RCS VERSION    : $Revision: 2.15 $
  LAST CHANGE    : $Date: 1998/04/20 11:54:57 $

    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG

******************************************************************************/

#ifndef snns_ui
#define snns_ui

/* X11 */

#include <X11/StringDefs.h>
#include <X11/Intrinsic.h>



/* kernel */
#include "glob_typ.h"


/***************************** boolean ********************************/


#ifndef FALSE

#define FALSE (0==1)
#define TRUE  (0==0)

#endif

#define NOT !
#define not !
#define OR ||
#define or ||
#define AND &&
#define and &&

#define BIT_AND &
#define BIT_OR |
#define BIT_NOT ~
#define BIT_XOR ^

/***************************** integer ********************************/

#define MOD %
#define mod %
#define DIV /
/* #define div / */ /* not used */

/***************************** misc    ********************************/

#define MAX_NAME_LENGTH 4096

#define UI_ASCII_BLANK   32

/* used for testing return value of krui_setCurrent...() */
#define UI_SET_OK  0


/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/


typedef  int FlagType;
typedef  int StateType;


#ifndef Bool

typedef int Bool;

#endif

/* setup panel */
struct SetupDataType {
    Bool          showValueFlg;
    Bool          showValue;
    Bool          showTitleFlg;
    Bool          showTitle;
    Bool          showLinkFlg;
    Bool          showDirectionFlg;
    Bool          showWeightFlg;
    FlintType     linkPosTrigger, linkNegTrigger;
    FlintType     unitScaleFactor;
    FlintType     linkScaleFactor;
    char          siteName[80];
    int           backgroundColorIndex;
    int           selectionColorIndex;
    int           textColorIndex;
};

/* selection list item */
struct SelectionType {
    struct SelectionType     *nextPtr;
    struct PosType           gridPos;  /* grid position of unit */
    int                      subNetNo;
    int                      unitNo;   /* and unit no.          */
    int                      copyNo;   /* no of new created copy */
                                       /* Used during structure copy */
    int                      flags;
  	/* bit  0 - selected
	   bit  1 - reserved. not used. last operation failed for 
	            this unit. 
           bit  3 - UI_ALREADY_MOVED
	   bit  4 - UI_ALREADY_REVERSED
	   The last two bits are used to remember whether this unit was
	   already touched by a move or reverse operation. */
}; 


/* unit (info panel) */
struct UnitAttributeType {
    int             no;
    int             subnetNo;
    Bool            frozen;
    struct PosType  gridPos;
    unsigned short  layers;
    char            name[MAX_NAME_LENGTH];
    char            ftype[MAX_NAME_LENGTH];
    char            actFuncName[MAX_NAME_LENGTH];
    char            outFuncName[MAX_NAME_LENGTH];
    int             iotype;
    FlintType       act;
    FlintType       i_act;
    FlintType       out;
    FlintType       bias;
};


/* link (info panel) */
struct LinkAttributeType {
    FlintType       weight;
    FlintType       siteValue;
    char            siteName[MAX_NAME_LENGTH];
    char            siteFuncName[MAX_NAME_LENGTH];
};


struct ErrorValuesType {
    FlintType       SSE;
    FlintType       MSE;
    FlintType       SSE_div_Out;
};


/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/

/* ui_mainP.c */

#define UI_POPUP_DUMMY   1
#define UI_POPUP_SETUP   2
#define UI_POPUP_FILE    3
#define UI_POPUP_CANVAS  4
#define UI_POPUP_HELP    5
#define UI_POPUP_LIST    6
#define UI_POPUP_LAYER   7
#define UI_POPUP_EDIT    8
#define UI_POPUP_CONTROL  9
#define UI_POPUP_WEIGHTS_DONE   10
#define UI_POPUP_WEIGHTS_CANCEL 11
#define UI_POPUP_RESULT  12
#define UI_POPUP_SUBPATTERN  13
#define UI_POPUP_INFO  16

#define SEARCH_WORD_MAXLEN 40

#define UI_RANDOM 1
#define UI_JOG    2

/* mainProcs */

struct HelpType {
    Widget          shellWidget;
    Widget          textWidget;
    int             no;
    Bool            unmapped;
    struct HelpType *nextPtr;
}; 


/* lists */

struct SimpleListType {
    Widget  widgetPtr;     /* in the center of this widget the window will occur */
    Widget  listWidget;    /* contains the actual list */
    String  *listPtr;      /* pointer to a array of strings */
    int     noOfAllocItems;/* size of this array */
    int     noOfItems;     /* used items in this array */
    char    msg[100];      /* title */
    struct UnitAttributeType *unitPtr;
    int     listType;
};

/* 1 to 6 are not used in the final release ! The corresponding values
are taken from the kernel file glob_typ.h. The other constants are
starting with a value of 20 to be open for further extesions of the
kernel. */

#define UI_LIST_OUT_FUNC    1 
#define UI_LIST_ACT_FUNC    2 
#define UI_LIST_SITE_FUNC   3 
#define UI_LIST_LEARN_FUNC  4 
#define UI_LIST_UPDATE_FUNC 5 
#define UI_LIST_INIT_FUNC   6 
#define UI_LIST_PRUN_FUNC   10
#define UI_LIST_FF_LEARN_FUNC  0x0404
#define UI_LIST_REMAP_FUNC  12

#define UI_LIST_IOTYPE         20 
#define UI_LIST_SITE           21
#define UI_LIST_FTYPE_SITE     22 
#define UI_LIST_UNIT_SITE      23 
#define UI_LIST_FTYPE_OUT_FUNC 24 
#define UI_LIST_FTYPE_ACT_FUNC 25 
#define UI_LIST_FTYPE_NAME     26 
#define UI_LIST_FTYPE          27



/* layers */


#define MAX_NO_LAYERS 8


/* colors */

#define UI_COL_BLACK  0
#define UI_COL_RED    1
#define UI_COL_GREEN  2
#define UI_COL_YELLOW 3
#define UI_COL_BLUE   4
#define UI_COL_WHITE  7
#define UI_COL_BROWN  8
#define UI_COL_SEA    9

#define UI_COL_DARK   0
#define UI_COL_NORM   1
#define UI_COL_BRIGHT 2


#define UI_BLACK  56
#define UI_RED    57
#define UI_GREEN  58
#define UI_BLUE   59
#define UI_WHITE  63
#define UI_BROWN  36
#define UI_YELLOW 60
#define UI_CYAN   62
#define UI_SEA    21


#define UI_PALETTE_MONO                               0
#define UI_PALETTE_MONO_INVERSE                       1

#define ui_textColor        ui_editColor[displayPtr->setup.textColorIndex]
#define ui_selectionColor   ui_editColor[displayPtr->setup.selectionColorIndex]
#define ui_backgroundColor  ui_editColor[displayPtr->setup.backgroundColorIndex]

#define UI_MAXCOLORSTEPS 15

#define UI_MAX_EDIT_COLS 64
#define UI_VERT_COLS 8
#define UI_HORIZ_COLS 8

#define UI_SELECT_TEXT_COLOR   0
#define UI_SELECT_BACK_COLOR   1
#define UI_SELECT_SEL_COLOR    2


/* displays */


#define UI_LOCAL   1
#define UI_GLOBAL  2


#define  UI_CLOSED      0
#define  UI_OPEN        1

#define  UI_ALWAYS_FULL 1
#define  UI_INTELLIGENT 2


struct Ui_DisplayType {
    Widget                widget;          /* drawable widget */
    Window                drawable;        /* drawable window*/
    Widget                frameWidget;     /* widget containing the
					      setup button */
    int                   displayNo;       /* unique number */
    Position              x,y;             /* Window coordinates */
    Dimension             width, height;   /* Window dimensions */
    struct PosType        origin;          /* upper left corner */
    int                   gridSize;        /* in pixel */
    int                   unitsInX;
    int                   unitsInY;
    Bool                  frozen;          /* no update in this display */
    Bool                  raster;          /* draw points on positions - not used*/
    Bool                  refreshNeeded;   /* a refresh is needed 
					      Used after command: Graphic Direction.
					      move and copy actions need now a full
					      refresh */
    int                   subNetNo;        /* visible in this display */
    struct SetupDataType  setup;           /* all setup datas */
    FlagType              flags;          
    unsigned short        layers;          /* visible in this display */
    FlagType              updateType;
                          /* can be:
			        UI_ALWAYS_FULL  = draw everything
				UI_INTELLIGENT  = decide what is better
				*/
    struct Ui_DisplayType  *nextPtr;
};




/* control */

#define UI_LEARN_SINGLE  1
#define UI_LEARN_ALL     2

#define UI_NO_LEARN_PARAMS  5
#define UI_NO_UPDATE_PARAMS 5
#define UI_NO_INIT_PARAMS   5
#define UI_NO_REMAP_PARAMS   5
/* !!!!!! ATTENTION !!!!!
   When changing the following value, always make sure that 
   NO_OF_LEARN_PARAMS = UI_NO_LEARN_PARAMS + UI_NO_OF_CASCADE_PARAMS
   remains true!! NO_OF_LEARN_PARAMS is declared in kernel/sources/glob_typ.h
   Also note, that the file kernel/sources/cc_mac.h assumes that 
   NO_OF_LEARN_PARAMS >= 27 !!!!! 
*/ 
#define UI_NO_OF_CASCADE_PARAMS 23

#define UI_STEPS         1
#define UI_LEARN         2

#define UI_FIRST         1
#define UI_PREVIOUS      2
#define UI_NEXT          3
#define UI_LAST          4
#define UI_JUMP          5

/* file */


#define UI_FILE_TXT     4
#define UI_FILE_PAT     2
#define UI_FILE_NET     1
#define UI_FILE_CFG     3
#define UI_FILE_RES     5


/* setup */

#define UI_ACTIVATION         1
#define UI_INITIAL_ACTIVATION 2
#define UI_OUTPUT             3
#define UI_BIAS               4
#define UI_NUMBER             5
#define UI_NAME               6
#define UI_ZVALUE             7
#define UI_WINNER             11

#define  UI_UNIT_BOTTOM_GENERAL 1
#define  UI_UNIT_TOP_GENERAL    2
#define  UI_LINK_GENERAL        3
#define  UI_LINK_VALUE          4
#define  UI_LINK_DIRECTION      5

/* Triggers */
#define  UI_LINKS_POS        3
#define  UI_LINKS_NEG        4
#define  UI_SCALE_FACTOR     5
#define  UI_SCALE_LINKS      6


/* info */

struct UnitWidgetType {
    Widget    no;                       /* label */
    Widget    subnetNo;                 /* label */
    Widget    frozen;                   /* toggle */
    Widget    name;                     /* asciiText */
    Widget    ftype;                    /* label */
    Widget    actFuncName, outFuncName; /* label */
    Widget    iotype;                   /* label */
    Widget    act, i_act, out, bias;    /* asciiText */
};

struct LinkWidgetType {
    Widget    weight;                   /* asciiText */
    Widget    siteValue;                /* label */
    Widget    siteName;                 /* label */
    Widget    siteFuncName;             /* label */
};




struct InfoCallLayerStruct {
    Widget    widget;
    int       unitType;
};

#define UI_SOURCE   0
#define UI_TARGET   1
#define UI_DEFAULT  2




/* graphic_ui.c */

#define  UI_WHITE_COLOR 0
#define  UI_BLACK_COLOR 1
             /* pixel unset or set */

#define  UI_ERASE_BOTTOM  2  /* signal to erase the text at unit bottom */
#define  UI_DRAW          1  /* signal to draw a unit/link  */
#define  UI_ERASE         0  /* signal to erase a unit/link */

#define  UI_WEIGHT_OFF      0
#define  UI_WEIGHT_START    1
#define  UI_WEIGHT_END      2
#define  UI_WEIGHT_MIDDLE   3

/* size in pixels of icons (big = 64  and small = 16) and cursors = 16 */
#define  UI_ICON_SIZE        64
#define  UI_CURSOR_SIZE      16

#define  UI_UNIT_SIZE        16

                        /* the graphic view to a unit is max 16x16 pixels */
                        
/****************************************************************************/
/* key_ui.c */

#define  UI_INPUT              1   /* bit 1 set       */
#define  UI_OUT                2   /* bit 2 set       */
#define  UI_ALL                3   /* bit 1 and 2 set */
#define  UI_FORWARD_BINDING    4   /* bit 3           */
#define  UI_BACKWARD_BINDING   8   /* bit 4           */
#define  UI_TWOWAY_BINDING    12   /* bit 3 and 4 set */
#define  UI_ENVIRONMENT       32   /* bit 6 set       */
#define  UI_STRUCTURE_COPY    64   /* bit 7 set       */
#define  UI_SAFETY           128   /* bit 8 set       */



/****************************************************************************/
/* selection_ui.c */

#define UI_NO_FLAG           0
#define UI_SELECTED          1   /* bit 1 */
#define UI_ALREADY_MOVED     4   /* bit 3 */
#define UI_ALREADY_REVERSED  8   /* bit 4 */

/* size of the square around a unit to indicate the selection */
#define UI_SELECTION_SIZE 34
#define UI_OCCUPIED       6




/****************************************************************************/
/* ui_pix_deleteRect() */

#define UI_ALIGN      1
#define UI_NO_ALIGN   0

/****************************************************************************/
/* textwin */

#define UI_MAX_TW_BUFFER_SIZE 250

/****************************************************************************/
/* help_popup */


#define    UI_FORWARD        0
#define    UI_BACKWARD       1

#define    UI_FROM_BEGINNING 1

#define    UI_HELP_MOUSE     0
#define    UI_HELP_KEY       1
#define    UI_HELP_STATUS    2
#define    UI_HELP_FILE      3
#define    UI_HELP_HELP      4
#define    UI_HELP_TEXT      5
#define    UI_HELP_SETUP     6
#define    UI_HELP_INFO      7
#define    UI_HELP_CONTROL    8

/****************************************************************************/

/* edit */

#define  UI_EDIT_SITE              1
#define  UI_EDIT_FTYPE             2

#define  UI_EDIT_SITE_FUNC         1
#define  UI_EDIT_ACT_FUNC          2
#define  UI_EDIT_OUT_FUNC          3

/* key */

#define  UI_SET_NAME          1
#define  UI_SET_IOTYPE        2
#define  UI_SET_FTYPE         3
#define  UI_SET_ACT           4
#define  UI_SET_I_ACT         5
#define  UI_SET_OUT           6
#define  UI_SET_BIAS          7
#define  UI_SET_ACT_FUNC      8
#define  UI_SET_IACT_FUNC     9
#define  UI_SET_OUT_FUNC      10

#define  UI_START_STATE                  0
#define  UI_STATE_MODE                   1
#define  UI_STATE_UNITS                  2
#define  UI_STATE_LINKS                  3
#define  UI_STATE_FLAG                   4
#define  UI_STATE_LINKS_MAKE             5   
#define  UI_STATE_UNITS_COPY             6
#define  UI_STATE_LINKS_COPY             7
#define  UI_STATE_LINKS_DELETE           8
#define  UI_STATE_UNITS_SET              9
#define  UI_STATE_UNITS_COPY_STRUCTURE  10
#define  UI_STATE_GRAPHIC               11
#define  UI_STATE_UNITS_SET_FUNCTION    12
#define  UI_STATE_UNITS_INSERT          13
#define  UI_STATE_GETDEST               14
#define  UI_STATE_SITES                 15
#define  UI_STATE_SITES_COPY            16
#define  UI_STATE_UNITS_3D              17

/* action */


#define UI_INSERT_DEFAULT 1
#define UI_INSERT_TARGET  2
#define UI_INSERT_FTYPE   3


#define UI_ACTION_MOVE    1
#define UI_ACTION_COPY    2


#define UI_FREEZE    11
#define UI_UNFREEZE  12


/****************************************************************************/
/* update_whole_net_ui.c */

#define UI_DRAW_UNITS         1
#define UI_DRAW_LINKS         2



/****************************************************************************/



/****************************************************************************/
/* ui_print.c */


#define UI_FORMAT_POSTSCRIPT    0
#define UI_FORMAT_EPSF          1

#define UI_DEST_PRINTER         0
#define UI_DEST_FILE            1

#define UI_PAPER_DINA4          0
#define UI_PAPER_DINA3          1
#define UI_PAPER_USLETTER       2

#define UI_CLIP_ON              0
#define UI_CLIP_OFF             1

#define UI_COLOR_ON             0
#define UI_COLOR_OFF            1

#define UI_AUTOSCALE_ON         0
#define UI_AUTOSCALE_OFF        1

#define UI_AUTOUNI_ON           0
#define UI_AUTOUNI_OFF          1

#define UI_ORIENTATION_PORTAIT   0
#define UI_ORIENTATION_LANDSCAPE 1

#define UI_UNITSIZE_FIXED       0
#define UI_UNITSIZE_VALUE       1

#define UI_SHAPE_RECT           0
#define UI_SHAPE_CIRCLE         1

#define UI_TEXT_SOLID           0
#define UI_TEXT_TRANS           1

#define UI_BORDER_ON            0
#define UI_BORDER_OFF           1


typedef struct {
    int dest;                           /*  printer or file                          */
    int format;                         /*  fileformat ps or eps                     */
    int paper;                          /*  paperformat DIN A4 ...                   */
    int autoScale;                      /*  scale to maximum                         */
    int clip;                           /*  clip at border                           */
    int color;                          /*  color postscript                         */
    int orient;                         /*  landscape or portrait                    */
    int autoUni;                        /*  scale uniformly in x and y direction     */
    int size;                           /*  unit has fixed size or size by a value   */
    int shape;                          /*  unit shape is circle or rectangle        */
    int text;                           /*  text is solid or transparent             */
    int border;                         /*  draw border                              */
    int resolution;                     /*  printers resolution in dpi               */
    int displayToPrint;                 /*  number of associated display             */
    float borderHoriz;                  /*  horizontal border in cm                  */
    float borderVert;                   /*  vertical border in cm                    */
    float scaleValX;                    /*  x scale if not autoscale                 */
    float scaleValY;                    /*  y scale if not autoscale                 */
    float unitGray;                     /*  grayvalue (0..1) to fill the units       */
    char fileNameStr[MAX_NAME_LENGTH];  /*  default filename                         */
    char cmdLineStr[MAX_NAME_LENGTH];   /*  default command line                     */
} Ui_printerValType;


/****************************************************************************/


/* Macros */


/* chars */

#define ui_isCapLetter(arg)                ((arg >= 'A') && (arg<= 'Z'))
#define ui_isNonCapLetter(arg)             ((arg >= 'a') && (arg<= 'z'))



/* flags */

#define ui_utilIsSet(flag_word,flags)    ((flag_word & (flags)) == (flags))
#define ui_utilIsNotSet(flag_word,flags) ((flag_word & (flags)) != (flags))
#define ui_utilSetFlag(flag_word,flag)   (flag_word = flag_word | (flag))
#define ui_utilResetFlag(flag_word,flag) (flag_word = flag_word & (~ (flag)))


#define ui_utilGetIOTypeName(iotype)    (ui_list_iotypes[iotype])

#define ui_isUnitVisibleInDisplay(displayPtr, unitNo) ((krui_getUnitSubnetNo(unitNo) == displayPtr->subNetNo) && (krui_getUnitLayerNo(unitNo) & displayPtr->layers))


#define ui_utilAreEqualPositions(pos1,pos2)     ((pos1.x == pos2.x) && (pos1.y == pos2.y))
#define ui_utilAreDifferentPositions(pos1,pos2) ((pos1.x != pos2.x) || (pos1.y != pos2.y))



/****************************************************************************/
/****************************************************************************/
/****************************************************************************/
/****************************************************************************/






#endif


/* end of file */
/* lines: 599 */
