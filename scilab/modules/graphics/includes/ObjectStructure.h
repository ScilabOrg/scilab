/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2001 - 2002 - INRIA - Mathieu Philippe
 * Copyright (C) 2002 - 2004 - INRIA - Djalel Abdemouche
 * Copyright (C) 2004 - 2006 - INRIA - Fabrice Leray
 * Copyright (C) 2005 - INRIA - Jean-Baptiste Silvy
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*------------------------------------------------------------------------
 *    Graphic library 
 *    newGraph Library header
 *    This file contains all structures definitions used for New Graphics mode.
 --------------------------------------------------------------------------*/
#ifndef __SCI_OBJECT_STRUCTURE__
#define __SCI_OBJECT_STRUCTURE__



/*en fait il n'y a essentiellement besion que de Math.h dans stack-c.h
  sauf pour les callback (il faudrait creer une fonction et l'appeler) */
#include "stack-c.h"
#include "StringMatrix.h"
#include "machine.h"
#include "DoublyLinkedList.h"

#ifdef _MSC_VER
#undef Top 
#include <windows.h>
#define Top C2F(vstk).top
#endif

#ifndef _MSC_VER
#define PS_SOLID 0
#define HS_HORIZONTAL 0
#define PLANES 0
#define RASTERCAPS 0
#define RC_PALETTE 0
#define BITSPIXEL 0
#define FLOAT 0
#define TEXTMETRIC 0
#define GM_ADVANCED 0
#define MWT_IDENTITY 0
#define HORZRES 0
#define VERTRES 0
#define VERTSIZE 0
#define DEFAULT_CHARSET 0
#define HORZSIZE 0                                                     
#define MF_SEPARATOR 0
#define SW_MINIMIZE 0
#define SW_SHOWNORMAL 0
#define SB_VERT 0
#define SB_HORZ 0
#endif 
#define NUMCOLORS_SCI 32
#define SCI_FIGURE_NAME_LENGTH 256
/* Renvoi le pointeur sur la structure */

#define pFIGURE_FEATURE(pointobj)      ((sciFigure        *)pointobj->pfeatures)/** */
#define pSTATUSB_FEATURE(pointobj)     ((sciStatusBar     *)pointobj->pfeatures)/** */
#define pSUBWIN_FEATURE(pointobj)      ((sciSubWindow     *)pointobj->pfeatures)/** */
#define pTEXT_FEATURE(pointobj)        ((sciText          *)pointobj->pfeatures)/** */
#define pTITLE_FEATURE(pointobj)       ((sciTitle         *)pointobj->pfeatures)/** */
#define pLEGEND_FEATURE(pointobj)      ((sciLegend        *)pointobj->pfeatures)/** */
#define pPOLYLINE_FEATURE(pointobj)    ((sciPolyline      *)pointobj->pfeatures)/** */
#define pARC_FEATURE(pointobj)         ((sciArc           *)pointobj->pfeatures)/** */
#define pRECTANGLE_FEATURE(pointobj)   ((sciRectangle     *)pointobj->pfeatures)/** */
#define pSURFACE_FEATURE(pointobj)     ((sciSurface       *)pointobj->pfeatures)/** */
#define pLIGHT_FEATURE(pointobj)       ((sciLightSource   *)pointobj->pfeatures)/** */ 
/*#define pAXIS_FEATURE(pointobj)        ((sciAxis          *)pointobj->pfeatures)*/ /** */
#define pAXES_FEATURE(pointobj)        ((sciAxes          *)pointobj->pfeatures)/** */
#define pGRAYPLOT_FEATURE(pointobj)    ((sciGrayplot      *)pointobj->pfeatures)/** */
#define pFEC_FEATURE(pointobj)         ((sciFec           *)pointobj->pfeatures)/** */
#define pPANNER_FEATURE(pointobj)      ((sciPanner        *)pointobj->pfeatures)/** */
#define pSBH_FEATURE(pointobj)         ((sciScrollBarHorz *)pointobj->pfeatures)/** */
#define pSBV_FEATURE(pointobj)         ((sciScrollBarVert *)pointobj->pfeatures)/** */
#define pAGREG_FEATURE(pointobj)       ((sciAgreg         *)pointobj->pfeatures)/** */
#define pSEGS_FEATURE(pointobj)        ((sciSegs          *)pointobj->pfeatures)/** */
#define pLABEL_FEATURE(pointobj)       ((sciLabel         *)pointobj->pfeatures)/** */
#define pUIMENU_FEATURE(pointobj)      ((sciUimenu        *)pointobj->pfeatures)/** */
#define pUICONTEXTMENU_FEATURE(pointobj)      ((sciUicontextmenu *)pointobj->pfeatures)/** */
#define pUICONTROL_FEATURE(pointobj)   ((sciUicontrol     *)pointobj->pfeatures)/** */
#define pCONSOLE_FEATURE(pointobj)     ((sciConsole       *)pointobj->pfeatures)/** */
#define pFRAME_FEATURE(pointobj)       ((sciFrame         *)pointobj->pfeatures)/** */
#define pWINDOW_FEATURE(pointobj)      ((sciWindow        *)pointobj->pfeatures)/** */
#define pWINDOWFRAME_FEATURE(pointobj) ((sciWindowFrame   *)pointobj->pfeatures)/** */
#define pSCREEN_FEATURE(pointobj)      ((sciScreen        *)pointobj->pfeatures)/** */
#define pWAITBAR_FEATURE(pointobj)     ((sciWaitbar        *)pointobj->pfeatures)/** */
#define pPROGRESSIONBAR_FEATURE(pointobj)     ((sciProgressionbar        *)pointobj->pfeatures)/** */

#ifndef _MSC_VER
typedef unsigned short HMENU;
typedef void *HFONT;
typedef unsigned long DWORD;
#endif

typedef struct tagPOINT2D
{
  double x;
  double y;
}
POINT2D;

typedef struct tagPOINT3D
{/** */
  double x;/** */
  double y;/** */
  double z;/** */
}/**used to specifie a 3D coordinates */
POINT3D;  

/**@name sciEntityType
 * Used to determine the type of the entity
 * @memo SCI_FIGURE,
 * @memo SCI_SUBWIN,
 * @memo SCI_TEXT,
 * @memo SCI_TITLE,
 * @memo SCI_LEGEND,
 * @memo SCI_ARC,
 * @memo SCI_POLYLINE,
 * @memo SCI_RECTANGLE,
 * @memo SCI_SURFACE,
 * @memo SCI_LIGHT,
 * @memo SCI_AXIS, 
 * @memo SCI_AXES, 
 * @memo SCI_SEGS 
 * @memo SCI_GRAYPLOT, 
 * @memo SCI_FEC,
 * @memo SCI_PANNER,
 * @memo SCI_SBH,
 * @memo SCI_SBV,
 * @memo SCI_UIMENU,
 * @memo SCI_UICONTEXTMENU,
 * @memo SCI_STATUSB,
 */
typedef enum
  {/**Entity type FIGURE*/
    SCI_FIGURE,		
    /**Entity type SUBWINDOW*/
    SCI_SUBWIN,		
    /**Entity type TEXT*/
    SCI_TEXT,			
    /**Entity type TITLE */
    SCI_TITLE,		
    /**Entity type LEGEND */
    SCI_LEGEND,		
    /**Entity type ARC */
    SCI_ARC,			
    /**Entity type POLYLINE*/
    SCI_POLYLINE,	    
    /**Entity type RECTANGLE*/
    SCI_RECTANGLE,    
    /**Entity type SURFACE*/
    SCI_SURFACE,
    /**Entity type LIGHT*/
    SCI_LIGHT,		
    /**Entity type AXIS*/
    SCI_AXIS,	
    /**Entity type AXES*/
    SCI_AXES,	  
    /**Entity type SEGS*/
    SCI_SEGS,	
    /**Entity type GRAYPLOT*/
    SCI_GRAYPLOT, 
    /**Entity type FEC*/
    SCI_FEC,				
    /**Entity type PANNER*/
    SCI_PANNER,		
    /**Entity type HORIZONTALL SCROLL BAR */
    SCI_SBH,			
    /**Entity type VERTICALL SCROLL BAR*/
    SCI_SBV,			
    /**Entity type CONTEXT MENU*/
    SCI_UICONTEXTMENU,	
    /**Entity type STATUS BAR*/
    SCI_STATUSB,	    
    /**Entity type Compound */
    SCI_AGREG,			
    /**Entity type LABEL created by F.Leray 26.05.04 */
    SCI_LABEL,
    /**Entity type UIMENU created by A.C 28.09.05 **/
    SCI_UIMENU,
    /**Entity type UICONTROL **/
    SCI_UICONTROL,
    /** Entity type CONSOLE created by JB Silvy 27/02/07 */
    SCI_CONSOLE,
    /** Entity type FRAME created by JB Silvy 27/02/07 */
    SCI_FRAME,
    /** Entity type WINDOW created by JB Silvy 27/02/07 */
    SCI_WINDOW,
    /** Entity type WINDOWFRAME created by JB Silvy 27/02/07 */
    SCI_WINDOWFRAME,
    /** Entity type SCREEN created by JB Silvy 27/02/07 */
    SCI_SCREEN,
    /** Entity type WAITBAR */
    SCI_WAITBAR,
    /** Entity type PROGRESSIONBAR */
    SCI_PROGRESSIONBAR
  }
/**Struct of Entity type*/
sciEntityType;	

typedef struct _Vertices 
{
  int value_xm;
  int value_ym;
  double value_x;
  double value_y;
  double value_z;
  
  struct _Vertices * pNext;
} 
Vertices;


/**@name sciPointObj
 * Used to determine the feature and the type of the entity
 */
typedef struct
{/** */
  sciEntityType entitytype;
  /**  points to the characteristic of the structure (figure, axes...) */
  void * pfeatures;
  /** List of all observers watching this object */
  DoublyLinkedList * pObservers ;
  /** Drawing object used to actually draw the graphic handle */
  void * pDrawer ;
}/** */
sciPointObj;  


/**@name sciHandleTab
 * Used to determine handles associated with the entity
 */
typedef struct tagHandleTab
{/** point to the structure that uses this handle */
  sciPointObj *pointobj;	
  /** */
  long index;
  /** */
  struct tagHandleTab *pprev;
  /** */
  struct tagHandleTab *pnext;
}/** */
sciHandleTab; 


/**@name Sons
 * Used to determine the hierarchy
 sciPointObj *pointobj;
 sciSons     *pnext;
*/
typedef struct tagSons
{
  /** */
  struct tagSons *pprev;	  
  /**  is the pointer of next the son's structure      */
  sciPointObj *pointobj;	
  /** */
  struct tagSons *pnext;
}
/** */
sciSons;  


/**@name RelationShip
 * Used to determine the hierarchy
 */
typedef struct
{
  /** is the scilab handle of THIS */
  sciHandleTab * phandle;       
  /** points to the parent structures */
  sciPointObj * pparent;	       
  /** points to the sons structures */
  sciSons * psons;	       
  /** the last sciSons of the list (the first created!) */
  sciSons * plastsons;
  /** the set of selected sons. List of lists for each type. Not to be used directly.*/
  DoublyLinkedList * pSelectedSon ;
}/** */
sciRelationShip;  



/**@name GraphicContext
 * Used to know what are the contexte value attached with the Graphic area  
 * 
 * LineStyle:
 * PS_SOLID The pen is solid. 
 * PS_DASH The pen is dashed. This style is valid only when the pen width is one 
 *         or less in device units. 
 * PS_DOT The pen is dotted. This style is valid only when the pen width is one 
 *         or less in device units. 
 * PS_DASHDOT The pen has alternating dashes and dots. This style is valid only 
 *         when the pen width is one or less in device units. 
 * PS_DASHDOTDOT The pen has alternating dashes and double dots. This style is 
 *         valid only when the pen width is one or less in device units. 
 * PS_NULL The pen is invisible. 
 * PS_INSIDEFRAME The pen is solid. When this pen is used in any GDI drawing 
 *         function that takes a bounding rectangle, the dimensions of the 
 *         figure are shrunk so that it fits entirely in the bounding rectangle, 
 *         taking into account the width of the pen. This applies only to geometric pens. 
 */
typedef struct
{
  /** currently not used in this version */
  /** pointe sur un ID de la table des couleur Scilab */
  int backgroundcolor;	    
  /** pointe sur un ID de la table des couleur Scilab */
  int foregroundcolor;
  /** */
  int fillcolor;
  /** Wheter the object is filled or not */
  BOOL isfilled;
  /** associeted to PEN or mark size */
  int linewidth;
  /** */
  BOOL isline;
  /** */
  int linestyle;
  /** */
  BOOL ismark;
  /** */
  int markstyle;
  /** */
  int marksize;
  /** */
  int marksizeunit;
  /** */
  int markbackground;
  /** */
  int markforeground;
}/** */
sciGraphicContext;  


/*----------------------
  SCIFONT.H header file no more used
  ----------------------*/

/* Changing those lines, be carreful that
   the new attribut is update in the sciset and sciget routines
*/
/** */
#define SCI_DONT_CARE          0
/** */
#define SCI_ATTR_BOLD          1
/** */
#define SCI_ATTR_ITALIC        2
/** */
#define SCI_ATTR_UNDERLINE     4
/** */
#define SCI_ATTR_STRIKEOUT     8


/**@name sciFont
 * Used to know what are the contexte value attached with the Graphic area  
 */
typedef struct
{
  /** currently not used in this version */
  /* il faudrait prendre la font windows */
  /** pointe sur un ID de la table des couleur Scilab */
  int backgroundcolor;		
  /** pointe sur un ID de la table des couleur Scilab */
  int foregroundcolor;		
  /** pointe sur le nom de la fonte  */
  char *pfontname;		 
  /** renvoi la longueur de fontname */
  unsigned int fontnamelen;	
  /** type of the font */
  int fonttype; /* scilab font index 0 to 9 */
  /** specify if the font size must be stucks to Scilab font sizes 0:6 or might be any double */
  BOOL useFractionalMetrics;
  /** Gets the width of the character in tenth of point */
  double fontSize;		
  /** this is coded in radian*/
  double textorientation;		
}/** */
sciFont;  


/**@name scigMode
 * Used to determine all the mode that can be used. Only some entity can sets this itself
 */
typedef struct
{ 
  /** Are new Plot added to the old plot on the graphics window        */
  BOOL addplot;	       
  /** The min max ranges of the the graphics window is given by users  */
  BOOL autoscaling;    
  /** Is zooming allowed                                               */
  BOOL zooming;   
  /** drawing xor mode                                                         */
  int xormode;
  	       	       
  
}/** */
scigMode;


/**
 * @name Range
 * Structure used to specify Range
 */
typedef struct
{/** */
  double xmin;
  /** */
  double xmax;
  /** */
  double ymin;
  /** */
  double ymax;
  /** */
  double zmin;
  /** */
  double zmax;
}/** */
sciRange;  


/**
 * FigureData only used by figure model
 */
typedef struct
{
  int figureWidth ;
  int figureHeight ;
  int windowWidth ;
  int windowHeight ;
  int windowPosition[2];
  int viewport[4]; /* 4 but for now only two are used */
  double * colorMap ;
  int numColors ;
  BOOL autoResizeMode;
}
FigureModelData ;

/**
 * Structure used to specify Figure (different to XGC) 
 */
typedef struct
{	
  sciRelationShip relationship;
  scigMode gmode;
  sciGraphicContext graphiccontext; /* the only property used here is background */
  sciPointObj * originalsubwin0011;
  /** specifies the title for this window  */
  char name[SCI_FIGURE_NAME_LENGTH];	 
  /** specifies le length of the string name */
  int namelen;
  /** specifies the number of this window            */
  int number;

  int numcolors;

  /** specifies if this window is iconified*/
  BOOL isiconified;		     
  /** specifies if this window is selected             */
  BOOL isselected; 
  int rotstyle;
  /** specifies if this object is visble             */
  BOOL visible;
  BOOL auto_redraw;
  /** specifies the number of the selected son         */
  int numsubwinselected;
  /** specifies the current pixmap status         */
  int pixmapMode ;
  
  BOOL allredraw;
  
  sciFont fontcontext; /* F.Leray 08.04.04 */
  char * infoMessage ; /**< String displayed in the info bar of the graphic window */

  char * eventHandler         ; /**< Name of the EventHandler function */
  BOOL   isEventHandlerEnable ; /**< flag enabling or disabling eventhandler */
  
  FigureModelData * pModelData ; /**< To be used by figure model */

  int * user_data; /* adding 27.06.05 */
  int size_of_user_data;
  
  char * tag;
}/** */
sciFigure;  

typedef enum { ALIGN_NONE = 0, ALIGN_LEFT = 1, ALIGN_CENTER = 2, ALIGN_RIGHT = 3 } sciTextAlignment ;

/**@name Text
 * Structure used to specify Text
 */
typedef struct
{
  /** */
  sciRelationShip relationship;
  /** */
  sciFont fontcontext;
  sciGraphicContext graphiccontext; /* the only properties used by Text are foreground and background */
  
  /** the displayed text */
  StringMatrix * pStrings ;

  /** position inside the axes (not pixel) */
  double x ;
  /** position inside the axes (not pixel) */
  double y ;
  double z ;

  BOOL is3d ; /**< If FALSE the text will be displayed using the 2d scale even if its parent
               *   subwindow is in 3d mode. */

  BOOL autoSize ; /* to know wether the size of the displayed array is specified by user or */
                   /* automatically computed. */

  double userSize[2] ; /**< Width and height of the displayed string array defined by user */
  
  sciTextAlignment stringsAlign ; /**< Alignment of the strings inside the array */
  
  BOOL centeredPos ; /**< to know wether the (x,y) position is the point in the middle of the
                        string or the lower-left point */
  BOOL isboxed  ;
  /** */
  /** specifies the text scilab code for the callback associated with this entity */
  char *callback; 
  /** the length of the callback code */
  int callbacklen; 
  int callbackevent;

  double corners[4][3]; /**< position of the text bounding box corners */

  /** specifies if this object is visble             */
  BOOL visible;
  int isclip;
  double clip_region[4];
  int clip_region_set;
  
  int * user_data; /* adding 27.06.05 */
  int size_of_user_data;

}
sciText;  




/**@name sciLegendPlace
 * Enumeration used to specify the title place 
 */
typedef enum
  {
    /** */
    /** */
    SCI_LEGEND_IN_UPPER_LEFT = 1,
    /** */
    SCI_LEGEND_IN_UPPER_RIGHT = 2,
    /** */
    SCI_LEGEND_IN_LOWER_LEFT = 3,
    /** */
    SCI_LEGEND_IN_LOWER_RIGHT = 4,
    /** */
    SCI_LEGEND_BY_COORDINATES = 5,
    /** */
    SCI_LEGEND_OUT_UPPER_LEFT = -1,
    /** */
    SCI_LEGEND_OUT_UPPER_RIGHT = -2,
    /** */
    SCI_LEGEND_OUT_LOWER_LEFT = -3,
    /** */
    SCI_LEGEND_OUT_LOWER_RIGHT = -4,
    /** */
    SCI_LEGEND_UPPER_CAPTION = -5,
   /** */
    SCI_LEGEND_LOWER_CAPTION = -6
  }  /** */
sciLegendPlace;



/**@name Legend
 * Structure used to specify Legend 
 */
typedef struct
{
  sciGraphicContext graphiccontext; /* used to draw the line and marks of the curve F.Leray 21.01.05 */
  /** */
  int nblegends;
  /** */
  int *pstyle;
  /** */
  sciText text;
  /** */
  POINT2D pos;
  /** specifies the frame's width */
  int width;	     
  /** specifies the frame's height */
  int height;	     
  /** */
  BOOL isselected;
  /** specifies if the legend is surrounded */
  BOOL issurround;	     
  /* void    *associetedentity;  get the associated entity by asking parents.relation.sons */
  /* the subwindow dimension is get by asking relation     */
  sciLegendPlace place;
  /** pointer on the labelled objects */
  long long *tabofhandles;
  /** specifies if this object is visble             */
  BOOL visible; 
  int isclip;
  double clip_region[4];
  int clip_region_set;
  int * user_data; /* adding 27.06.05 */
  int size_of_user_data;
}/** */
sciLegend;  


/**@name sciTitlePlace
 * Enumeration used to specify the title place 
 */
typedef enum
  {
    /** */
    SCI_TITLE_IN_TOP = 0,
    /** */
    SCI_TITLE_IN_BOTTOM = 1
  }/** */
sciTitlePlace;	


/**@name Titre
 * Structure used to specify Texte
 */
typedef struct
{
  /** */
  sciText text;
  /** absolut position in subindow*/
  POINT2D pos;
  int ptype;
  /** up, down */
  sciTitlePlace titleplace;
  /** */
  BOOL isselected;
  /** specifies if this object is visble             */
  BOOL visible;
  int isclip;
}/** */
sciTitle;

/**@name Titre
 * Structure used to specify Labels like Title or classic labels
 */
typedef struct
{
  /* sciRelationShip relationship; */
  /** contains the data to display the label */
 /*  sciText text; */
  sciPointObj * text ;
  /** absolut position in subindow*/

  int ptype ; /**< to distinguish between title, x_label,.... */

  /** */
  BOOL isselected;
  BOOL auto_position; /* automatic or manual position selection for label */
  BOOL auto_rotation; /* automatic or manual rotation selection for label (depends on the current view mode 2d or 3d mainly for y and z labels) */
                      /* not implemented for titles */

}
sciLabel ;

typedef struct
{
  sciRelationShip relationship;

  /* Color property */
  double *foregroundcolor;

  /* Callback property */
  char *callback;
  int callbackType;

  /* Specifies if this object is visible in its parent children */
  BOOL handle_visible;

  int MenuPosition;

  /* Userdata property */
  int * user_data;
  int size_of_user_data;

  /* Index in the java UIElementMapper */
  int hashMapIndex;

  /* Tag property */
  char * tag;

}/** */
sciUimenu;

typedef struct
{
  sciRelationShip relationship;
  int hashMapIndex;
}/** */
sciUicontextmenu;

typedef struct
{
  sciRelationShip relationship;
  int hashMapIndex;
}/** */
sciWaitbar;

typedef struct
{
  sciRelationShip relationship;
  int hashMapIndex;
}/** */
sciProgressionbar;

typedef struct
{
  sciRelationShip relationship;

  /* Specifies if this object is visible in its parent children */
  BOOL handle_visible;

  /* Colors property */
  double *backgroundcolor;
  double *foregroundcolor;

  /* Callback property */
  char *callback;
  int callbackType;

  /* Userdata property */
  int * user_data;
  int size_of_user_data;

  /* Index in the java UIElementMapper */
  int hashMapIndex;

  /* ListboxTop property */
  int * listboxTop;

  /* Value property */
  int * value;
  int valueSize;

  /* Style property: Must be a value from UicontrolStyle enum */
  int style;

  /* FontAngle property: Must be a value from UicontrolFontProperties enum */
  int fontAngle;

  /* FontWeight property: Must be a value from UicontrolFontProperties enum */
  int fontWeight;

  /* FontUnits property: Must be a value from UicontrolUnits enum */
  int fontUnits;

  /* Units property: Must be a value from UicontrolUnits enum */
  int units;

  /* Relief property: Must be a value from UicontrolRelief enum */
  int relief;

  /* Horizontalalignment property */
  int horizontalAlignment;

  /* Verticalalignment property */
  int verticalAlignment;

  /* Min/Max properties */
  int min;
  int max;

  /* SliderStep property */
  double * sliderStep;

  /* Tag property */
  char * tag;
}/** */
sciUicontrol;

/* uicontrol font properties */
typedef enum {LIGHT_FONT, NORMAL_FONT, DEMI_FONT, BOLD_FONT, ITALIC_FONT, OBLIQUE_FONT} UicontrolFontProperties ;
/* uicontrol style */
typedef enum {
  SCI_PUSHBUTTON,
  SCI_RADIOBUTTON,
  SCI_CHECKBOX,
  SCI_EDIT,
  SCI_UITEXT,
  SCI_SLIDER,
  SCI_UIFRAME,
  SCI_LISTBOX,
  SCI_POPUPMENU
} UicontrolStyle ;
/* uicontrol units */
typedef enum {POINTS_UNITS, NORMALIZED_UNITS, INCHES_UNITS, CENTIMETERS_UNITS, PIXELS_UNITS} UicontrolUnits ;
/* uicontrol relief */
typedef enum {FLAT_RELIEF, GROOVE_RELIEF, RAISED_RELIEF, RIDGE_RELIEF, SOLID_RELIEF, SUNKEN_RELIEF} UicontrolRelief;
/* uicontrol alignment */
typedef enum {LEFT_ALIGNMENT, CENTER_ALIGNMENT, RIGHT_ALIGNMENT, TOP_ALIGNMENT, MIDDLE_ALIGNMENT, BOTTOM_ALIGNMENT} UicontrolAlignment;

/* the part of the drawn box for 3d axis */
typedef enum { BT_OFF = FALSE, BT_ON = TRUE, BT_HIDDEN_AXIS, BT_BACK_HALF } EAxesBoxType ;

typedef struct 
{  
  double  xlim[4];  /* [xmin,xmax,ar,nint]           */ /* F.Leray 21.09.04 : NOUVEAU sens pour xlim,ylim,zlim: NON! Comme avant valeurs en tight limits on/off */ /* F.Leray 07.10.04 */
  double  ylim[4];  /* [ymin,ymax,ar,nint]           */ /* pour afficher les graduations automatiques on a calcule des xyzgrads provenant de TheTicks */
  double  zlim[4];  /* [zmin,zmax,ar,nint]         */ 

  /* tics data from algo */
  double  xgrads[20], ygrads[20], zgrads[20];                   /* Here they are  */
  int nxgrads, nygrads, nzgrads;                         /* with their size <=> nber of tics */

  /* tics data from user (=> u_...)*/
  double *u_xgrads, *u_ygrads, *u_zgrads;                       /* Here they are  */
  int u_nxgrads, u_nygrads, u_nzgrads;                   /* with their size <=> nber of tics */
  
  int  ticscolor;
  /*  int  fontsize;
      int  textcolor;*/
  sciFont fontcontext;
  
  int  subint[3]; /* Dj.A 17/12/03 */
  EAxesBoxType  rect ; /**< to know the part of the box we have to draw */
  char xdir;   /**<  xdir  = 'u' | 'd'  : gives the xy-axes positions */ 
  char ydir;   /**<  ydir  = 'r' | 'l' : gives the xy-axes positions */ 
 

  /* flags for switching from auto to manual ticks */
  BOOL auto_ticks[3]; /* if on, it means that the ticks are taken from computation (see theticks algo. by Francois D.) */
  
  char **u_xlabels,  **u_ylabels,  **u_zlabels; /* label string corresponding to each specified u_xyzgrads */
    
  int hiddenAxisColor ; /* the color and style of the hidden axis */

  double  limits[7]; /* = 1 set tight limits = 0 set axes auto shape */
  integer flag[3]; /* 3d options */
  
  /* F.Leray 07.10.04 REMOVE AAINT*/
  int nbsubtics[3]; /* Don't touch to subint because also deals with drawaxis: AXES structure has multiple uses... */ /* F.Leray 07.10.04 */

  BOOL reverse[3]; /* if TRUE, it means that the corresponding axe is reversed */
  BOOL axes_visible[3]; /* if TRUE, it means that the corresponding axes is drawn */
  
}
AXES; 

/**@name SubWindow
 * Structure used to specify SubWindow wich is a subplot
 */
typedef struct
{
  AXES axes;
  /** */
  sciRelationShip relationship;
  /** */
  scigMode gmode;
  /** */
  sciGraphicContext graphiccontext;
  /** */
  /** specifies the title for this window  */
  char name[sizeof ("ScilabGraphic") + 4];	    
  /** */
  int namelen;
  /** specifies the number for this window                   */
  int number;			
  /** specifies if this subwindow is selected                */
  /*BOOL isselected;*/			 
  /** specifies the position in the parent figure            */
  int infigureposx;			 
  /** specifies the position in the parent figure            */
  int infigureposy;			 
  /** specifies the width of the subplot                     */
  int windimwidth;		       
  /** specifies the width of the subplot                     */
  int windimheight;		      
  /** specifies the limite of the plot                       */
  sciRange range;			 
  /** specifies the factor of the x zoom                     */
  int zoomx;				 
  /** specifies the factor of the y zoom                     */
  int zoomy;
  double SRect[6]; /* [xmin xmax ymin ymax zmin zmax] : Strict rect. coming from update_specification_bounds function or from a set(a,"data_bounds",[...]) */
  double FRect[6]; /* real data_bounds */
  double WRect[4]; /* size of the subplot */
  double ARect[4]; /* margins, give the size of the axe*/
  int zoomy_kp;
  /* ZRect_kp is now useless : when unzooming we deal with SRect values */
  double ZRect[6]; /* reversed for zoom only to avoid using FRect as zoom box AND computed box */ /* F.Leray 09.12.04 */

  char logflags[3]; /* Z has a logflag now F.Leray 03.11.04 */
  int grid[3];
  /*   BOOL isaxes; */

  BOOL is3d;
  BOOL tight_limits;
  double theta_kp;
  double alpha_kp;
  /* viewing angles */
  double theta;
  double alpha;	
  /** specifie the associated entity popup menu */
  sciPointObj *pPopMenu;    
  /** specifies the text scilab code for the callback associated with this entity */
  char *callback; 
  /** the length of the callback code */
  int callbacklen;  
  int callbackevent; 
  /** specifies if this object is visble             */
  BOOL visible;

  BOOL useNurbsForArcs;

  int isclip;
  double clip_region[4];
  int clip_region_set;
  /**DJ.Abdemouche 2003**/
  integer project[3];
  BOOL isoview; 
  int hiddencolor;
  int hiddenstate;
  int with_leg; /* Adding F.Leray 07.05.04 */ /* for strflag[0] support : not needed today */
  BOOL cube_scaling; /* Matlab like view in 3D when one or two range is/are preferential */
  BOOL FirstPlot; /* An internal state used to indicated that high level functions must not use SRect*/

  /* F.Leray 25.04.05 Labels in sciSubWin*/
  sciPointObj * mon_title;
  sciPointObj * mon_x_label;
  sciPointObj * mon_y_label;
  sciPointObj * mon_z_label;

  /*  An internal state used as subtics flag (auto == FALSE, TRUE == manual) named flagNax */
  BOOL flagNax;
  int * user_data; /* adding 27.06.05 */
  int size_of_user_data;
  
  int YGradMostOnLeft;
  int YGradMostOnRight;
  int XGradMostOnTop;
  int XGradMostOnBottom;

  BOOL firsttime_x;
  BOOL firsttime_y;

  Vertices * vertices_list; /* F.Leray 30.08.05 : stores the (x,y) coord. in term of user data coord. + pixel value on screen */
}/** */
sciSubWindow;  


/**@name Arc
 * Structure used to specify 
 * @author Matthieu PHILIPPE /MAJ D.ABDEMOUCHE
 * @version 0.1
 * @args 
 * @return 
 * @see 
 */
typedef struct
{
  /** */
  sciRelationShip relationship;
  /** */
  sciGraphicContext graphiccontext;
  /** */
  double x;
  /** */
  double y;
  /** */
  double width;
  /** */
  double height;
  /** begin at alpha1 */
  double alphabegin;		
  /** end at alpha2   */
  double alphaend;
  /** */
  BOOL isselected;
  /** specifies the text scilab code for the callback associated with this entity */
  char *callback; 
  /** the length of the callback code */
  int callbacklen;   
  int callbackevent;
  /** specifies if this object is visble             */
  BOOL visible;
  int isclip;
  double clip_region[4];
  int clip_region_set;
  double z; /**DJ.Abdemouche 2003**/

  BOOL useNurbs; /* Specify whether the arc should be drawn using nurbs or a set of lines */

  int * user_data; /* adding 27.06.05 */
  int size_of_user_data;
  /* the isline property is inside the associated graphiccontext */
}  /** */
sciArc;


/**@name listPoints
 * Structure used to specify 
 */
typedef struct taglistPoints
{
  /** */
  double x;
  /** */
  double y;
  /** */
  struct taglistPoints *pnextpoints;
}  /** */
sciListPoints;


/**@name Polyline
 * Structure used to specify 
 * @author Matthieu PHILIPPE /MAJ D.ABDEMOUCHE
 * @version 0.1
 * @args 
 * @return 
 * @see 
 */
typedef struct
{
  sciRelationShip relationship;
  sciGraphicContext graphiccontext;
  double *pvx;			/* vecteur des points x doublon avec pvector je les garde pour compatiblite*/
  double *pvy;			/* vecteur des points y doublon avec pvector*/
  double *pvz; /**DJ.Abdemouche 2003**/
  int n1;                       /** number of points, for interp vector shoud be 3 or 4 */
  int closed;                   /** is it a closed polyline */
  int plot;                     /** defines the polyline_style (interpolated, staircase, bar_plot,...) : is it simple poly or a plot (Plot2d /Plot2d1/.../Plot2d4) */
  BOOL isselected;
  char *callback;		/** specifies the text scilab code for the callback associated with this entity */
  int callbacklen;		/** the length of the callback code */  
  int callbackevent;
  /** specifies if this object is visble             */
  BOOL visible;
  int isclip;
  double clip_region[4];
  int clip_region_set;
  int * user_data; /* adding 27.06.05 */
  int size_of_user_data;
  BOOL isinterpshaded;
  int *scvector; /* interpolation vector */
  int bar_layout;

  double * x_shift;
  double * y_shift;
  double * z_shift;
  
  double bar_width;
  
  double arsize_factor; /* to be able to enlarge arrow size without changing the line thickness */
}
sciPolyline;  /** */


/**@name Rectangle
 * Structure used to specify 
 */
typedef struct
{
  sciRelationShip relationship;
  sciGraphicContext graphiccontext;
  double x;			   /** original */
  double y;			   /** original */
  double width;
  double height;
  int str; 
  integer strwidth;
  integer strheight;
  double horzcurvature;		  /** to do rectangle with round corner */ /*F.Leray not implemented till now... 19.03.04 to see...*/
  double vertcurvature;		  /** to do rectangle with round corner */ /*F.Leray not implemented till now... 19.03.04 to see...*/
  BOOL isselected;
  char *callback; /** specifies the text scilab code for the callback associated with this entity */
  int callbacklen; /** the length of the callback code */  
  int callbackevent;
  /** specifies if this object is visble             */
  BOOL visible;
  int isclip;
  double clip_region[4];
  int clip_region_set;
  double z;  /** rectangle */

  BOOL flagstring; /* flag used to determine wether the rectangle is used to surround a string : used when axes is reversed */
  int * user_data; /* adding 27.06.05 */
  int size_of_user_data;
}
sciRectangle;  /** */


typedef enum
  {
    SCI_FAC3D = 0,
    SCI_PLOT3D = 1,
    SCI_CONTOUR,
    SCI_PARAM3D,
    SCI_PARAM3D1
  }
sciTypeOf3D;


/**@name Surface
 * Structure used to specify 
 */
typedef struct
{
  sciRelationShip relationship;
  sciGraphicContext graphiccontext;
  double * pvecx; /* x data */
  double * pvecy; /* y data */
  double * pvecz; /* z data */
  double *inputCMoV; /* Adding here in order to always have the input data*/
  /* We will determinate if it is a vector or matrix data with m3n and n3n values*/
  /* temporary variable, useless for drawing */
  /* name means input color or vector */
  double *zcol; /* temporary variable, useless for drawing */
  double *color;  /* Final matrix used only for drawing :  */
                  /*  color == zcol if 'direct' mode used (direct indices on colormap)  */
                  /* else 'scaled' mode used => color == linear interp. of zcol on the colormap */
                  /* if flag_color = 0 or 1, color is null */
                  /* if flag_color = 2, color is a vector */
                  /* if flag_color = 3, color is a matrix */
                  /* if flag_color = 4, color is a vector */

  int cdatamapping; /* like in Matlab, it determines how the color is computed ('scaled' or 'direct' mode) */
                    /* 0: scaled */
                    /* 1: direct (default) */
  integer izcol; /* 0 : no color; 1 : color is a vector, 2 : color is a matrix */
  integer dimzx; /* == m3 */
  integer dimzy; /* == n3 */

  /*F.Leray 12.03.04 Adding here to know the length of arrays pvecx, pvecy and pvecz*/

  int nc; /* number of colors = m3n * n3n */
  int nx; /* number of x data */
  int ny; /* number of y data */
  int nz; /* number of z data */
  int isfac; /* redondant with typof3d */
  int m1,n1; /* size of pvecx */
  int m2,n2; /* size of pvecy */
  int m3,n3; /* size of pvecz */
  int m3n,n3n; /* size of color */

  integer flag[3]; /* only the first value is used as color_mode */
  double ebox[6]; /* apparently unused */
  int flagcolor; /* this flag indicates the type of the color of how the facet have to be colored 
		    0: uniformed color
		    1: facet's color are computed with z*/ /* in case of a simple plot...!!! F.Leray 19.03.04 */
  sciTypeOf3D typeof3d; /* Fac3d or Plot3d */
  int hiddencolor;
  BOOL isselected;
  char *callback; /** specifies the text scilab code for the callback associated with this entity */
  int callbacklen; /** the length of the callback code */  
  int callbackevent;
  /** specifies if this object is visble             */
  BOOL visible;

  int flag_x; /* 1 if the vector is increasing, -1 if decreasing. */
              /* The vector should be either increasing or decreasing. */
  int flag_y;

  int isclip; /**< Clipping state of the object */
  double clip_region[4]; /**< Clipping region */
  int clip_region_set; /**< To know if the clippign region is set */
  
  int * user_data; /* adding 27.06.05 */
  int size_of_user_data;
}
sciSurface;  /** */


/**@name LightSource
 * Structure used to specify 
 */
typedef struct
{
  sciRelationShip relationship;
  sciGraphicContext graphiccontext;
  POINT3D org;
  BOOL switchon;		/* on or off */
  /** specifies if this object is visble             */
  BOOL visible;
  int isclip;
  double clip_region[4];
  int clip_region_set;
  int * user_data; /* adding 27.06.05 */
  int size_of_user_data;
}
sciLightSource;	 /** */


/**@name Axis
 * Structure used to specify axis feature 
 */
/* NOT USED IN FACT */
/* typedef struct */
/* { */
/*   sciRelationShip relationship; */
/*   sciGraphicContext graphiccontext;  */
/*   sciText text; */
/*   char strflag[4]; */
/*   int strflaglen; */
/*   double aaint[4]; */
/*   double minx; */
/*   double miny; */
/*   double minz; */
/*   double maxx; */
/*   double maxy; */
/*   double maxz; */
/*   int styledimension;		/\* 2=2d 3=3d *\/ */
/*   int stylecrossing; */
/*   double orgcrossing; */
/*   char *plabelx; */
/*   char *plabely; */
/*   char *plabelz; */
/*   BOOL manualscale; */
/*   BOOL plotit; */
/*   BOOL isselected; */
/*   int grid; */
/*   char *callback; /\** specifies the text scilab code for the callback associated with this entity *\/ */
/*   int callbacklen; /\** the length of the callback code *\/   */
/*   int callbackevent; */
/*   /\** specifies if this object is visble             *\/ */
/*   BOOL visible; */
/*   int isclip; /\* Adding F.Leray 10.03.04*\/ */
/* } */
/* sciAxis;  /\** *\/ */



/**@name Axes
 * Structure used to specify axes feature 
 * @author Djalel Abdemouche 
 * @version 0.1
 * @args 
 * @return 
 * @see 
 */
 
typedef struct
{
  sciRelationShip relationship;
  sciGraphicContext graphiccontext;
  sciFont fontcontext;
  char dir;   /** dir = 'r' | 'l' | 'u' | 'd' : gives the tics directions **/  
  char tics;  /** tics = 'v' (for vector) or 'r' (for range) or i **/
  POINT2D *vector;		/* vecteur de points redondant, for future developpement*/
  double *vx;  /** vx vector of size nx **/
  double *vy;  /** vy vector of size ny **/ 
  /**DJ.Abdemouche 2003**/
  double *vz; /* not used */
  int nx;
  int ny;
  int nb_tics_labels; /* F.Leray 29.04.05 : number of tics_labels can be different from Max(nx,ny) if xy_type diff. from 'v' */
  /***/
  int nz;
  char **str ;  /** string vector **/
  int subint;  /** subints : number of sub intervals **/ 
  char *format; /** format for tick marks **/
  char logscale; /* not used */
  int seg;      /**0 or 1, flag which control the drawing of the segment associated to the axes **/
  char *callback; /** specifies the text scilab code for the callback associated with this entity */
  int callbacklen; /** the length of the callback code */  
  int callbackevent;
  /** specifies if this object is visble             */
  BOOL visible;
  int isclip;
  double clip_region[4]; /* to introduce for axis ? */
  int clip_region_set;

  int * user_data; /* adding 27.06.05 */
  int size_of_user_data;
}
sciAxes;  

/**@name Segs
 * Structure used to specify axes feature 
 * @author Djalel Abdemouche 
 * @version 0.1
 * @args 
 * @return 
 * @see 
 */
  
typedef struct
{
  sciRelationShip relationship;
  sciGraphicContext graphiccontext; 
  double *vx;  /** vx vector of size Nbr **/ /*F.Leray 18.02.04 ...of size Nbr1 ? No depending on the type ptype*/
  double *vy;  /** vy vector of size Nbr **/ /*F.Leray 18.02.04 ...of size Nbr2 ? No depending on the type ptype*/
  double *vfx; /* size Nbr1 * Nbr2 */
  double *vfy; /* size Nbr1 * Nbr2 */
  integer Nbr1; /**< size of vx and vy id segs handle, or size of vx if champ */ 
  integer Nbr2; /**< size of vy if champ handle */
  integer *pstyle;
  integer iflag;      /**0 or 1, flag which control the drawing of the segment  **/
  double arrowsize;  /*F.Leray units : hundreds (i.e. 100, 150,...)*/
  double parfact;
  integer ptype; /* if ptype=0, it is segments; if ptype=1, it is champ (champ or champ1)*/
  integer typeofchamp; /* when ptype=0, if typeofchamp=0 => champ is invoked else champ1 is invoked (typeofchamp==1) */
  BOOL isselected;
  char *callback; /** specifies the text scilab code for the callback associated with this entity */
  int callbacklen; /** the length of the callback code */  
  int callbackevent;
  /** specifies if this object is visble             */
  BOOL visible;
  int isclip;
  double clip_region[4];
  int clip_region_set;
  double *vz; /**DJ.Abdemouche 2003**/
  double *vfz;

  int * user_data; /* adding 27.06.05 */
  int size_of_user_data;
}
sciSegs; 
 



/**@name Grayplot
 * Structure used to specify axes feature 
 * @author Djalel Abdemouche 
 * @version 0.1
 * @args 
 * @return 
 * @see 
 */
 
typedef struct
{
  sciRelationShip relationship;
  sciGraphicContext graphiccontext; 
  
  double *pvecx;  /** vx vector of size nx **/
  double *pvecy;  /** vy vector of size ny **/ 
  double *pvecz;  /** vz vector of size nx*ny **/  
  int nx; 
  int ny;
  int type;   /** 0 if a grayplot, 1if a matplot, 2 if matplot 1 **/  
  char datamapping[7]; /* "scaled" or "direct" */
  BOOL isselected;
  char *callback; /** specifies the text scilab code for the callback associated with this entity */
  int callbacklen; /** the length of the callback code */   
  int callbackevent;
  /** specifies if this object is visble             */
  BOOL visible;

  int isclip; /**< Clipping state of the object */
  double clip_region[4]; /**< Clipping region */
  int clip_region_set; /**< To know if the clippign region is set */

  int * user_data; /* adding 27.06.05 */
  int size_of_user_data;
}
sciGrayplot;  
/** */

/**@name Fec
 * Structure used to specify fec feature 
 * @author Djalel Abdemouche 
 * @version 0.1
 * @args 
 * @return 
 * @see 
 */
 
typedef struct
{
  sciRelationShip relationship;
  sciGraphicContext graphiccontext; 
  
  double *pvecx; /* X coordinates of nodes */
  double *pvecy; /* Y coordinates of nodes */
  double *pnoeud; /* indices of nodes */
  double *pfun; /* Height value for nodes */
  int Nnode; /* number of nodes */
  int Ntr;   /* number of triangles */
  double zminmax[2]; /* Array of size 2 containing zmin and zmax */
  integer colminmax[2]; /*  */
  integer colout[2]; /* */
  BOOL with_mesh;
  BOOL isselected;
  char *callback; /** specifies the text scilab code for the callback associated with this entity */
  int callbacklen; /** the length of the callback code */ 
  int callbackevent;
  /** specifies if this object is visble             */
  BOOL visible;

  int isclip; /**< Clipping state of the object */
  double clip_region[4]; /**< Clipping region */
  int clip_region_set; /**< To know if the clippign region is set */

  int * user_data; /* adding 27.06.05 */
  int size_of_user_data;
}
sciFec;  /** */


/**@name Panner
 * Structure used to specify 
 */
typedef struct
{
  sciRelationShip relationship;
  int posx;
  int posy;
  int width;
  int height;
  int totalheigth;		/* the height in the menu bar */
}
sciPanner;  /** */


/**@name ScrollBarHorz
 * Structure used to specify 
 */
typedef struct
{
  sciRelationShip relationship;
  int poshorz;
  int totalheight;
}
sciScrollBarHorz;  /** */


/**@name ScrollBarVert
 * Structure used to specify 
 */
typedef struct
{
  sciRelationShip relationship;
  int posvert;
  int totalwidth;
}
sciScrollBarVert;  /** */


/**@name StatusBar
 * Structure used to specify 
 */
typedef struct
{
  sciRelationShip relationship;
  sciGraphicContext graphiccontext;
  sciText text;
  char *ptext;
}
sciStatusBar;  /** */


/**@name Panner
 * Structure used to specify 
 */
typedef struct
{
  sciRelationShip relationship;
  double xmin;			   /** original xmin */
  double ymin;			   /** original ymin */
  double xmax;
  double ymax;
  BOOL isselected;
  char *callback; /** specifies the text scilab code for the callback associated with this entity */
  int callbacklen; /** the length of the callback code */ 
  int callbackevent;
  /** specifies if this object is visble             */
  BOOL visible;

  int * user_data; /* adding 27.06.05 */
  int size_of_user_data;
}
sciAgreg;  /** */

/**
 * Console window Object
 */
typedef struct 
{
  sciRelationShip relationship ;

  BOOL visible ;

  int * user_data ;
  int size_of_user_data;

} sciConsole ;

/**
 * Frame Object
 */
typedef struct 
{
  sciRelationShip relationship ;

  BOOL visible ;

  int * user_data ;
  int size_of_user_data;

} sciFrame ;

/**
 * Window object
 */
typedef struct 
{
  sciRelationShip relationship ;

  BOOL visible ;

  int * user_data ;
  int size_of_user_data;

} sciWindow ;

/**
* WindowFrame object
*/
typedef struct 
{
  sciRelationShip relationship ;

  BOOL visible ;

  int * user_data ;
  int size_of_user_data;

} sciWindowFrame ;

/**
* Screen object
*/
typedef struct 
{
  sciRelationShip relationship ;

  BOOL visible ;

  int * user_data ;
  int size_of_user_data;

} sciScreen ;


#endif /*__SCI_OBJECT_STRUCTURE__ */
