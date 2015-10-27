/*****************************************************************************
  FILE           : $Source: /projects/higgs1/SNNS/CVS/SNNS/xgui/sources/d3_global.h,v $
  SHORTNAME      : global.ph
  SNNS VERSION   : 4.2
 
  PURPOSE        : type definitions and global defines
  NOTES          : all global types are defined here
                   this is the only module for global variables
                   most varaibles are initilized
                   the default values could be changed here
 
  AUTHOR         : Ralf Huebner
  DATE           : 1.12.1991
 
  CHANGED BY     : Sven Doering, Niels Mache
  RCS VERSION    : $Revision: 2.6 $
  LAST CHANGE    : $Date: 1998/02/25 15:20:10 $
 
    Copyright (c) 1990-1995  SNNS Group, IPVR, Univ. Stuttgart, FRG
    Copyright (c) 1996-1998  SNNS Group, WSI, Univ. Tuebingen, FRG
 
******************************************************************************/
#ifndef _D3GLOBAL_DEFINED_
#define _D3GLOBAL_DEFINED_



#ifndef D3_GLOBAL_DEFS
#define D3_GLOBAL_DEFS


#ifndef ZERO
#define ZERO 0
#endif



#define ANZ_VECS        9
#define ANZ_LINES      12
#define ANZ_PKT_LINE    2
#define ANZ_FACETS      6
#define ANZ_PKT_FACETS  4




/*****************************************************************************
   
                             type definitions

******************************************************************************/




typedef float vector [4];         /* vector type */
typedef float matrix [4][4];      /* matrix type */
typedef vector cube [ANZ_VECS];   /* cube type   */


typedef struct {                   /* polygon            */
    int n;                         /* number of sides    */
    int mask;                      /* interpolation mask */
    vector vert[ANZ_PKT_FACETS];   /* vertices           */
} d3_polygon_type;



typedef struct {                   /* unit_mode               */
    int size;                      /* show value as size      */
    int color;                     /* show value as color     */
    int top_label;                 /* show value on top label */
    int bottom_label;              /* show value bottom label */ 
} d3_unit_mode_type;



typedef struct {                    /*  light_type                      */
    int shade_mode;                 /*  shading mode                    */
    vector position;                /*  light source position           */
    float Ia;                       /*  ambient light intensity         */
    float Ka;                       /*  ambient reflection coefficient  */
    float Ip;                       /*  point light intensity           */
    float Kd;                       /*  diffuse refection coefficient   */
} d3_light_type;





typedef struct {
    vector trans_vec, rot_vec, scale_vec;    /* translation, rotation, scale      */
    vector trans_step, rot_step, scale_step; /* translation, rotation, scale step */
    vector viewpoint;                        /* viewpoint for perspective         */
    float unit_aspect;                       /* aspect beteen the size and  space */
    float link_scale;                        /* scale for the links               */
    float pos_link_trigger;                  /* positive trigger for the links    */
    float neg_link_trigger;                  /* negative trigger for the links    */
    int font;                                /* 3D font                           */
    int projection_mode;                     /* central or parallel               */
    int model_mode;                          /* solid or wireframe                */
    int color_mode;                          /* mono or rgb                       */
    int link_mode;                           /* shows link values                 */
    d3_unit_mode_type  unit_mode;            /* shows unit values                 */
    d3_light_type light;                     /* light source                      */
} d3_state_type;


typedef struct {                             /* WINDOW */
     int x0, y0;                             /* xmin and ymin */
     int x1, y1;                             /* xmax and ymax */
} d3_window_type;


struct d3_unitPtrDef {                       /* linear list type      */ 
     int unitNo;                             /* key                   */
     struct d3_unitPtrDef *next;             /* pointer to next item  */
};


typedef struct d3_unitPtrDef d3_unitPtrType;


/*****************************************************************************
   
                             mode definitions

******************************************************************************/


/*   projection_mode   */

#define central         0
#define parallel        1



/*   model_mode  */

#define wire_frame      0
#define solid           1



/*   color_mode */

#define mono_mode       0
#define gray_mode       1
#define rgb_mode        2



/*   unit_mode   */

#define nothing_on      0
#define activation_on   1
#define init_act_on     2
#define output_on       3
#define bias_on         4
#define name_on         5
#define number_on       6
#define zvalue_on       7



/*   link_mode   */

#define links_on        0
#define links_off       1
#define links_label     2
#define links_color     3



/*   light_mode  */

#define shade_off       0
#define shade_constant  1


/*****************************************************************************
   
                                 makros

******************************************************************************/


#ifndef PI
#define PI 3.1415926
#endif

#define rad(phi)  (phi / 180.0) * PI            /* radiant to degrees   */
#define deg(phi)  (180.0 * phi) / PI            /* degrees to radiant   */

#define WAIT  {char ch; scanf ("%c", &ch);}     /* wait for a keystroke */




/*****************************************************************************
   
                            global defines

******************************************************************************/



/* X fonts */

#define d3_fontWidth 8

/* d3 fonts */

#define fnt5x8    0
#define fnt8x14   1
#define fnt5x7    2


/* transformations */ 


#define ROT_X_LEFT     0
#define ROT_X_RIGHT    1
#define ROT_Y_LEFT     2
#define ROT_Y_RIGHT    3
#define ROT_Z_LEFT     4
#define ROT_Z_RIGHT    5
#define TRANS_X_LEFT   6
#define TRANS_X_RIGHT  7
#define TRANS_Y_LEFT   8
#define TRANS_Y_RIGHT  9
#define TRANS_Z_LEFT  10
#define TRANS_Z_RIGHT 11
#define SCALE_PLUS    12
#define SCALE_MINUS   13



/* selections for unit menu */

#define select_size      1
#define select_color     2
#define select_toplabel  3
#define select_bottlabel 4



#endif


/* Xlib and Xaw Variables */

extern Display      *d3_display;
extern Window        d3_window;
extern GC            d3_gc;
extern int           d3_screen;




/* Varibles to control the Windows */

extern int d3_displayXsize;
extern int d3_displayYsize;

extern bool d3_displayIsReady;
extern bool d3_controlIsReady;

extern bool d3_freeze;

extern int d3_fontXsize, d3_fontYsize;

extern int d3_numberWidth;
extern int d3_shortNumberWidth;

extern unsigned d3_textColor;
extern float d3_intens;

extern d3_window_type d3_clipWindow;



/* the initial state structure */


extern d3_state_type  d3_state;



/* unit cube  */



extern cube d3_e_cube;



#endif 

/* end of file */
/* lines: */
