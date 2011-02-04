/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2001-2002 - INRIA - Mathieu Philipe
 * Copyright (C) 2002-2004 - INRIA - Djalel Abdemouche
 * Copyright (C) 2004-2006 - INRIA - Fabrice Leray
 * Copyright (C) 2005 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2008-2008 - INRIA - Bruno JOFRET
 * Copyright (C) 2010 - DIGITEO - Bruno JOFRET
 * Copyright (C) 2010-2011 - DIGITEO - Manuel Juliachs
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
 *    Comment:
 *    This file contains all functions used to Init or Re-Init the window
 *    (Figure and/or Subwin) to the default graphics properties.
 --------------------------------------------------------------------------*/

#include <stdio.h>
#include <string.h>


#ifdef _MSC_VER
#include "strdup_Windows.h"
#endif

#include "InitObjects.h"
#include "SetProperty.h"
#include "GetProperty.h"
#include "DestroyObjects.h"
#include "CloneObjects.h"
#include "BuildObjects.h"
#include "sciprint.h"
#include "ObjectSelection.h"
#include "HandleManagement.h"
#include "BasicAlgos.h"

#include "MALLOC.h" /* MALLOC */
#include "localization.h"

#include "createGraphicObject.h"
#include "deleteGraphicObject.h"
#include "graphicObjectProperties.h"
#include "getGraphicObjectProperty.h"
#include "setGraphicObjectProperty.h"

#define LABEL_BUFFER_LENGTH	128

static char error_message[70]; /* DJ.A 08/01/04 */
unsigned short defcolors[] = {
  0,   0,   0, /* Black: DEFAULTBLACK */
  0,   0, 255, /* Blue */
  0, 255,   0, /* Green */
  0, 255, 255, /* Cyan */
  255,   0,   0, /* Red */
  255,   0, 255, /* Magenta */
  255,   255,   0, /* Yellow */
  255, 255, 255, /* White: DEFAULTWHITE */
  0,   0, 144, /* Blue4 */
  0,   0, 176, /* Blue3 */
  0,   0, 208, /* Blue2 */
  135, 206, 255, /* LtBlue */
  0, 144,   0, /* Green4 */
  0, 176,   0, /* Green3 */
  0, 208,   0, /* Green2 */
  0, 144, 144, /* Cyan4 */
  0, 176, 176, /* Cyan3 */
  0, 208, 208, /* Cyan2 */
  144,   0,   0, /* Red4 */
  176,   0,   0, /* Red3 */
  208,   0,   0, /* Red2 */
  144,   0, 144, /* Magenta4 */
  176,   0, 176, /* Magenta3 */
  208,   0, 208, /* Magenta2 */
  128,  48,   0, /* Brown4 */
  160,  64,   0, /* Brown3 */
  192,  96,   0, /* Brown2 */
  255, 128, 128, /* Pink4 */
  255, 160, 160, /* Pink3 */
  255, 192, 192, /* Pink2 */
  255, 224, 224, /* Pink */
  255, 215,   0  /* Gold */
};

static sciPointObj * pfiguremdl = NULL;
static sciPointObj * paxesmdl = NULL;

sciPointObj * getFigureModel( void )
{
  return pfiguremdl ;
}

sciPointObj * getAxesModel( void )
{
  return paxesmdl ;
}

BOOL isFigureModel(sciPointObj * pObj)
{
  return (pObj == pfiguremdl);
}

BOOL isAxesModel(sciPointObj * pObj)
{
  return (pObj == paxesmdl);
}


/* DJ.A 08/01/04 */
int C2F(graphicsmodels) (void)
{
    sciSubWindow * ppaxesmdl = NULL ;
    int iZero = 0;
    BOOL bTrue = TRUE;
    BOOL bFalse = FALSE;

    int defaultBackground = -2;
    int m = NUMCOLORS_SCI;
    int i = 0;
    double *pdblColorMap = MALLOC(m * 3 * sizeof(double)) ;

    double margins[4];
    double clipRegion[4];
    int clipRegionSet;
    int firstPlot;
    int result;

    /*
    ** Init Figure Model
    */
    if ((pfiguremdl = MALLOC ((sizeof (sciPointObj)))) == NULL)
    {
        strcpy(error_message,_("Default figure cannot be create.\n"));
        return 0;
    }

    // Create default figure by Asking MVC a new one.
    pfiguremdl->UID = createGraphicObject(__GO_FIGURE__);
    // Name
    setGraphicObjectProperty(pfiguremdl->UID, __GO_NAME__, _("Graphic window number %d"), jni_string, 1);

    // Id
    setGraphicObjectProperty(pfiguremdl->UID, __GO_ID__, &iZero, jni_int, 1);
    // pModelData
    // isselected ?? (No more used)
    // rotstyle = unary (0)
  setGraphicObjectProperty(pfiguremdl->UID, __GO_ROTATION_TYPE__, &iZero, jni_int, 1);
  // visible

  setGraphicObjectProperty(pfiguremdl->UID, __GO_VISIBLE__, &bTrue, jni_bool, 1);
  // immediateDrawingMode
  setGraphicObjectProperty(pfiguremdl->UID, __GO_IMMEDIATE_DRAWING__, &bTrue, jni_bool, 1);
  // background
  setGraphicObjectProperty(pfiguremdl->UID, __GO_BACKGROUND__, &defaultBackground, jni_int, 1);
  // user data
  setGraphicObjectProperty(pfiguremdl->UID, __GO_USER_DATA__, NULL, jni_string, 0);
  // Size of user data
  setGraphicObjectProperty(pfiguremdl->UID, __GO_USER_DATA_SIZE__, &iZero, jni_int, 1);
  // Pixmap Mode
  setGraphicObjectProperty(pfiguremdl->UID, __GO_PIXMAP__, &bFalse, jni_bool, 1);
  // Info Message
  setGraphicObjectProperty(pfiguremdl->UID, __GO_INFO_MESSAGE__, "", jni_string, 1);
  // Event Handler
  setGraphicObjectProperty(pfiguremdl->UID, __GO_EVENTHANDLER__, "", jni_string, 1);
  // Event Handler Enable
  setGraphicObjectProperty(pfiguremdl->UID, __GO_EVENTHANDLER_ENABLE__, &bFalse, jni_bool, 1);
  // Tag
  setGraphicObjectProperty(pfiguremdl->UID, __GO_TAG__, "", jni_string, 1);


  if (pdblColorMap == NULL)
  {
	  sprintf(error_message,_("%s: No more memory.\n"),"InitFigureModel");
	  return -1 ;
  }

  for (i = 0 ; i < m ; i++ )
  {
      pdblColorMap[i        ] = (double) (defcolors[3*i]/255.0) ;
      pdblColorMap[i + m    ] = (double) (defcolors[3*i+1]/255.0) ;
      pdblColorMap[i + 2 * m] = (double) (defcolors[3*i+2]/255.0) ;
  }
  // ColorMap
  setGraphicObjectProperty(pfiguremdl->UID, __GO_COLORMAP__, pdblColorMap, jni_double_vector, 3 * m);

  // Parent
  setGraphicObjectProperty(pfiguremdl->UID, __GO_PARENT__, "", jni_string, 1);

  // Register object inside Scilab
  sciAddNewHandle(pfiguremdl);

  /*
  ** Init Axes Model
  */
  if ((paxesmdl = MALLOC ((sizeof (sciPointObj)))) == NULL)
  {
      strcpy(error_message,_("Default axes cannot be created.\n"));
      return 0;
  }

  // Create default Axes by Asking MVC a new one.
  paxesmdl->UID = createGraphicObject(__GO_AXES__);

  /* Axes Model properties */

  result = InitAxesModel();

  if (result < 0)
  {
    strcpy(error_message,_("Default axes cannot be created.\n"));
    deleteGraphicObject(paxesmdl->UID);
    return 0;
  }

  /* Margins and clip region */

  margins[0] = 0.125;
  margins[1] = 0.125;
  margins[2] = 0.125;
  margins[3] = 0.125;

  setGraphicObjectProperty(paxesmdl->UID, __GO_MARGINS__, margins, jni_double_vector, 4);

  clipRegion[0] = 0.0;
  clipRegion[1] = 0.0;
  clipRegion[2] = 0.0;
  clipRegion[3] = 0.0;

  setGraphicObjectProperty(paxesmdl->UID, __GO_CLIP_BOX__, clipRegion, jni_double_vector, 4);

  clipRegionSet = 0;
  setGraphicObjectProperty(paxesmdl->UID, __GO_CLIP_BOX_SET__, &clipRegionSet, jni_bool, 1);

  /* add the handle in the handle list */
  if ( sciAddNewHandle(paxesmdl) == -1 )
  {
    return NULL;
  }

  /*
   * Specifies that no high-level drawing function has been called yet.
   */
  firstPlot = 1;
  setGraphicObjectProperty(paxesmdl->UID, __GO_FIRST_PLOT__, &firstPlot, jni_bool, 1);

#if 0
  ppaxesmdl->FirstPlot = TRUE;
#endif

  /* Sets the parent-child relationship between the default Figure and Axes */
  setGraphicObjectRelationship(pfiguremdl->UID, paxesmdl->UID);

  /* Commented out: the equivalent MVC operations are done just above */
#if 0
  sciSetEntityType (paxesmdl, SCI_SUBWIN);
  if ((paxesmdl->pfeatures = MALLOC ((sizeof (sciSubWindow)))) == NULL)
    {
      FREE(paxesmdl);
      strcpy(error_message,_("Default axes cannot be create.\n"));
      return 0;
    }
	createDefaultRelationShip(paxesmdl);

	/* add the handle in the handle list */
  if ( sciAddNewHandle(paxesmdl) == -1 )
  {
    return NULL ;
  }


  /* there are properties not initialized bu InitAxesModel */
  /* Is it a missing ? */

  ppaxesmdl->FirstPlot = TRUE;

  ppaxesmdl->ARect[0]   = 0.125;
  ppaxesmdl->ARect[1]   = 0.125;
  ppaxesmdl->ARect[2]   = 0.125;
  ppaxesmdl->ARect[3]   = 0.125;

  ppaxesmdl->clip_region[0] = 0.;
  ppaxesmdl->clip_region[1] = 0.;
  ppaxesmdl->clip_region[2] = 0.;
  ppaxesmdl->clip_region[3] = 0.;

  /* the model has not been changed !!! */
  ppaxesmdl->clip_region_set = 0 ;

#endif

  return 1;
}

/*
 * This function has been adapted to the MVC framework.
 * Its code ought to be moved to the Java Model implementation,
 * either within the relevant constructors (Axes, ContouredObject)
 * or initialization methods.
 */

/**sciInitGraphicContext
 * Inits the graphic context of this object with the default value. the new graphic object inherits parent's features by sciGetParent()
 */
int
sciInitGraphicContext (sciPointObj * pobj)
{
  char* type;

  /*
   * initialisation du contexte graphique par defaut
   * que l'on peut recuperer sur les structure de base de scilab
   * la colormap des fils est heritee du parent
   */

  getGraphicObjectProperty(pobj->UID, __GO_TYPE__, jni_string, &type);


//  switch (sciGetEntityType (pobj))
  {

  /*
   * The GO_FIGURE block is never reached as InitFigureModel
   * is not called any more.
   */
  if (strcmp(type, __GO_FIGURE__) == 0)
  {
    /*
     * The Figure GraphicContext properties (MVC Contoured properties) were not
     * directly used by the Figure object (except backgroundcolor) but served to
     * initialize its children Axes' ones.
     */
    if ( pobj == pfiguremdl )
    {
#if 0
      (sciGetGraphicContext(pobj))->backgroundcolor = /*-3;*/ 33;  /* F.Leray 29.03.04: Wrong index here: 32+1 (old method) must be changed to -1 new method*/
      (sciGetGraphicContext(pobj))->foregroundcolor = /*-2;*/ 32;  /* F.Leray 29.03.04: Wrong index here: 32+2 (old method) must be changed to -2 new method*/
      (sciGetGraphicContext(pobj))->fillcolor = (sciGetGraphicContext(pobj))->backgroundcolor;
      (sciGetGraphicContext(pobj))->linewidth = 1;
      (sciGetGraphicContext(pobj))->linestyle = 0; /* solid */
      (sciGetGraphicContext(pobj))->ismark    = FALSE;
      (sciGetGraphicContext(pobj))->isline    = TRUE;
      (sciGetGraphicContext(pobj))->isfilled  = FALSE;
      (sciGetGraphicContext(pobj))->markstyle = 0;
      (sciGetGraphicContext(pobj))->marksize = 0 ; /* New F.Leray 21.01.05 */
      (sciGetGraphicContext(pobj))->markbackground = /*-3;*/ 33; /* New F.Leray 21.01.05 */
      (sciGetGraphicContext(pobj))->markforeground = /*-2;*/ 32; /* New F.Leray 21.01.05 */
      (sciGetGraphicContext(pobj))->marksizeunit = 2; /* New F.Leray 22.02.05 */ /* 1 : points, 2 : tabulated */
#endif
    }
    else
    {
#if 0
      cloneGraphicContext( pfiguremdl, pobj );
#endif
    }
    return 0;
  }
  else if (strcmp(type, __GO_AXES__) == 0)
  {

    /*
     * This block is reached as InitGraphicContext is called by InitAxesModel
     * The property set calls it performs should be moved to the Java Model.
     * Contoured properties are not copied from the parent Figure any more for now
     * and are instead explicitely set here, in order to initialize the default
     * Axes' ones.
     */
    if ( pobj == paxesmdl )
    {
      int background = -2;
      int foreground = -1;
      double lineWidth = 1.0;

      /* 0: solid */
      int lineStyle = 0;

      int markMode = 0;
      int lineMode = 1;
      int fillMode = 0;
      int markStyle = 0;
      int markSize = 0;

      /* 0: point, 1: tabulated */
      int markSizeUnit = 1;

      setGraphicObjectProperty(pobj->UID, __GO_BACKGROUND__, &background, jni_int, 1);
      setGraphicObjectProperty(pobj->UID, __GO_LINE_COLOR__, &foreground, jni_int, 1);

      setGraphicObjectProperty(pobj->UID, __GO_LINE_THICKNESS__, &lineWidth, jni_double, 1);
      setGraphicObjectProperty(pobj->UID, __GO_LINE_STYLE__, &lineStyle, jni_int, 1);

      setGraphicObjectProperty(pobj->UID, __GO_MARK_MODE__, &markMode, jni_bool, 1);
      setGraphicObjectProperty(pobj->UID, __GO_LINE_MODE__, &lineMode, jni_bool, 1);
      setGraphicObjectProperty(pobj->UID, __GO_FILL_MODE__, &fillMode, jni_bool, 1);

      setGraphicObjectProperty(pobj->UID, __GO_MARK_STYLE__, &markStyle, jni_int, 1);
      setGraphicObjectProperty(pobj->UID, __GO_MARK_SIZE__, &markSize, jni_int, 1);
      setGraphicObjectProperty(pobj->UID, __GO_MARK_SIZE_UNIT__, &markSizeUnit, jni_int, 1);

      setGraphicObjectProperty(pobj->UID, __GO_MARK_BACKGROUND__, &background, jni_int, 1);
      setGraphicObjectProperty(pobj->UID, __GO_MARK_FOREGROUND__, &foreground, jni_int, 1);


#if 0
      cloneGraphicContext( sciGetParent (pobj), pobj ) ;

      sciGetGraphicContext(pobj)->backgroundcolor = /*-3 ;*/ 33;
      sciGetGraphicContext(pobj)->foregroundcolor = /*-2 ;*/ 32;
      sciGetGraphicContext(pobj)->markbackground  = /*-3*/ 33;
      sciGetGraphicContext(pobj)->markforeground  = /*-2*/ 32;
#endif
    }
    /*
     * This block is never reached since the Axes model
     * is now cloned within the MVC.
     */
    else
    {
#if 0
      cloneGraphicContext( paxesmdl, pobj ) ;
#endif
    }
    return 0;
  }
  else if (strcmp(type, __GO_ARC__) == 0)
  {
    char* parent;

    getGraphicObjectProperty(pobj->UID, __GO_PARENT__, jni_string, &parent);
    cloneGraphicContext(parent, pobj->UID);
  }
  else if (strcmp(type, __GO_SEGS__) == 0)
  {
    char* parent;

    getGraphicObjectProperty(pobj->UID, __GO_PARENT__, jni_string, &parent);
    cloneGraphicContext(parent, pobj->UID);
  }
  else if (strcmp(type, __GO_CHAMP__) == 0)
  {
    char* parent;

    getGraphicObjectProperty(pobj->UID, __GO_PARENT__, jni_string, &parent);
    cloneGraphicContext(parent, pobj->UID);
  }
  /*
   * Copies the parent object's contour properties values
   */
  else if (strcmp(type, __GO_POLYLINE__) == 0)
  {
    char* parent;

    getGraphicObjectProperty(pobj->UID, __GO_PARENT__, jni_string, &parent);
    cloneGraphicContext(parent, pobj->UID);

    /*
     * Previously done by copying the deprecated C GraphicContext structure.
     * To be deleted.
     */
#if 0
    cloneGraphicContext( sciGetParent(pobj), pobj );
#endif
  }
  else if (strcmp(type, __GO_RECTANGLE__) == 0)
  {
    char* parent;

    getGraphicObjectProperty(pobj->UID, __GO_PARENT__, jni_string, &parent);
    cloneGraphicContext(parent, pobj->UID);
  }
  else if ((strcmp(type, __GO_FAC3D__) == 0) || (strcmp(type, __GO_PLOT3D__) == 0))
  {
    char* parent;

    getGraphicObjectProperty(pobj->UID, __GO_PARENT__, jni_string, &parent);
    cloneGraphicContext(parent, pobj->UID);
  }
  else if (strcmp(type, __GO_LEGEND__) == 0)
  {
    char* parent;

    getGraphicObjectProperty(pobj->UID, __GO_PARENT__, jni_string, &parent);
    cloneGraphicContext(parent, pobj->UID);
  }

  /* Deactivated for now */
  /* This must be implemented within the MVC */
#if 0
  case SCI_ARC:
  case SCI_SEGS:
  case SCI_FEC:
  case SCI_GRAYPLOT:
  case SCI_POLYLINE:
  case SCI_RECTANGLE:
  case SCI_SURFACE:
  case SCI_AXES:
  case SCI_LEGEND: /* Adding a graphic context to legend object F.Leray 21.01.05 */
  case SCI_TEXT:
    cloneGraphicContext( sciGetParent (pobj), pobj ) ;
    return 0;
    break;
  case SCI_LABEL: /* F.Leray 28.05.04, modif JB.Silvy 03/2006 */
    if ( sciGetParent(pobj) == paxesmdl )
    {
      /* this is a label model */
      cloneGraphicContext( sciGetParent( pobj ), pobj ) ;
      return 0;
    }
    else
    {
      sciPointObj * plabelmdl = NULL ;
      switch ( pLABEL_FEATURE(pobj)->ptype )
      {
      case 1:
        plabelmdl = pSUBWIN_FEATURE(paxesmdl)->mon_title;
        break ;
      case 2:
        plabelmdl = pSUBWIN_FEATURE(paxesmdl)->mon_x_label;
        break ;
      case 3:
        plabelmdl = pSUBWIN_FEATURE(paxesmdl)->mon_y_label;
        break ;
      case 4:
        plabelmdl = pSUBWIN_FEATURE(paxesmdl)->mon_z_label;
        break ;
      default:
        /* arrgh*/
        return -1 ;
        break ;
      }
      cloneGraphicContext( plabelmdl, pobj ) ;
      return 0 ;
    }
  case SCI_AGREG:
  case SCI_UIMENU:
  default:
    return -1;
    break;
#endif
  }

return 0;
}

/**
 * Inits the font context of an object by copying the one of an other.
 * @param pObjSource the object from which the FC is taken
 * @param pObjDest the object in which the FC is paste
 */
int initFCfromCopy(  sciPointObj * pObjSource, sciPointObj * pObjDest )
{
    return cloneFontContext( pObjSource->UID, pObjDest->UID );
}



/*
 * This function has been partially adapted to the MVC framework.
 * Its code ought to be moved to the Java Model implementation,
 * either within the Font constructor or an initialization method.
 */
/**sciInitFontContext
 * Inits the graphic context of this object with the default value
 * @param sciPointObj * pobj: the pointer to the entity
 */
int
sciInitFontContext (sciPointObj * pobj)
{
  char* type;

  /*
   * initialisation du contexte font par defaut
   * que l'on peut recuperer sur les structures de base de scilab
   * la colormap des fils est heritee du parent
   * nous prenons le couleur de background et foreground
   * plutot que fontforeground pour l'initialisation
   */

  /* unknown function initfontname "Win-stand"!! */
  /* static TCHAR inifontname[] = TEXT ("Times New Roman");*/


  getGraphicObjectProperty(pobj->UID, __GO_TYPE__, jni_string, &type);

//  switch (sciGetEntityType (pobj))

/* Deactivated for now */
/* This must be implemented within the MVC */
#if 0
  {
  case SCI_TEXT:
  case SCI_LEGEND:
  case SCI_AXES:
  {
    initFCfromCopy( sciGetParent(pobj), pobj ) ;
  }
  break;
  case SCI_LABEL: /* Re-init here must be better F.Leray 28.05.04 */
    if ( sciGetParent(pobj) == paxesmdl )
    {
      initFCfromCopy( sciGetParent(pobj), pobj ) ;
    }
    else
    {
      sciPointObj * plabelmdl = NULL;

      if(pLABEL_FEATURE(pobj)->ptype == 1) /* title */
      {
        plabelmdl = pSUBWIN_FEATURE(paxesmdl)->mon_title;
      }
      else if(pLABEL_FEATURE(pobj)->ptype == 2) /* x_label */
      {
        plabelmdl = pSUBWIN_FEATURE(paxesmdl)->mon_x_label;
      }
      else if(pLABEL_FEATURE(pobj)->ptype == 3) /* y_label */
      {
        plabelmdl = pSUBWIN_FEATURE(paxesmdl)->mon_y_label;
      }
      else if(pLABEL_FEATURE(pobj)->ptype == 4) /* z_label */
      {
        plabelmdl = pSUBWIN_FEATURE(paxesmdl)->mon_z_label;
      }

      initFCfromCopy( plabelmdl, pobj ) ;

    }
    break;
#endif

  if (strcmp(type, __GO_AXES__) == 0)
  {
    /*
     * Font properties are not copied from the parent Figure for now
     * and are instead explicitely set here, in order to initialize the
     * default Axes' font properties.
     */
    if (pobj == paxesmdl)
    {
      int fontColor = -1;
      double fontSize = 1.0;
      int fontFractional = 0;
      /* 6: Helvetica */
      int fontStyle = 6;

      setGraphicObjectProperty(pobj->UID, __GO_FONT_COLOR__, &fontColor, jni_int, 1);
      setGraphicObjectProperty(pobj->UID, __GO_FONT_SIZE__, &fontSize, jni_double, 1);
      setGraphicObjectProperty(pobj->UID, __GO_FONT_FRACTIONAL__, &fontFractional, jni_bool, 1);
      setGraphicObjectProperty(pobj->UID, __GO_FONT_STYLE__, &fontStyle, jni_int, 1);

      /* Deactivated for now since it causes a crash */
#if 0
      initFCfromCopy( sciGetParent( pobj ), pobj );
#endif
    }
    else
    {
      /*
       * This block is never reached at all since since the Axes model
       * is now cloned within the MVC via a C call.
       */
      initFCfromCopy( paxesmdl, pobj );
    }
  }
  else if (strcmp(type, __GO_FIGURE__) == 0)
  {
    if (pobj == pfiguremdl)
    {
       sciInitFontStyle (pobj, 6); /* set helvetica font */
      (sciGetFontContext(pobj))->backgroundcolor = 33;
      (sciGetFontContext(pobj))->foregroundcolor = 32;
      (sciGetFontContext(pobj))->fontSize = 1.0;
      (sciGetFontContext(pobj))->textorientation = 0.0;
      (sciGetFontContext(pobj))->useFractionalMetrics = FALSE;
      /* END ADDING F.Leray 08.04.04*/
    }
    else
    {
      initFCfromCopy( pfiguremdl, pobj );
    }
  }
  else if (strcmp(type, __GO_LEGEND__) == 0)
  {
    char* parent;

    getGraphicObjectProperty(pobj->UID, __GO_PARENT__, jni_string, &parent);
    cloneFontContext(parent, pobj->UID);
  }

  /* Deactivated for now */
#if 0
  case SCI_ARC:
  case SCI_SEGS:
  case SCI_FEC:
  case SCI_GRAYPLOT:
  case SCI_POLYLINE:
  case SCI_RECTANGLE:
  case SCI_SURFACE:
  case SCI_AGREG:
  case SCI_UIMENU:
  default:
    return -1;
    break;
  }
#endif

  return 0;
}

int InitFigureModel( void )
{
  int i ;
  int m = NUMCOLORS_SCI ;
  double * colorMap ;
  sciFigure * ppFigure = pFIGURE_FEATURE( pfiguremdl) ;

  ppFigure->allredraw = FALSE;

  if ( sciInitGraphicContext(pfiguremdl) < 0 )
  {
    return -1 ;
  }
  if ( sciInitGraphicMode(pfiguremdl) < 0 )
  {
    return -1 ;
  }
  if ( sciInitFontContext(pfiguremdl) < 0)
  {
    /* F.Leray 10.06.04 */
    return -1 ;
  }

  sciInitName(pfiguremdl, _("Graphic window number %d"));
  pFIGURE_FEATURE (pfiguremdl)->number          = 0   ;

  /* Set figure model attributes */
  ppFigure->pModelData = newFigureModelData() ;

  pFIGURE_FEATURE (pfiguremdl)->isselected = TRUE;
  pFIGURE_FEATURE (pfiguremdl)->rotstyle = 0;
  pFIGURE_FEATURE (pfiguremdl)->visible = TRUE;
  /* auto_redraw */
  sciInitImmediateDrawingMode(pfiguremdl, TRUE);/* by default, we draw live */

  pFIGURE_FEATURE (pfiguremdl)->user_data = (int *) NULL; /* pour completude */
  pFIGURE_FEATURE (pfiguremdl)->size_of_user_data = 0; /* pour completude */

  pFIGURE_FEATURE (pfiguremdl)->numsubwinselected = 0;
  sciInitPixmapMode(pfiguremdl, FALSE);
  sciInitInfoMessage( pfiguremdl, "") ;

  /*
  ** Must set Event Handler before making it enable
  ** Otherwise causes a warning.
  */
  // sciInitEventHandler(pfiguremdl, "");
  pFIGURE_FEATURE(pfiguremdl)->eventHandler = strdup("");
  // sciInitIsEventHandlerEnable( pfiguremdl, FALSE ) ;
  pFIGURE_FEATURE(pfiguremdl)->isEventHandlerEnable = FALSE;

  pFIGURE_FEATURE(pfiguremdl)->tag = NULL ;

  pfiguremdl->pObservers = NULL ;
  pfiguremdl->pDrawer = NULL ;
  colorMap = MALLOC( m * 3 * sizeof(double) ) ;
  if ( colorMap == NULL )
  {
	  sprintf(error_message,_("%s: No more memory.\n"),"InitFigureModel");
	  return -1 ;
  }

  for ( i = 0 ; i < m ; i++ )
  {
    colorMap[i        ] = (double) (defcolors[3*i]/255.0) ;
    colorMap[i + m    ] = (double) (defcolors[3*i+1]/255.0) ;
    colorMap[i + 2 * m] = (double) (defcolors[3*i+2]/255.0) ;
  }

  sciSetColormap( pfiguremdl, colorMap, m, 3 ) ;
  pFIGURE_FEATURE (pfiguremdl)->numcolors = m;

  FREE( colorMap ) ;

  return 0;
}


/*
 * This function has been adapted to the MVC framework, using the
 * MVC's property set/get calls.
 */

int InitAxesModel()
{
  int cubeScaling;
  int logFlag;
  int ticksColor;
  int axisLocation;
  int boxType;
  int filled;
  int gridColor;
  int gridPosition;
  int view;
  int axisReverse;
  int axisVisible;
  int defaultNumberTicks;
  int autoTicks;
  int autoSubticks;
  int nbSubticks;
  int hiddenAxisColor;
  int hiddenColor;
  int isoview;
  int visible = 0;
  int *piVisible = &visible;
  int clipState;
  int tightLimits;
  int arcDrawingMethod;
  int i;
  int* tmp;
  double axesBounds[4];
  double dataBounds[6];
  double rotationAngles[2];
  double tab[] = {0.,0.1,0.2,0.3,0.4,0.5,0.6,0.7,0.8,0.9,1.}; /* graduations init. tmptab */
  /* z-axis initial ticks locations */
  double tabZTicksLocations[] = {-1.0, -0.8, -0.6, -0.4, -0.2, 0.0, 0.2, 0.4, 0.6, 0.8, 1.0};
  char labelBuffer[LABEL_BUFFER_LENGTH];
  char** stringVector;

  /*
   * Not needed any more since the MVC equivalent is now used
   * To be deleted
   */
#if 1
  sciSubWindow * ppaxesmdl = pSUBWIN_FEATURE (paxesmdl);
#endif

#if 0
  char linLogFlags[3] = {'n','n','n'};
#endif


  /* These functions have been adapted to the MVC framework */
  sciInitGraphicContext (paxesmdl);
  sciInitGraphicMode (paxesmdl);
//  sciInitFontContext (paxesmdl);  /* F.Leray 10.06.04 */

  cubeScaling = 0;
  setGraphicObjectProperty(paxesmdl->UID, __GO_CUBE_SCALING__, &cubeScaling, jni_bool, 1);

  /* Log flags set to linear for the 3 axes */
  logFlag = 0;

  setGraphicObjectProperty(paxesmdl->UID, __GO_X_AXIS_LOG_FLAG__, &logFlag, jni_bool, 1);
  setGraphicObjectProperty(paxesmdl->UID, __GO_Y_AXIS_LOG_FLAG__, &logFlag, jni_bool, 1);
  setGraphicObjectProperty(paxesmdl->UID, __GO_Z_AXIS_LOG_FLAG__, &logFlag, jni_bool, 1);

  ticksColor = -1;
  setGraphicObjectProperty(paxesmdl->UID, __GO_TICKS_COLOR__, &ticksColor, jni_int, 1);

  nbSubticks = 1;
  setGraphicObjectProperty(paxesmdl->UID, __GO_X_AXIS_SUBTICKS__, &nbSubticks, jni_int, 1);
  setGraphicObjectProperty(paxesmdl->UID, __GO_Y_AXIS_SUBTICKS__, &nbSubticks, jni_int, 1);
  setGraphicObjectProperty(paxesmdl->UID, __GO_Z_AXIS_SUBTICKS__, &nbSubticks, jni_int, 1);


  /* 0 corresponds to bottom position */
  axisLocation = 0;
  setGraphicObjectProperty(paxesmdl->UID, __GO_X_AXIS_LOCATION__, &axisLocation, jni_int, 1);

  /* 4 corresponds to left position */
  axisLocation = 4;
  setGraphicObjectProperty(paxesmdl->UID, __GO_Y_AXIS_LOCATION__, &axisLocation, jni_int, 1);

  /* 0 corresponds to OFF */
  boxType = 0;
  setGraphicObjectProperty(paxesmdl->UID, __GO_BOX_TYPE__, &boxType, jni_int, 1);

  filled = 1;
  setGraphicObjectProperty(paxesmdl->UID, __GO_FILLED__, &filled, jni_bool, 1);

  gridColor = -1;
  setGraphicObjectProperty(paxesmdl->UID, __GO_X_AXIS_GRID_COLOR__, &gridColor, jni_int, 1);
  setGraphicObjectProperty(paxesmdl->UID, __GO_Y_AXIS_GRID_COLOR__, &gridColor, jni_int, 1);
  setGraphicObjectProperty(paxesmdl->UID, __GO_Z_AXIS_GRID_COLOR__, &gridColor, jni_int, 1);

  /* 0: background */
  gridPosition = 0;

  setGraphicObjectProperty(paxesmdl->UID, __GO_GRID_POSITION__, &gridPosition, jni_int, 1);

  rotationAngles[0] = 0.0;
  rotationAngles[1] = 270.0;

  setGraphicObjectProperty(paxesmdl->UID, __GO_ROTATION_ANGLES__, rotationAngles, jni_double_vector, 2);

  /* 0: 2D view */
  view = 0;
  setGraphicObjectProperty(paxesmdl->UID, __GO_VIEW__, &view, jni_int, 1);


  /* Must be set after VIEW, since setting VIEW to 2D overwrites the 3D rotation angles */
  rotationAngles[0] = 45.0;
  rotationAngles[1] = 215.0;

  setGraphicObjectProperty(paxesmdl->UID, __GO_ROTATION_ANGLES_3D__, rotationAngles, jni_double_vector, 2);

  axisVisible = 0;
  setGraphicObjectProperty(paxesmdl->UID, __GO_X_AXIS_VISIBLE__, &axisVisible, jni_bool, 1);
  setGraphicObjectProperty(paxesmdl->UID, __GO_Y_AXIS_VISIBLE__, &axisVisible, jni_bool, 1);
  setGraphicObjectProperty(paxesmdl->UID, __GO_Z_AXIS_VISIBLE__, &axisVisible, jni_bool, 1);

  axisReverse = 0;
  setGraphicObjectProperty(paxesmdl->UID, __GO_X_AXIS_REVERSE__, &axisReverse, jni_bool, 1);
  setGraphicObjectProperty(paxesmdl->UID, __GO_Y_AXIS_REVERSE__, &axisReverse, jni_bool, 1);
  setGraphicObjectProperty(paxesmdl->UID, __GO_Z_AXIS_REVERSE__, &axisReverse, jni_bool, 1);

  /* Corresponds to the MVC AUTO_SUBTICKS property (!flagNax is equivalent to AUTO_SUBTICKS) */
#if 0
  ppaxesmdl->flagNax = FALSE;
#endif

  autoSubticks = 1;
  setGraphicObjectProperty(paxesmdl->UID, __GO_AUTO_SUBTICKS__, &autoSubticks, jni_bool, 1);

  /* To be corrected when the equivalent of flagnax is implemented within the MVC */
  nbSubticks = 1;
  setGraphicObjectProperty(paxesmdl->UID, __GO_X_AXIS_SUBTICKS__, &nbSubticks, jni_int, 1);
  setGraphicObjectProperty(paxesmdl->UID, __GO_Y_AXIS_SUBTICKS__, &nbSubticks, jni_int, 1);
  setGraphicObjectProperty(paxesmdl->UID, __GO_Z_AXIS_SUBTICKS__, &nbSubticks, jni_int, 1);

  /*
   * The code creating default ticks labels and positions should be put
   * into the Model. Ticks positions and locations should be computed using the default data bounds
   * instead of using pre-defined values.
   * Note that the pre-MVC ticks labels creation code is implemented in the C++ Renderer module
   * and should be moved to the Java Model's relevant parts (TicksProperty).
   */

  /*
   * Automatic ticks computation must be activated for the 3 axes before setting ticks values to
   * ensure that the ticks values set are the automatic ticks' ones.
   */
  autoTicks = 1;
  setGraphicObjectProperty(paxesmdl->UID, __GO_X_AXIS_AUTO_TICKS__, &autoTicks, jni_bool, 1);
  setGraphicObjectProperty(paxesmdl->UID, __GO_Y_AXIS_AUTO_TICKS__, &autoTicks, jni_bool, 1);
  setGraphicObjectProperty(paxesmdl->UID, __GO_Z_AXIS_AUTO_TICKS__, &autoTicks, jni_bool, 1);

  defaultNumberTicks = 11;

  setGraphicObjectProperty(paxesmdl->UID, __GO_X_AXIS_TICKS_LOCATIONS__, tab, jni_double_vector, defaultNumberTicks);
  setGraphicObjectProperty(paxesmdl->UID, __GO_Y_AXIS_TICKS_LOCATIONS__, tab, jni_double_vector, defaultNumberTicks);

  stringVector = createStringArray(defaultNumberTicks);

  if (stringVector == NULL)
  {
    return -1;
  }

  /*
   * A proper format should be used (ChoixFormatE function)
   */
  for (i = 0; i < defaultNumberTicks; i++)
  {
    sprintf(labelBuffer, "%.1f", tab[i]);
    stringVector[i] = strdup(labelBuffer);

    if (stringVector == NULL)
    {
      return -1;
    }
  }

  setGraphicObjectProperty(paxesmdl->UID, __GO_X_AXIS_TICKS_LABELS__, stringVector, jni_string_vector, defaultNumberTicks);
  setGraphicObjectProperty(paxesmdl->UID, __GO_Y_AXIS_TICKS_LABELS__, stringVector, jni_string_vector, defaultNumberTicks);

  /*
   * The same number of ticks is used now for the x,y and z axes.
   * Previously, the z-axis contained only 3 ticks (-1, 0, 1). However, the renderer module was
   * overriding this default number (3) by creating an 11-tick z-axis when required (3D view).
   * Ticks locations and labels are however different for the z-axis (from -1 to +1 instead of 0 to 1).
   */
  setGraphicObjectProperty(paxesmdl->UID, __GO_Z_AXIS_TICKS_LOCATIONS__, tabZTicksLocations, jni_double_vector, defaultNumberTicks);

  /* ChoixFormatE should be used */
  for (i = 0; i < defaultNumberTicks; i++)
  {
    FREE(stringVector[i]);

    sprintf(labelBuffer, "%.1f", tabZTicksLocations[i]);
    stringVector[i] = strdup(labelBuffer);

    if (stringVector == NULL)
    {
      return -1;
    }
  }

  setGraphicObjectProperty(paxesmdl->UID, __GO_Z_AXIS_TICKS_LABELS__, stringVector, jni_string_vector, defaultNumberTicks);

  destroyStringArray(stringVector, defaultNumberTicks);

  /*
   * Initializing Font properties must be done after the labels have been set
   * as it directly sets the Labels' font properties.
   * To be modified.
   */
  sciInitFontContext (paxesmdl);  /* F.Leray 10.06.04 */

  /*
   * Indicates the direction of projection (0 for the axis corresponding to the direction,
   * 1 for the others).
   * To be implemented within the MVC if determined to be useful.
   */
#if 0
  ppaxesmdl->project[0]= 1;
  ppaxesmdl->project[1]= 1;
  ppaxesmdl->project[2]= 0;
#endif


  hiddenAxisColor = 4;
  setGraphicObjectProperty(paxesmdl->UID, __GO_HIDDEN_AXIS_COLOR__, &hiddenAxisColor, jni_int, 1);

  hiddenColor = 4;
  setGraphicObjectProperty(paxesmdl->UID, __GO_HIDDEN_COLOR__, &hiddenColor, jni_int, 1);

  isoview = 0;
  setGraphicObjectProperty(paxesmdl->UID, __GO_ISOVIEW__, &isoview, jni_bool, 1);

  /* Axes bounds set to fill the whole drawing area */
  axesBounds[0] = 0.0;
  axesBounds[1] = 0.0;
  axesBounds[2] = 1.0;
  axesBounds[3] = 1.0;
  setGraphicObjectProperty(paxesmdl->UID, __GO_AXES_BOUNDS__, axesBounds, jni_double_vector, 4);

  /* xmin, xmax */
  dataBounds[0] = 0.0;
  dataBounds[1] = 1.0;
  /* ymin, ymax */
  dataBounds[2] = 0.0;
  dataBounds[3] = 1.0;
  /* zmin, zmax */
  dataBounds[4] = -1.0;
  dataBounds[5] = 1.0;

  setGraphicObjectProperty(paxesmdl->UID, __GO_DATA_BOUNDS__, dataBounds, jni_double_vector, 6);

  /* visible */
  getGraphicObjectProperty(pfiguremdl->UID, __GO_VISIBLE__, jni_bool, &piVisible);
  setGraphicObjectProperty(paxesmdl->UID, __GO_VISIBLE__, &visible, jni_bool, 1);

  /* 0: clipping off */
  clipState = 0;
  setGraphicObjectProperty(paxesmdl->UID, __GO_CLIP_STATE__, &clipState, jni_int, 1);

  /* "real data bounds" and "data bounds" are initially the same */
  setGraphicObjectProperty(paxesmdl->UID, __GO_REAL_DATA_BOUNDS__, dataBounds, jni_double_vector, 6);

  tightLimits = 0;
  setGraphicObjectProperty(paxesmdl->UID, __GO_TIGHT_LIMITS__, &tightLimits, jni_bool, 1);

  /* Sets the default arc drawing method to lines (1), which is faster */
  arcDrawingMethod = 1;
  setGraphicObjectProperty(paxesmdl->UID, __GO_ARC_DRAWING_METHOD__, &arcDrawingMethod, jni_int, 1);

  return 0;

  /*
   * Former non-MVC property sets
   * Commented out and not deleted since a few properties are not implemented yet:
   * project, user_data, user_grads, and Label default properties.
  */

#if 0
  ppaxesmdl->axes.ticscolor  = -1;
#endif

  /* Deactivated for now */
#if 0
  ppaxesmdl->callback = (char *)NULL;
  ppaxesmdl->callbacklen = 0;
  ppaxesmdl->callbackevent = 100;
#endif

  /*
   * subint has apparently been superseded by nbsubtics (set below),
   * which itself corresponds to the MVC's __GO_{X,Y,Z}_AXIS_SUBTICKS__
   */
#if 0
  sciInitLogFlags(paxesmdl, linLogFlags);
  ppaxesmdl->axes.ticscolor  = -1;
  ppaxesmdl->axes.subint[0]  = 1;
  ppaxesmdl->axes.subint[1]  = 1;
  ppaxesmdl->axes.subint[2]  = 1;
#endif

#if 0
  ppaxesmdl->axes.xdir='d';
  ppaxesmdl->axes.ydir='l';
  ppaxesmdl->axes.rect  = BT_OFF;
  sciInitIsFilled(paxesmdl, TRUE);
#endif

  /* Not implemented yet within the MVC */
#if 0
  ppaxesmdl->user_data = (int *) NULL; /* to be complete */
  ppaxesmdl->size_of_user_data = 0;
#endif

 /*
  * This has apparently has been useless for a long time
  * as it is not referred to anywhere else than here.
  */
#if 0
  for (i=0 ; i<7 ; i++)
  {
    ppaxesmdl->axes.limits[i] = 0;
  }
#endif

#if 0
  for (i=0 ; i<3 ; i++)
  {
    ppaxesmdl->grid[i]  = -1;
  }
#endif

#if 0
	ppaxesmdl->gridFront = FALSE; /* draw in background */
#endif


#if 0
  ppaxesmdl->alpha  = 0.0;
  ppaxesmdl->theta  = 270.0;
#endif

#if 0
  ppaxesmdl->alpha_kp  = 45.0;
  ppaxesmdl->theta_kp  = 215.0;
#endif

#if 0
  ppaxesmdl->is3d  = FALSE;
#endif

  /* F.Leray 22.09.04 */
#if 0
  (ppaxesmdl->axes).axes_visible[0] = FALSE;
  (ppaxesmdl->axes).axes_visible[1] = FALSE;
  (ppaxesmdl->axes).axes_visible[2] = FALSE;
  (ppaxesmdl->axes).reverse[0] = FALSE;
  (ppaxesmdl->axes).reverse[1] = FALSE;
  (ppaxesmdl->axes).reverse[2] = FALSE;
#endif

  /*
   * Stores the auto sub-ticks internal state
   */
#if 0
  ppaxesmdl->flagNax = FALSE;

  /*F.Leray : just for completion : */
  ppaxesmdl->axes.nbsubtics[0] = 1; /* not used at all because needs ppaxesmdl->flagNax = TRUE */
  ppaxesmdl->axes.nbsubtics[1] = 1; /* and when it is TRUE, it means WE have given the corresponding */
  ppaxesmdl->axes.nbsubtics[2] = 1; /* ppaxesmdl->axes.nbsubtics[0,1,2] !! */
#endif

  /* Former graduations, 11 z-axis ticks are now used for the MVC's default axes */
#if 0
  (ppaxesmdl->axes).nxgrads = 11; /* computed ticks */
  (ppaxesmdl->axes).nygrads = 11;
  (ppaxesmdl->axes).nzgrads = 3;

  for(i=0;i<11;i++)
  {
    ppaxesmdl->axes.xgrads[i] = tab[i];
  }

  for(i=0;i<11;i++)
  {
    ppaxesmdl->axes.ygrads[i] = tab[i];
  }

  (ppaxesmdl->axes).zgrads[0] = -1.;
  (ppaxesmdl->axes).zgrads[1]  = 0.;
  (ppaxesmdl->axes).zgrads[2]  = 1.;
#endif

  /* Users grads are not implemented yet */
#if 0
  (ppaxesmdl->axes).u_xgrads = (double *)NULL;
  (ppaxesmdl->axes).u_ygrads = (double *)NULL;
  (ppaxesmdl->axes).u_zgrads = (double *)NULL;
  (ppaxesmdl->axes).u_nxgrads = 0; /* user ticks */
  (ppaxesmdl->axes).u_nygrads = 0;
  (ppaxesmdl->axes).u_nzgrads = 0;
  (ppaxesmdl->axes).u_xlabels= (char **) NULL;
  (ppaxesmdl->axes).u_ylabels= (char **) NULL;
  (ppaxesmdl->axes).u_zlabels= (char **) NULL;
#endif

#if 0
  sciInitAutoTicks(paxesmdl, TRUE, TRUE, TRUE);
  /* end 22.09.04 */
#endif

  /*
   * The flag array internal state is apparently deprecated.
   * flag[0] is related to the treatment of hidden parts and apparently deprecated (To be checked).
   * flag[1] is related to scaling and apparently deprecated (To be checked).
   * flag[2] corresponds to the MVC's GO_BOX_TYPE property
   */
#if 0
  ppaxesmdl->axes.flag[0]= 2;
  ppaxesmdl->axes.flag[1]= 2;
  ppaxesmdl->axes.flag[2]= 4;
  sciInitHiddenAxisColor(paxesmdl, 4);
#endif

  /*
   * The project boolean array (internal state) serves to indicate whether projection is parallel
   * to one of the {X,Y,Z} axes (0 for the axis corresponding to the direction of projection,
   * 1 for the others). It may still be used by the high-level drawing commands (plot, etc).
   * To be implemented within the MVC.
   */
#if 0
  ppaxesmdl->project[0]= 1;
  ppaxesmdl->project[1]= 1;
  ppaxesmdl->project[2]= 0;
  sciInitHiddenColor(paxesmdl, 4);
  ppaxesmdl->isoview= FALSE;/*TRUE;*/
#endif

#if 0
  ppaxesmdl->WRect[0]   = 0;
  ppaxesmdl->WRect[1]   = 0;
  ppaxesmdl->WRect[2]   = 1;
  ppaxesmdl->WRect[3]   = 1;
  ppaxesmdl->FRect[0]   = 0.0; /* xmin */
  ppaxesmdl->FRect[1]   = 0.0; /* ymin */
  ppaxesmdl->FRect[2]   = 1.0; /* xmax */
  ppaxesmdl->FRect[3]   = 1.0; /* ymax */
  ppaxesmdl->FRect[4]   = -1.0;/* zmin */
  ppaxesmdl->FRect[5]   = 1.0;   /* zmax */
  ppaxesmdl->visible = sciGetVisibility(pfiguremdl);
  /* /\*   ppaxesmdl->drawlater = sciGetDrawLater(pfiguremdl); *\/ */
  /*   ppaxesmdl->drawlater = FALSE; */
#endif

#if 0
  ppaxesmdl->isclip = -1; /* off */

  /* Les SRect sont rentres dans l'ordre:
     [xmin ymin zmin xmax ymax zmax] */
  ppaxesmdl->SRect[0]   = 0.0; /* xmin */
  ppaxesmdl->SRect[1]   = 1.0; /* xmax */
  ppaxesmdl->SRect[2]   = 0.0; /* ymin */
  ppaxesmdl->SRect[3]   = 1.0; /* ymax */
  ppaxesmdl->SRect[4]   = -1.0;/* zmin */
  ppaxesmdl->SRect[5]   = 1.0;  /* zmax */

  ppaxesmdl->tight_limits = FALSE;

  /* By default arcs don't use nurbs since it is faster */
  sciInitUseNurbs(paxesmdl, FALSE);

  paxesmdl->pObservers = NULL ;
  paxesmdl->pDrawer = NULL ;
#endif


  /*
   * Label creation is done in the MVC Axes constructor for now.
   * However, the equivalent of initLabel (which initializes Label
   * properties default values) must be implemented.
   */

  /* F.Leray 10.06.04 */
  /* Adding default Labels inside Axes */
  /*---------------------------------------------------------------------------*/

   /******************************  title *************************/
#if 0
  ppaxesmdl->mon_title = initLabel( paxesmdl ) ;

  if ( ppaxesmdl->mon_title == NULL )
  {
    return -1 ;
  }

  pLABEL_FEATURE(ppaxesmdl->mon_title)->ptype = 1;

  /******************************  x_label *************************/

  ppaxesmdl->mon_x_label = initLabel( paxesmdl ) ;

  if ( ppaxesmdl->mon_x_label == NULL )
  {
    return -1 ;
  }

  pLABEL_FEATURE( ppaxesmdl->mon_x_label )->ptype = 2 ;

  /******************************  y_label *************************/

  ppaxesmdl->mon_y_label = initLabel( paxesmdl ) ;

  if ( ppaxesmdl->mon_y_label == NULL )
  {
    return -1 ;
  }

  pLABEL_FEATURE( ppaxesmdl->mon_y_label )->ptype = 3 ;

  /******************************  z_label *************************/

  ppaxesmdl->mon_z_label = initLabel( paxesmdl ) ;

  if ( ppaxesmdl->mon_z_label == NULL )
  {
    return -1 ;
  }

  pLABEL_FEATURE( ppaxesmdl->mon_z_label )->ptype = 4 ;
#endif

  return 0;
}



int ResetFigureToDefaultValues(sciPointObj * pobj)
{

  int x[2];


  if(sciGetEntityType(pobj)!=SCI_FIGURE) /* MUST BE used for figure entities only */
    return -1;

  /** Initialize the colormap */
  /* try to install the colormap in the graphic context */
  sciSetColormap( pobj, pFIGURE_FEATURE(pfiguremdl)->pModelData->colorMap, sciGetNumColors(pfiguremdl), 3 ) ;


  /* initialisation de context et mode graphique par defaut (figure model)*/
  if (sciInitGraphicContext (pobj) == -1)
    {
      sciDelHandle (pobj);
      FREE(pobj->pfeatures);
      FREE(pobj);
      return -1;
    }
  if (sciInitGraphicMode (pobj) == -1)
    {
      sciDelHandle (pobj);
      FREE(pobj->pfeatures);
      FREE(pobj);
      return -1;
    }

  /* F.Leray 08.04.04 */
  if (sciInitFontContext (pobj) == -1)
    {
      sciDelHandle (pobj);
      FREE(pobj->pfeatures);
      FREE(pobj);
      return -1;
    }

  sciInitName(pobj, sciGetName(pfiguremdl));
  sciSetResize(pobj,sciGetResize(pfiguremdl));
	sciSetDimension( pobj, sciGetWidth(pfiguremdl), sciGetHeight(pfiguremdl) );
	if (!sciGetResize(pobj))
	{
		/* window size and axes size may change independently */
		sciSetWindowDim( pobj, sciGetWindowWidth(pfiguremdl), sciGetWindowHeight(pfiguremdl) ) ;
	}

  sciGetScreenPosition(pfiguremdl, &x[0], &x[1]) ;
  sciSetScreenPosition(pobj,x[0],x[1]);
  pFIGURE_FEATURE (pobj)->isselected = pFIGURE_FEATURE (pfiguremdl)->isselected;
  pFIGURE_FEATURE (pobj)->rotstyle = pFIGURE_FEATURE (pfiguremdl)->rotstyle;
  pFIGURE_FEATURE (pobj)->visible = pFIGURE_FEATURE (pfiguremdl)->visible;
  sciInitImmediateDrawingMode(pobj, sciGetImmediateDrawingMode(pfiguremdl));
  pFIGURE_FEATURE (pobj)->numsubwinselected = pFIGURE_FEATURE (pfiguremdl)->numsubwinselected;
  sciInitPixmapMode(pobj, sciGetPixmapMode(pfiguremdl));

  return 0;
}


/*
 * This function has been adapted to the MVC framework.
 * Its code ought to be moved to the Java Model implementation,
 * either within the relevant constructor or an initialization method.
 */

/**sciInitGraphicMode
 * Inits the graphic mode of this object with the default value
 */
int
sciInitGraphicMode (sciPointObj * pobj)
{
  char* type;

  getGraphicObjectProperty(pobj->UID, __GO_TYPE__, jni_string, &type);

//  switch (sciGetEntityType (pobj))

    /*
     * The GO_FIGURE block is never reached as InitFigureModel
     * is not called at all (was previously called by
     * the graphicsmodels function).
     */
    if (strcmp(type, __GO_FIGURE__) == 0)
    {
      /* 3: copy pixel drawing mode */
      int xormode = 3;

      if (pobj == pfiguremdl)
	{
          /*
           * These 3 properties are not used by the Figure object proper, but
           * rather serve to initialize its children Axes' ones.
           */
#if 0
	  (sciGetGraphicMode (pobj))->addplot = TRUE;
	  (sciGetGraphicMode (pobj))->autoscaling = TRUE;
	  (sciGetGraphicMode (pobj))->zooming = FALSE;
#endif

          setGraphicObjectProperty(pobj->UID, __GO_PIXEL_DRAWING_MODE__, &xormode, jni_int, 1);

#if 0
	  (sciGetGraphicMode (pobj))->xormode = 3; /* copy */
#endif
	}
      /*
       * Useless now since the Figure model
       * is cloned within the MVC via a C call
       */
      else
	{
#if 0
	  (sciGetGraphicMode (pobj))->addplot = (sciGetGraphicMode (pfiguremdl))->addplot;
	  (sciGetGraphicMode (pobj))->autoscaling = (sciGetGraphicMode (pfiguremdl))->autoscaling;
	  (sciGetGraphicMode (pobj))->zooming = (sciGetGraphicMode (pfiguremdl))->zooming;
	  (sciGetGraphicMode (pobj))->xormode = (sciGetGraphicMode (pfiguremdl))->xormode;
#endif
	}
    }
    else if (strcmp(type, __GO_AXES__) == 0)
    {
      /*
       * Same values as the ones from the Figure model. These values were copied from the parent
       * Figure but are for now set using the values below.
       */

      /* autoClear is the logical not of addPlot (autoClear == 0 corresponds to addPlot == TRUE) */
      int autoClear = 0;
      int autoScale = 1;
      int zoom = 0;
      /* 3: copy */
      int xormode = 3;

      if (pobj == paxesmdl)
	{
          setGraphicObjectProperty(pobj->UID, __GO_AUTO_CLEAR__, &autoClear, jni_bool, 1);
          setGraphicObjectProperty(pobj->UID, __GO_AUTO_SCALE__, &autoScale, jni_bool, 1);
          setGraphicObjectProperty(pobj->UID, __GO_ZOOM_ENABLED__, &zoom, jni_bool, 1);

	  /*
           * Internal state: was possibly used to avoid accessing the parent Figure's pixel drawing mode
           * or may be entirely useless, as pixel drawing mode is associated to the whole Figure.
           * As it has no corresponding MVC property, this call will not set anything.
           */
          setGraphicObjectProperty(pobj->UID, __GO_PIXEL_DRAWING_MODE__, &xormode, jni_int, 1);

#if 0
	  (sciGetGraphicMode (pobj))->addplot =sciGetAddPlot (sciGetParent (pobj));
	  (sciGetGraphicMode (pobj))->autoscaling =sciGetAutoScale (sciGetParent (pobj));
	  (sciGetGraphicMode (pobj))->zooming =sciGetZooming (sciGetParent (pobj));
	  (sciGetGraphicMode (pobj))->xormode =sciGetXorMode (sciGetParent (pobj));
#endif
      	}
      /*
       * This block is never reached at all since since the Axes model
       * is now cloned within the MVC via a C call.
       */
      else
	{
        int iTmp = 0;
          int* piTmp = &iTmp;

          getGraphicObjectProperty(paxesmdl->UID, __GO_AUTO_CLEAR__, jni_bool, &piTmp);
          autoClear = iTmp;
          getGraphicObjectProperty(paxesmdl->UID, __GO_AUTO_SCALE__, jni_bool, &piTmp);
          autoScale = iTmp;
          getGraphicObjectProperty(paxesmdl->UID, __GO_ZOOM_ENABLED__, jni_bool, &piTmp);
          zoom = iTmp;

          setGraphicObjectProperty(pobj->UID, __GO_AUTO_CLEAR__, &autoClear, jni_bool, 1);
          setGraphicObjectProperty(pobj->UID, __GO_AUTO_SCALE__, &autoScale, jni_bool, 1);
          setGraphicObjectProperty(pobj->UID, __GO_ZOOM_ENABLED__, &zoom, jni_bool, 1);

	  /*
           * Internal state: used to avoid accessing the parent's pixel drawing mode
           * obsolete ? Not implemented yet within the MVC
           */

          getGraphicObjectProperty(paxesmdl->UID, __GO_PIXEL_DRAWING_MODE__, jni_bool, &piTmp);
          xormode = iTmp;

          setGraphicObjectProperty(pobj->UID, __GO_PIXEL_DRAWING_MODE__, &xormode, jni_int, 1);

#if 0
	  (sciGetGraphicMode (pobj))->addplot =(sciGetGraphicMode (paxesmdl))->addplot;
	  (sciGetGraphicMode (pobj))->autoscaling = (sciGetGraphicMode (paxesmdl))->autoscaling;
	  (sciGetGraphicMode (pobj))->zooming =(sciGetGraphicMode (paxesmdl))->zooming;
	  (sciGetGraphicMode (pobj))->xormode =(sciGetGraphicMode (paxesmdl))->xormode;
#endif
      	}
    }

    /* Deactivated */
#if 0
    case SCI_TEXT:
    case SCI_LEGEND:
    case SCI_ARC:
    case SCI_SEGS:
    case SCI_FEC:
    case SCI_GRAYPLOT:
    case SCI_POLYLINE:
    case SCI_RECTANGLE:
    case SCI_SURFACE:
    case SCI_AXES:
    case SCI_AGREG:
    case SCI_LABEL: /* F.Leray 28.05.04 */
    case SCI_UIMENU:
    default:
      sciprint (_("This object has not any graphic mode\n"));
      return -1;
      break;
#endif

  return 0;
}

/*---------------------------------------------------------------------------------*/
/* allocate and set a new label to default values */
sciPointObj * initLabel( sciPointObj * pParentObj )
{
  sciPointObj * newLabel = MALLOC (sizeof (sciPointObj)) ;
  sciLabel    * ppLabel ;
  char        * emptyString = "" ;

  if ( newLabel == NULL )
  {
    return NULL ;
  }

  sciSetEntityType( newLabel, SCI_LABEL ) ;

  newLabel->pfeatures = MALLOC ( sizeof (sciLabel) ) ;
  if ( newLabel->pfeatures == NULL)
  {
    FREE( newLabel );
    return NULL ;
  }


  ppLabel = pLABEL_FEATURE( newLabel ) ;

  /* we must first construct the text object inside the label */
  ppLabel->text = allocateText( pParentObj, &emptyString, 1, 1,
                                0.0, 0.0, TRUE, NULL, FALSE, NULL, NULL,
                                FALSE, FALSE, FALSE, ALIGN_LEFT ) ;

	/* RelationShip is actually stored in the text object */
	newLabel->relationShip = ppLabel->text->relationShip;

  if ( ppLabel->text == NULL )
  {
    deallocateText( ppLabel->text ) ;
    FREE( ppLabel ) ;
    FREE( newLabel  );
    return NULL ;
  }

  if ( sciAddNewHandle ( newLabel ) == -1 )
  {
    FREE( ppLabel ) ;
    FREE( newLabel ) ;
    return NULL ;
  }

  if ( !sciAddThisToItsParent( newLabel, pParentObj ) )
  {
    sciDelHandle (newLabel);
    FREE( ppLabel ) ;
    FREE( newLabel  );
    return NULL ;
  }

  ppLabel->auto_position = TRUE;
  ppLabel->auto_rotation = TRUE;

  ppLabel->isselected = TRUE;

  ppLabel->ptype = 0 ; /* must be changed : 1 for title, 2 x_label, 3 y_label, 4 z_label */

  sciInitIs3d( newLabel, FALSE ) ;

  newLabel->pObservers = NULL ;
  newLabel->pDrawer = NULL ;

  if ( sciInitGraphicContext( newLabel ) == -1 )
  {
    DestroyLabel( newLabel ) ;
    return NULL ;
  }

  if ( sciInitFontContext( newLabel ) == -1 )
  {
    DestroyLabel( newLabel ) ;
    return NULL ;
  }



  return newLabel ;
}

/*---------------------------------------------------------------------------------*/
void destroyDefaultObjects( void )
{
  /* will destroy the figure and its children (so the axes). */
  destroyGraphicHierarchy( pfiguremdl ) ;
  pfiguremdl = NULL ;
  paxesmdl = NULL;
}
/*---------------------------------------------------------------------------------*/
/**
 * Create new data with defautl properties.
 */
FigureModelData * newFigureModelData( void )
{
  FigureModelData * modelData = MALLOC(sizeof(FigureModelData)) ;

  if ( modelData == NULL ) { return NULL ; }

  modelData->figureWidth  = 610 ;
  modelData->figureHeight = 460 ;
  modelData->windowWidth  = 620 ;
  modelData->windowHeight = 590 ;
  modelData->windowPosition[0] = 200 ; /* Set [-1,-1] to let the os use the position */
  modelData->windowPosition[1] = 200 ;
  modelData->colorMap = NULL ;
  modelData->numColors = 0 ;
  modelData->autoResizeMode = TRUE;
  modelData->viewport[0] = 0;
  modelData->viewport[1] = 0;
  modelData->viewport[2] = 610;
  modelData->viewport[3] = 461;
	modelData->infoMessage = NULL;
	modelData->antialiasingQuality = 0;

  return modelData ;
}
/*---------------------------------------------------------------------------------*/
/**
 * Free an existing model Data
 */
void destroyFigureModelData( FigureModelData * data )
{
  if ( data != NULL )
  {
		if (data->colorMap != NULL)
		{
			FREE(data->colorMap);
			data->colorMap = NULL;
		}

		if (data->infoMessage != NULL)
		{
			FREE(data->infoMessage);
			data->infoMessage = NULL;
		}

    FREE( data ) ;
    data = NULL ;
  }
}
/*---------------------------------------------------------------------------------*/
/**
 * Set the colormap of a figure to the default one.
 */
void sciSetDefaultColorMap(sciPointObj * pFigure)
{
  int numColor = sciGetNumColors(getFigureModel());
  double * colorMap = MALLOC( 3 * numColor * sizeof(double) );
  if (colorMap == NULL)
  {
    sciprint(_("%s: No more memory.\n"),"sciSetDefaultColorMap");
    return;
  }
  sciGetColormap(getFigureModel(), colorMap);
  sciSetColormap(pFigure, colorMap, numColor, 3);

  FREE(colorMap);
}
/*---------------------------------------------------------------------------------*/
/**
 * @return TRUE if pObj is one of the model objects, FALSE otherwise
 */
BOOL isModelObject(sciPointObj * pObj)
{
	return pObj == pfiguremdl
		|| pObj == paxesmdl
		|| pObj == pSUBWIN_FEATURE(paxesmdl)->mon_title
		|| pObj == pSUBWIN_FEATURE(paxesmdl)->mon_x_label
		|| pObj == pSUBWIN_FEATURE(paxesmdl)->mon_y_label
		|| pObj == pSUBWIN_FEATURE(paxesmdl)->mon_z_label;
}
/*---------------------------------------------------------------------------------*/
