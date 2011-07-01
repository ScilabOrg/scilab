/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2001-2002 - INRIA - Mathieu Philippe
 * Copyright (C) 2002-2004 - INRIA - Djalel Abdemouche
 * Copyright (C) 2004-2006 - INRIA - Fabrice Leray
 * Copyright (C) 2005 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2007 - INRIA - Vincent Couvert
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
 *    This file contains all functions used to BUILD new objects :
 - allocating memory
 - setting default value
 - binding the newly created object tyo the entire existing hierarchy
 --------------------------------------------------------------------------*/
#include <string.h>
#include <stdio.h>

#include "BuildObjects.h"
#include "GetProperty.h"
#include "InitObjects.h"
#include "DestroyObjects.h"
#include "SetProperty.h"
#include "CloneObjects.h"
#include "StringMatrix.h"
#include "Scierror.h"
#include "CurrentFigure.h"
#include "ObjectSelection.h"
#include "BuildDrawingObserver.h"
#include "FigureList.h"
#include "localization.h"
#include "GraphicSynchronizerInterface.h"
#include "Interaction.h"
#include "ColorMapManagement.h"
#include "get_ticks_utils.h"
#include "HandleManagement.h"
#include "loadTextRenderingAPI.h"

#include "MALLOC.h" /* MALLOC */
#include "Scierror.h"

#include "createGraphicObject.h"
#include "deleteGraphicObject.h"
#include "returnType.h"
#include "getGraphicObjectProperty.h"
#include "setGraphicObjectProperty.h"
#include "graphicObjectProperties.h"
#include "callJoGLView.h"

#include "FigureModel.h"
#include "AxesModel.h"

/**
 * If a current figure exists : return it
 * Otherwise create a new one.
 */
GRAPHICS_IMPEXP char *createNewFigureWithAxes()
{
    int iID = 0;
    char *pFigureUID = NULL;
    char *pAxesUID = NULL;

    pFigureUID = cloneGraphicObject(getFigureModel());
    setGraphicObjectProperty(pFigureUID, __GO_ID__, &iID, jni_int, 1);
    /*
     * Clones a new Axes object using the Axes model which is then
     * attached to the newly created Figure.
     */
    pAxesUID = cloneGraphicObject(getAxesModel());

    /* Sets the parent-child relationship within the MVC */
    setGraphicObjectRelationship(pFigureUID, pAxesUID);

    /* Sets the newly created Axes as the Figure's current selected child */
    setGraphicObjectProperty(pFigureUID, __GO_SELECTED_CHILD__, pAxesUID, jni_string, 1);

    setCurrentFigure(pFigureUID);
    setCurrentObject(pAxesUID);
    setCurrentSubWin(pAxesUID);

    return pFigureUID;
}


/*-----------------------------------------------------------------------------*/

/**ConstructSubWin
 * This function creates the Subwindow (the Axes) and the elementary structures.
 * It has been adapted to the MVC: all the initialization operations are now performed
 * within the Model. The update of color properties (foreground, background, etc.)
 * according to the assigned parent Figure's colormap is not implemented yet.
 * To be implemented.
 */
sciPointObj *
ConstructSubWin(sciPointObj * pparentfigure)
{
    /*
     * These variables were used by the now commented out
     * initialization code.
     * To be deleted
     */
#if 0
    char logFlags[3];
    int i;
    char dir;
    BOOL autoTicks[3];
    sciSubWindow * ppsubwin = NULL;
    sciSubWindow * ppaxesmdl = pSUBWIN_FEATURE (paxesmdl);
#endif

    char* parentType;

    sciPointObj *pClone = (sciPointObj *) NULL;
    sciPointObj * paxesmdl = getAxesModel();

    getGraphicObjectProperty(pparentfigure->UID, __GO_TYPE__, jni_string, &parentType);

    if (strcmp(parentType, __GO_FIGURE__) != 0)
    {
        Scierror(999, _("The parent has to be a FIGURE\n"));
        return (sciPointObj*) NULL;
    }

    pClone = sciCloneObj(paxesmdl);

//    if ( sciAddNewHandle(pClone) == -1 )
//    {
//        deleteGraphicObject(pClone->UID);
//        FREE(pClone);
//        return (sciPointObj*) NULL;
//    }

    setGraphicObjectRelationship(pparentfigure->UID, pClone->UID);

    return pClone;

   /*
    * The following operations are now performed within the Model
    * when cloning the Axes from the Axes model.
    * To be deleted
    */
#if 0
	if (sciGetEntityType (pparentfigure) == SCI_FIGURE)
	{

		if ((pobj = MALLOC ((sizeof (sciPointObj)))) == NULL)
			return NULL;
		sciSetEntityType (pobj, SCI_SUBWIN);
		if ((pobj->pfeatures = MALLOC ((sizeof (sciSubWindow)))) == NULL)
		{
			FREE(pobj);
			return (sciPointObj *) NULL;
		}

		startFigureDataWriting(pparentfigure);

		if ( sciStandardBuildOperations( pobj, pparentfigure ) == NULL )
		{
			FREE( pobj->pfeatures ) ;
			FREE( pobj ) ;
			endFigureDataWriting(pparentfigure);
			return NULL ;
		}

		ppsubwin =  pSUBWIN_FEATURE (pobj); /* debug */


		ppsubwin->callback = (char *)NULL;
		ppsubwin->callbacklen = 0;
		ppsubwin->callbackevent = 100;

		if (sciInitGraphicContext (pobj) == -1)
		{
			//sciDelThisToItsParent (pobj, sciGetParent (pobj));
			// sciDelHandle (pobj);
			FREE(pobj->pfeatures);
			FREE(pobj);
			endFigureDataWriting(pparentfigure);
			return (sciPointObj *) NULL;
		}
		if (sciInitGraphicMode (pobj) == -1)
		{
			//sciDelThisToItsParent (pobj, sciGetParent (pobj));
			// sciDelHandle (pobj);
			FREE(pobj->pfeatures);
			FREE(pobj);
			endFigureDataWriting(pparentfigure);
			return (sciPointObj *) NULL;
		}

		/* F.Leray 08.04.04 */
		if (sciInitFontContext (pobj) == -1)
		{
			//sciDelThisToItsParent (pobj, sciGetParent (pobj));
			// sciDelHandle (pobj);
			FREE(pobj->pfeatures);
			FREE(pobj);
			endFigureDataWriting(pparentfigure);
			return (sciPointObj *) NULL;
		}

		/* update colors so they will fit the colormap of parent figure and not model one*/
		sciRecursiveUpdateBaW(pobj, sciGetNumColors(getFigureModel()), sciGetNumColors(sciGetParentFigure(pobj)));

		sciGetLogFlags(paxesmdl, logFlags);
		sciInitLogFlags(pobj, logFlags);

		ppsubwin->axes.ticscolor  = ppaxesmdl->axes.ticscolor;
		ppsubwin->axes.subint[0]  = ppaxesmdl->axes.subint[0];
		ppsubwin->axes.subint[1]  = ppaxesmdl->axes.subint[1];
		ppsubwin->axes.subint[2]  = ppaxesmdl->axes.subint[2];

		dir= ppaxesmdl->axes.xdir;
		ppsubwin->axes.xdir = dir;
		dir= ppaxesmdl->axes.ydir;
		ppsubwin->axes.ydir = dir;

		ppsubwin->axes.rect  = ppaxesmdl->axes.rect;
		sciInitIsFilled(pobj, sciGetIsFilled(paxesmdl));
		for (i=0 ; i<7 ; i++)
		{
			ppsubwin->axes.limits[i]  = ppaxesmdl->axes.limits[i] ;
		}

		for (i=0 ; i<3 ; i++)
		{
			ppsubwin->grid[i]  = ppaxesmdl->grid[i] ;
		}

		ppsubwin->gridFront = ppaxesmdl->gridFront;

		ppsubwin->alpha  = ppaxesmdl->alpha;
		ppsubwin->theta  = ppaxesmdl->theta;
		ppsubwin->alpha_kp  = ppaxesmdl->alpha_kp;
		ppsubwin->theta_kp  = ppaxesmdl->theta_kp;
		ppsubwin->is3d  = ppaxesmdl->is3d;

		/* F.Leray 22.09.04 */

		ppsubwin->axes.u_xlabels = (char **) NULL; /* init. ci-apres */
		ppsubwin->axes.u_ylabels = (char **) NULL; /* init. ci-apres */
		ppsubwin->axes.u_zlabels = (char **) NULL; /* init. ci-apres */
		ppsubwin->axes.u_xgrads  = (double *)NULL;
		ppsubwin->axes.u_ygrads  = (double *)NULL;
		ppsubwin->axes.u_zgrads  = (double *)NULL;


		(ppsubwin->axes).axes_visible[0] = ppaxesmdl->axes.axes_visible[0];
		(ppsubwin->axes).axes_visible[1] = ppaxesmdl->axes.axes_visible[1];
		(ppsubwin->axes).axes_visible[2] = ppaxesmdl->axes.axes_visible[2];
		(ppsubwin->axes).reverse[0] = ppaxesmdl->axes.reverse[0];
		(ppsubwin->axes).reverse[1] = ppaxesmdl->axes.reverse[1];
		(ppsubwin->axes).reverse[2] = ppaxesmdl->axes.reverse[2];
		ppsubwin->flagNax = ppaxesmdl->flagNax;

		/* do not forget the nbsubtics ! */
		(ppsubwin->axes).nbsubtics[0] = ppaxesmdl->axes.nbsubtics[0];
		(ppsubwin->axes).nbsubtics[1] = ppaxesmdl->axes.nbsubtics[1];
		(ppsubwin->axes).nbsubtics[2] = ppaxesmdl->axes.nbsubtics[2];

		(ppsubwin->axes).nxgrads = ppaxesmdl->axes.nxgrads;
		(ppsubwin->axes).nygrads = ppaxesmdl->axes.nygrads;
		(ppsubwin->axes).nzgrads = ppaxesmdl->axes.nzgrads;

		for(i=0;i<(ppsubwin->axes).nxgrads;i++) ppsubwin->axes.xgrads[i] = ppaxesmdl->axes.xgrads[i];
		for(i=0;i<(ppsubwin->axes).nygrads;i++) ppsubwin->axes.ygrads[i] = ppaxesmdl->axes.ygrads[i];
		for(i=0;i<(ppsubwin->axes).nzgrads;i++) ppsubwin->axes.zgrads[i] = ppaxesmdl->axes.zgrads[i];

		(ppsubwin->axes).u_nxgrads = ppaxesmdl->axes.u_nxgrads;
		(ppsubwin->axes).u_nygrads = ppaxesmdl->axes.u_nygrads;
		(ppsubwin->axes).u_nzgrads = ppaxesmdl->axes.u_nzgrads;
		(ppsubwin->axes).u_xgrads  = AllocUserGrads(ppsubwin->axes.u_xgrads,ppaxesmdl->axes.u_nxgrads);
		CopyUserGrads(ppaxesmdl->axes.u_xgrads,
			ppsubwin->axes.u_xgrads,
			ppsubwin->axes.u_nxgrads);
		(ppsubwin->axes).u_ygrads  = AllocUserGrads(ppsubwin->axes.u_ygrads,ppaxesmdl->axes.u_nygrads);
		CopyUserGrads(ppaxesmdl->axes.u_ygrads,
			ppsubwin->axes.u_ygrads,
			ppsubwin->axes.u_nygrads);
		(ppsubwin->axes).u_zgrads  = AllocUserGrads(ppsubwin->axes.u_zgrads,ppaxesmdl->axes.u_nzgrads);
		CopyUserGrads(ppaxesmdl->axes.u_zgrads,
			ppsubwin->axes.u_zgrads,
			ppsubwin->axes.u_nzgrads);
		(ppsubwin->axes).u_xlabels = AllocAndSetUserLabelsFromMdl(ppsubwin->axes.u_xlabels,
			ppaxesmdl->axes.u_xlabels,
			ppsubwin->axes.u_nxgrads);

		(ppsubwin->axes).u_ylabels = AllocAndSetUserLabelsFromMdl(ppsubwin->axes.u_ylabels,
			ppaxesmdl->axes.u_ylabels,
			ppsubwin->axes.u_nygrads);

		(ppsubwin->axes).u_zlabels = AllocAndSetUserLabelsFromMdl(ppsubwin->axes.u_zlabels,
			ppaxesmdl->axes.u_zlabels,
			ppsubwin->axes.u_nzgrads);

		sciGetAutoTicks(paxesmdl, autoTicks);
		sciInitAutoTicks(pobj, autoTicks[0], autoTicks[1], autoTicks[2]);
		/* end 22.09.04 */

		ppsubwin->axes.flag[0]= ppaxesmdl->axes.flag[0];
		ppsubwin->axes.flag[1]= ppaxesmdl->axes.flag[1];
		ppsubwin->axes.flag[2]= ppaxesmdl->axes.flag[2];
		sciInitHiddenAxisColor(pobj, sciGetHiddenAxisColor(paxesmdl));
		ppsubwin->project[0]= ppaxesmdl->project[0];
		ppsubwin->project[1]= ppaxesmdl->project[1];
		ppsubwin->project[2]= ppaxesmdl->project[2];
		sciInitHiddenColor(pobj, sciGetHiddenColor(paxesmdl));
		ppsubwin->isoview= ppaxesmdl->isoview;
		ppsubwin->WRect[0]   = ppaxesmdl->WRect[0];
		ppsubwin->WRect[1]   = ppaxesmdl->WRect[1];
		ppsubwin->WRect[2]   = ppaxesmdl->WRect[2];
		ppsubwin->WRect[3]   = ppaxesmdl->WRect[3];

		ppsubwin->ARect[0]   = ppaxesmdl->ARect[0];
		ppsubwin->ARect[1]   = ppaxesmdl->ARect[1];
		ppsubwin->ARect[2]   = ppaxesmdl->ARect[2];
		ppsubwin->ARect[3]   = ppaxesmdl->ARect[3];

		ppsubwin->FRect[0]   = ppaxesmdl->FRect[0];
		ppsubwin->FRect[1]   = ppaxesmdl->FRect[1] ;
		ppsubwin->FRect[2]   = ppaxesmdl->FRect[2];
		ppsubwin->FRect[3]   = ppaxesmdl->FRect[3];
		ppsubwin->FRect[4]   = ppaxesmdl->FRect[4] ;
		ppsubwin->FRect[5]   = ppaxesmdl->FRect[5];

		ppsubwin->visible = ppaxesmdl->visible;

		ppsubwin->clip_region_set = 0 ;
		sciInitIsClipping( pobj, sciGetIsClipping(paxesmdl) ) ;
		sciSetClipping(   pobj, sciGetClipping(  paxesmdl) ) ;

		ppsubwin->cube_scaling = ppaxesmdl->cube_scaling;

		ppsubwin->SRect[0]  =  ppaxesmdl->SRect[0];
		ppsubwin->SRect[1]  =  ppaxesmdl->SRect[1];
		ppsubwin->SRect[2]  =  ppaxesmdl->SRect[2];
		ppsubwin->SRect[3]  =  ppaxesmdl->SRect[3];
		ppsubwin->SRect[4]  =  ppaxesmdl->SRect[4];
		ppsubwin->SRect[5]  =  ppaxesmdl->SRect[5];

		ppsubwin->tight_limits = ppaxesmdl->tight_limits;
		ppsubwin->FirstPlot = ppaxesmdl->FirstPlot;

		sciInitUseNurbs(pobj, sciGetUseNurbs(paxesmdl));

		/* clone user data from model */
		cloneUserData(paxesmdl, pobj);

		if (sciInitSelectedSubWin(pobj) < 0 )
		{
			endFigureDataWriting(pparentfigure);
			return (sciPointObj *)NULL ;
		}

		/* Construction des labels: x,y,z et Title */

		if ((ppsubwin->mon_title =  ConstructLabel (pobj, "",1)) == NULL){
			//sciDelThisToItsParent (pobj, sciGetParent (pobj)); /* pobj type = scisubwindow*/
			// sciDelHandle (pobj);
			FREE(pobj->pfeatures);
			FREE(pobj);
			endFigureDataWriting(pparentfigure);
			return (sciPointObj *) NULL;
		}

		sciSetStrings( ppsubwin->mon_title,
			sciGetText( pSUBWIN_FEATURE(paxesmdl)->mon_title) ) ;


		/*------------------------------------*/
		if ((ppsubwin->mon_x_label =  ConstructLabel (pobj, "",2)) == NULL){
			DestroyLabel(ppsubwin->mon_title);
			//sciDelThisToItsParent (pobj, sciGetParent (pobj));
			// sciDelHandle (pobj);
			FREE(pobj->pfeatures);
			FREE(pobj);
			endFigureDataWriting(pparentfigure);
			return (sciPointObj *) NULL;
		}

		sciSetStrings( ppsubwin->mon_x_label,
			sciGetText(pSUBWIN_FEATURE(paxesmdl)->mon_x_label) ) ;


		/*------------------------------------*/
		if ((ppsubwin->mon_y_label =  ConstructLabel (pobj, "",3)) == NULL){
			DestroyLabel(ppsubwin->mon_title);
			DestroyLabel(ppsubwin->mon_x_label);
			//sciDelThisToItsParent (pobj, sciGetParent (pobj));
			// sciDelHandle (pobj);
			FREE(pobj->pfeatures);
			FREE(pobj);
			endFigureDataWriting(pparentfigure);
			return (sciPointObj *) NULL;
		}
		sciSetStrings( ppsubwin->mon_y_label,
			sciGetText( pSUBWIN_FEATURE(paxesmdl)->mon_y_label) ) ;

		/*------------------------------------*/
		if ((ppsubwin->mon_z_label =  ConstructLabel (pobj, "",4)) == NULL){
			DestroyLabel(ppsubwin->mon_title);
			DestroyLabel(ppsubwin->mon_x_label);
			DestroyLabel(ppsubwin->mon_y_label);
			//sciDelThisToItsParent (pobj, sciGetParent (pobj));
			// sciDelHandle (pobj);
			FREE(pobj->pfeatures);
			FREE(pobj);
			endFigureDataWriting(pparentfigure);
			return (sciPointObj *) NULL;
		}
		sciSetStrings( ppsubwin->mon_z_label,
			sciGetText(pSUBWIN_FEATURE(paxesmdl)->mon_z_label)  ) ;

		/* labels auto_position modes */
		pLABEL_FEATURE(ppsubwin->mon_x_label)->auto_position =
			pLABEL_FEATURE(ppaxesmdl->mon_x_label)->auto_position;

		pLABEL_FEATURE(ppsubwin->mon_y_label)->auto_position =
			pLABEL_FEATURE(ppaxesmdl->mon_y_label)->auto_position;

		pLABEL_FEATURE(ppsubwin->mon_z_label)->auto_position =
			pLABEL_FEATURE(ppaxesmdl->mon_z_label)->auto_position;

		pLABEL_FEATURE(ppsubwin->mon_title)->auto_position =
			pLABEL_FEATURE(ppaxesmdl->mon_title)->auto_position;

		/* labels auto_rotation modes */
		pLABEL_FEATURE(ppsubwin->mon_x_label)->auto_rotation =
			pLABEL_FEATURE(ppaxesmdl->mon_x_label)->auto_rotation;

		pLABEL_FEATURE(ppsubwin->mon_y_label)->auto_rotation =
			pLABEL_FEATURE(ppaxesmdl->mon_y_label)->auto_rotation;

		pLABEL_FEATURE(ppsubwin->mon_z_label)->auto_rotation =
			pLABEL_FEATURE(ppaxesmdl->mon_z_label)->auto_rotation;

		pLABEL_FEATURE(ppsubwin->mon_title)->auto_rotation =
			pLABEL_FEATURE(ppaxesmdl->mon_title)->auto_rotation;

                /*
                 * Deactivated for now since the cloneGraphicContext function now
                 * takes two char* (source MVC Identifier, destination MVC Identifier) as arguments.
                 * To be implemented.
                 */
#if 0
                cloneGraphicContext( ppaxesmdl->mon_x_label, ppsubwin->mon_x_label );
                cloneGraphicContext( ppaxesmdl->mon_y_label, ppsubwin->mon_y_label );
                cloneGraphicContext( ppaxesmdl->mon_z_label, ppsubwin->mon_z_label );
                cloneGraphicContext( ppaxesmdl->mon_title  , ppsubwin->mon_title   );
#endif

		endFigureDataWriting(pparentfigure);

		return (sciPointObj *)pobj;

	}
	else
	{
		Scierror(999, _("The parent has to be a FIGURE\n"));
		return NULL;
	}
#endif
}



/**
 * Creates a new text object. However the object is not added in the handle list.
 * Its graphic and font contexts are initialized.
 * This function is to be used with objects including a text object.
 */
char * allocateText( char       * pparentsubwinUID,
                            char             ** text         ,
                            int                 nbRow        ,
                            int                 nbCol        ,
                            double              x            ,
                            double              y            ,
                            BOOL                autoSize     ,
                            double              userSize[2]  ,
                            BOOL                centerPos    ,
                            int               * foreground   ,
                            int               * background   ,
                            BOOL                isboxed      ,
                            BOOL                isline       ,
                            BOOL                isfilled     ,
                            sciTextAlignment    align         )
{
    char* pobjUID = NULL;
    int textDimensions[2];
    int visible = 0;
    int* piVisible = &visible;
    int clipRegionSet;
    int clipState = 0;
    int* piClipState = &clipState;

    double* clipRegion;
    double position[3];
    double setUserSize[2];

    pobjUID = (char*) createGraphicObject(__GO_TEXT__);


    /* Required to initialize the default contour properties */
    setGraphicObjectProperty(pobjUID, __GO_PARENT__, pparentsubwinUID, jni_string, 1);

    /* To be implemented */
#if 0
    ppText->callback = (char *)NULL;
    ppText->callbacklen = 0;
    ppText->callbackevent = 100;
    ppText->visible = sciGetVisibility( pparentsubwin );
#endif

    getGraphicObjectProperty(pparentsubwinUID, __GO_VISIBLE__, jni_bool, &piVisible);
    setGraphicObjectProperty(pobjUID, __GO_VISIBLE__, &visible, jni_bool, 1);

    /* Clipping: to be checked for consistency */
    clipRegionSet = 0;
    setGraphicObjectProperty(pobjUID, __GO_CLIP_BOX_SET__, &clipRegionSet, jni_bool, 1);

    getGraphicObjectProperty(pparentsubwinUID, __GO_CLIP_BOX__, jni_double_vector, &clipRegion);
    setGraphicObjectProperty(pobjUID, __GO_CLIP_BOX__, clipRegion, jni_double_vector, 4);

    getGraphicObjectProperty(pparentsubwinUID, __GO_CLIP_STATE__, jni_int, &piClipState);
    setGraphicObjectProperty(pobjUID, __GO_CLIP_STATE__, &clipState, jni_int, 1);

    /* Check if we should load LaTex / MathML Java libraries */
    loadTextRenderingAPI(text, nbRow, nbCol);

    /* Allocates the String array */
    textDimensions[0] = nbRow;
    textDimensions[1] = nbCol;

    setGraphicObjectProperty(pobjUID, __GO_TEXT_ARRAY_DIMENSIONS__, textDimensions, jni_int_vector, 2);

    setGraphicObjectProperty(pobjUID, __GO_TEXT_STRINGS__, text, jni_string_vector, nbRow*nbCol);

    position[0] = x;
    position[1] = y;
    position[2] = 0.0;

    setGraphicObjectProperty(pobjUID, __GO_POSITION__, position, jni_double_vector, 3);

    /*
     * Possibly useless.
     * To be deleted
     */
#if 0
    ppText->is3d = TRUE;
#endif

  /*
   * This was used by the previous renderer to return the Text
   * object's bounding box, but has no present equivalent within the MVC.
   * To be implemented when needed
   */
#if 0
  /* initialize to a not too weird value */
  for (i = 0; i < 4; i++)
  {
    ppText->corners[i][0] = ppText->x;
    ppText->corners[i][1] = ppText->y;
    ppText->corners[i][2] = ppText->z;
  }
#endif

    setGraphicObjectProperty(pobjUID, __GO_TEXT_BOX_MODE__, &centerPos, jni_int, 1);
    setGraphicObjectProperty(pobjUID, __GO_AUTO_DIMENSIONING__, &autoSize, jni_bool, 1);

    /* userSize must be specified if the size is given by the user */
    /* or the user specified a rectangle */
    if (!autoSize || centerPos)
    {
        setUserSize[0] = userSize[0];
        setUserSize[1] = userSize[1];
    }
    else
    {
        setUserSize[0] = 0.0;
        setUserSize[1] = 0.0;
    }

    setGraphicObjectProperty(pobjUID, __GO_TEXT_BOX__, setUserSize, jni_double_vector, 2);

    /* Required to get the correct MVC value from the sciTextAlignment enum */
    align = align - 1;

    /* Set alignment to left if its value is incorrect */
    if (align < 0 || align > 2)
    {
        align = 0;
    }

    setGraphicObjectProperty(pobjUID, __GO_ALIGNMENT__, &align, jni_int, 1);

    cloneGraphicContext(pparentsubwinUID, pobjUID);

    /* To be deleted */
#if 0
    if ( sciInitGraphicContext( pObj ) == -1)
    {
        deleteGraphicObject(pobjUID);
        FREE(pObj);
        return NULL;
    }
#endif

    cloneFontContext(pparentsubwinUID, pobjUID);

    /* To be deleted */
#if 0
    if ( sciInitFontContext( pObj ) == -1 )
    {
        deleteGraphicObject(pobjUID);
        FREE(pObj);
        return NULL;
    }
#endif

    setGraphicObjectProperty(pobjUID, __GO_BOX__, &isboxed, jni_bool, 1);
    setGraphicObjectProperty(pobjUID, __GO_LINE_MODE__, &isline, jni_bool, 1);
    setGraphicObjectProperty(pobjUID, __GO_FILL_MODE__, &isfilled, jni_bool, 1);

    if ( foreground != NULL )
    {
        setGraphicObjectProperty(pobjUID, __GO_LINE_COLOR__, foreground, jni_int, 1);
    }

    if ( background != NULL )
    {
        setGraphicObjectProperty(pobjUID, __GO_BACKGROUND__, foreground, jni_int, 1);
    }

    /* Parent reset to the null object */
    setGraphicObjectProperty(pobjUID, __GO_PARENT__, "", jni_string, 1);

    return pobjUID;
}

/**ConstructText
 * This function creates the parents window (manager) and the elementaries structures
 * @param  char *pparentsubwinUID : parent subwin UID
 * @param  char * text[] : intial text matrix string.
 * @param  int nbCol : the number column of the text
 * @param  int nbRow : the number of row of the text
 * @return  : object UID if ok , NULL if not
 */
char *
ConstructText (char * pparentsubwinUID, char ** text, int nbRow, int nbCol, double x,
	       double y, BOOL autoSize, double userSize[2], BOOL centerPos, int *foreground, int *background,
	       BOOL isboxed, BOOL isline, BOOL isfilled, sciTextAlignment align )
{
    char* parentType;
    char* pobjUID = NULL;

    getGraphicObjectProperty(pparentsubwinUID, __GO_TYPE__, jni_string, &parentType);

    if (strcmp(parentType, __GO_AXES__) != 0)
    {
        Scierror(999, _("The parent has to be a SUBWIN\n"));
        return (char*) NULL;
    }

    pobjUID = allocateText( pparentsubwinUID, text, nbRow, nbCol, x, y,
        autoSize, userSize, centerPos, foreground, background,
        isboxed, isline, isfilled, align );

    /* allocateText never returns NULL, to be deleted. */
#if 0
    if ( pobjUID == NULL )
    {
        return NULL;
    }
#endif

//    if (sciAddNewHandle (pobj) == -1)
//    {
//        deleteGraphicObject(pobj->UID);
//        FREE(pobj);
//        return NULL;
//    }

    setGraphicObjectRelationship(pparentsubwinUID, pobjUID);

    return pobjUID;
}


/**constructLegend
 * This function creates  Legend structure
 */
char *
ConstructLegend (char * pparentsubwinUID, char **text, long long tabofhandles[], int nblegends)
{
    char* pobjUID = NULL;

    int i;
    int iLegendPresent = 0;
    int* piLegendPresent = &iLegendPresent;
    int iVisible;
    int* piVisible = &iVisible;
    int* tmp;
    int textDimensions[2];
    int fillMode;
    int legendLocation;

    int clipRegionSet;
    int clipState;

    double* clipRegion;
    double position[2];

    char** lineIDS;
    char* parentType;

    /* Check beforehand whether a Legend object is already present */
    getGraphicObjectProperty(pparentsubwinUID, __GO_HAS_LEGEND_CHILD__, jni_bool, &piLegendPresent);

    if (iLegendPresent)
    {
        /*
	 * Object deletion not implemented yet.
         * To be implemented
         */
#if 0
        DestroyLegend();
#endif
    }

    getGraphicObjectProperty(pparentsubwinUID, __GO_TYPE__, jni_string, &parentType);

    if (strcmp(parentType, __GO_AXES__) != 0)
    {
        Scierror(999, _("The parent has to be a SUBWIN\n"));
        return (char*) NULL;
    }

    pobjUID = (char*) createGraphicObject(__GO_LEGEND__);

    /* Required to initialize the default contour and font properties */
    setGraphicObjectProperty(pobjUID, __GO_PARENT__, pparentsubwinUID, jni_string, 1);

    /* To be implemented */
#if 0
    ppLegend->text.callback = (char *)NULL;
    ppLegend->text.callbacklen = 0;
    ppLegend->text.callbackevent = 100;
#endif

    /* To be implemented, probably useless */
#if 0
    ppLegend->text.isboxed = FALSE;
#endif


    getGraphicObjectProperty(pparentsubwinUID, __GO_VISIBLE__, jni_bool, &piVisible);

    setGraphicObjectProperty(pobjUID, __GO_VISIBLE__, &iVisible, jni_bool, 1);

    lineIDS = (char**) MALLOC(nblegends*sizeof(char*));

    if (lineIDS == NULL)
    {
        Scierror(999, _("%s: No more memory.\n"),"ConstructLegend");
        return (char*) NULL;
    }

    textDimensions[0] = nblegends;
    textDimensions[1] = 1;

    setGraphicObjectProperty(pobjUID, __GO_TEXT_ARRAY_DIMENSIONS__, textDimensions, jni_int_vector, 2);
    setGraphicObjectProperty(pobjUID, __GO_TEXT_STRINGS__, text, jni_string_vector, nblegends);

    for (i = 0; i < nblegends; i++)
    {
        char* tmpObjUID;

        tmpObjUID =  getObjectFromHandle(tabofhandles[i]);

        /*
         * Links are ordered from most recent to least recent,
         * as their referred-to Polylines in the latter's parent Compound object.
         */
        lineIDS[nblegends-i-1] = tmpObjUID;
    }

    setGraphicObjectProperty(pobjUID, __GO_LINKS__, lineIDS, jni_string_vector, nblegends);

    FREE(lineIDS);


    position[0] = 0.0;
    position[1] = 0.0;
    setGraphicObjectProperty(pobjUID, __GO_POSITION__, position, jni_double_vector, 2);


    /* 9: LOWER_CAPTION */
    legendLocation = 9;
    setGraphicObjectProperty(pobjUID, __GO_LEGEND_LOCATION__, &legendLocation, jni_int, 1);

    /* To be implemented */
#if 0
    ppLegend->isselected = TRUE;
#endif

    /* Clipping: to be checked for consistency */
    clipRegionSet = 0;
    setGraphicObjectProperty(pobjUID, __GO_CLIP_BOX_SET__, &clipRegionSet, jni_bool, 1);

    /* 0: OFF */
    clipState = 0;
    setGraphicObjectProperty(pobjUID, __GO_CLIP_STATE__, &clipState, jni_int, 1);

    getGraphicObjectProperty(pparentsubwinUID, __GO_CLIP_BOX__, jni_double_vector, &clipRegion);
    setGraphicObjectProperty(pobjUID, __GO_CLIP_BOX__, clipRegion, jni_double_vector, 4);


    /* NEW :  used to draw the line and marks of the curve F.Leray 21.01.05 */
    cloneGraphicContext(pparentsubwinUID, pobjUID);

    /* To be deleted */
#if 0
    if (sciInitGraphicContext (pobj) == -1)
    {
        deleteGraphicObject(pobj->UID);
        FREE(pobj);
        return (sciPointObj*) NULL;
    }
#endif

    cloneFontContext(pparentsubwinUID, pobjUID);

    /* To be deleted */
#if 0
    if (sciInitFontContext (pobj) == -1)
    {
        deleteGraphicObject(pobj->UID);
        FREE(pobj);
        return (sciPointObj*) NULL;
    }
#endif

    fillMode = TRUE;
    setGraphicObjectProperty(pobjUID, __GO_FILL_MODE__, &fillMode, jni_bool, 1);

    setGraphicObjectProperty(pobjUID, __GO_PARENT__, "", jni_string, 1);

//    if (sciAddNewHandle(pobj) == -1)
//    {
//        deleteGraphicObject(pobj->UID);
//        FREE(pobj);
//        return NULL;
//    }

    setGraphicObjectRelationship(pparentsubwinUID, pobjUID);

    return pobjUID;
}
/*---------------------------------------------------------------------------------*/
/**
 * Create a polyline but does not add it to Scilab hierarchy
 */
char* allocatePolyline(char* pparentsubwinUID, double *pvecx, double *pvecy, double *pvecz,
                       int closed, int n1,int plot, int *foreground, int *background,
                       int *mark_style, int *mark_foreground, int *mark_background,
                       BOOL isline, BOOL isfilled, BOOL ismark, BOOL isinterpshaded)
{
    char* pobjUID = NULL;
    int i = 0;
    BOOL result;
    char* type;
    char* polylineID;
    double barWidth;
    double arrowSizeFactor;
    double* clipRegion;
    double* dataVector;
    int clipState = 0;
    int *piClipState = &clipState;
    int lineClosed;
    int numElementsArray[2];
    int polylineStyle;
    int visible = 0;
    int* piVisible = &visible;
    int zCoordinatesSet;
    int* tmp;
    int clipRegionSet = 0;
    int *piClipRegionSet = &clipRegionSet;


    getGraphicObjectProperty(pparentsubwinUID, __GO_TYPE__, jni_string, &type);

    if (strcmp(type, __GO_AXES__) != 0)
    {
        Scierror(999, _("The parent has to be a SUBWIN\n"));
        return NULL;
    }

    pobjUID = (char*) createGraphicObject(__GO_POLYLINE__);

    polylineID = (char*) createDataObject(pobjUID, __GO_POLYLINE__);

    if (polylineID == NULL)
    {
        deleteGraphicObject(pobjUID);
        return NULL;
    }

    /* To be deleted */
#if 0
    /* Create the default relationShip */
    createDefaultRelationShip(pobj);
#endif

    /* To be deleted */
#if 0
    sciSetParent( pobj, pparentsubwin );
#endif

    /*
     * Sets the polyline's parent in order to initialize the former's Contoured properties
     * with the latter's values (sciInitGraphicContext call below)
     */
    setGraphicObjectProperty(pobjUID, __GO_PARENT__, pparentsubwinUID, jni_string, 1);


    barWidth = 0.0;
    setGraphicObjectProperty(pobjUID, __GO_BAR_WIDTH__, &barWidth, jni_double, 1);

    /* To be implemented */
#if 0
    pPOLYLINE_FEATURE (pobj)->callback = (char *)NULL;
    pPOLYLINE_FEATURE (pobj)->callbacklen = 0;
    pPOLYLINE_FEATURE (pobj)->callbackevent = 100;
#endif

    getGraphicObjectProperty(pparentsubwinUID, __GO_VISIBLE__, jni_bool, &piVisible);

    setGraphicObjectProperty(pobjUID, __GO_VISIBLE__, &visible, jni_bool, 1);


    /* To be deleted, see the MVC corresponding calls below */
#if 0
    pPOLYLINE_FEATURE (pobj)->clip_region_set = 0;
    sciInitIsClipping( pobj, sciGetIsClipping((sciPointObj *) sciGetParentSubwin(pobj)) ) ;
    sciSetClipping(pobj,sciGetClipping(sciGetParentSubwin(pobj)));
#endif

    /* Clip state and region */
    /* To be checked for consistency */

    getGraphicObjectProperty(pparentsubwinUID, __GO_CLIP_BOX__, jni_double_vector, &clipRegion);
    setGraphicObjectProperty(pobjUID, __GO_CLIP_BOX__, clipRegion, jni_double_vector, 4);

    getGraphicObjectProperty(pparentsubwinUID, __GO_CLIP_BOX_SET__, jni_bool, &piClipRegionSet);
    setGraphicObjectProperty(pobjUID, __GO_CLIP_BOX_SET__, &clipRegionSet, jni_bool, 1);

    getGraphicObjectProperty(pparentsubwinUID, __GO_CLIP_STATE__, jni_int, &piClipState);
    setGraphicObjectProperty(pobjUID, __GO_CLIP_STATE__, &clipState, jni_int, 1);


    arrowSizeFactor = 1.0;
    setGraphicObjectProperty(pobjUID, __GO_ARROW_SIZE_FACTOR__, &arrowSizeFactor, jni_double, 1);

    /* To be implemented */
#if 0
    pPOLYLINE_FEATURE (pobj)->isselected = TRUE;
#endif


    /*
     * First element: number of gons (always 1 for a Polyline)
     * Second one: number of vertices composing the Polyline
     */
    numElementsArray[0] = 1;
    numElementsArray[1] = n1;

    /* Data */
    if ( n1 != 0 )
    {
        /*
         * Sets the number of elements (vertices composing the polyline) and allocates the coordinates array
         * The FALSE value is used to identify a failed memory allocation for now.
         */
        result = setGraphicObjectProperty(pobjUID, __GO_DATA_MODEL_NUM_ELEMENTS_ARRAY__, numElementsArray, jni_int_vector, 2);

        if (result == FALSE)
        {
            deleteGraphicObject(pobjUID);
            deleteDataObject(pobjUID);
            return NULL;
        }

        dataVector = MALLOC(3*n1*sizeof(double));

        if (dataVector == NULL)
        {
            deleteGraphicObject(pobjUID);
            deleteDataObject(pobjUID);
            return NULL;
        }

        if ((pvecx != (double *)NULL)&&(pvecy != (double *)NULL))
        {
            for (i = 0; i < n1; i++)
            {
                dataVector[i] = pvecx[i];
                dataVector[n1+i] = pvecy[i];
            }
        }
        else
        {
            for (i = 0; i < n1; i++)
            {
                dataVector[i] = 0.0;
                dataVector[n1+i] = 0.0;
            }
        }

        /**DJ.Abdemouche 2003**/
        if (pvecz == NULL)
        {
            for (i = 0; i < n1; i++)
            {
                dataVector[2*n1+i] = 0.0;
            }

            zCoordinatesSet = 0;
        }
        else
        {
            for (i = 0; i < n1; i++)
            {
                dataVector[2*n1+i] = pvecz[i];
            }

            zCoordinatesSet = 1;
        }

        /*
         * We could probably do without the dataVector copy by individually setting
         * x, y or z coordinates, and initializing coordinates to 0 during allocation
         * to ensure consistency
         */
        setGraphicObjectProperty(pobjUID, __GO_DATA_MODEL_COORDINATES__, dataVector, jni_double, n1);

        FREE(dataVector);

        setGraphicObjectProperty(pobjUID, __GO_DATA_MODEL_Z_COORDINATES_SET__, &zCoordinatesSet, jni_double, n1);
    }
    else
    {
        /* 0 points */
        result = setGraphicObjectProperty(pobjUID, __GO_DATA_MODEL_NUM_ELEMENTS_ARRAY__, numElementsArray, jni_int_vector, 2);

        if (result == FALSE)
        {
            deleteGraphicObject(pobjUID);
            deleteDataObject(pobjUID);
            return NULL;
        }
    }

    if (closed > 0)
    {
        lineClosed = 1;
    }
    else
    {
        lineClosed = 0;
    }

    setGraphicObjectProperty(pobjUID, __GO_CLOSED__, &lineClosed, jni_bool, 1);
    setGraphicObjectProperty(pobjUID, __GO_POLYLINE_STYLE__, &plot, jni_int, 1);

    /*
     * Initializes the contour properties (background, foreground, etc)
     * to the default values (those of the parent Axes).
     */
    cloneGraphicContext(pparentsubwinUID, pobjUID);

    // FIXME ???
    //sciInitGraphicContext(pobj);

    /* To be deleted */
#if 0
    if (sciInitGraphicContext (pobj) == -1)
    {
        FREE(pPOLYLINE_FEATURE (pobj)->pvy);
        FREE(pPOLYLINE_FEATURE (pobj)->pvx);
        FREE(pPOLYLINE_FEATURE(pobj));
        FREE(pobj);
        return NULL;
    }
#endif

  /* colors and marks setting */
    setGraphicObjectProperty(pobjUID, __GO_MARK_MODE__, &ismark, jni_bool, 1);
    setGraphicObjectProperty(pobjUID, __GO_LINE_MODE__, &isline, jni_bool, 1);
    setGraphicObjectProperty(pobjUID, __GO_FILL_MODE__, &isfilled, jni_bool, 1);

    /* shading interpolation vector and mode */
    setGraphicObjectProperty(pobjUID, __GO_INTERP_COLOR_MODE__, &isinterpshaded, jni_bool, 1);

    if(foreground != NULL)
    {
        setGraphicObjectProperty(pobjUID, __GO_LINE_COLOR__, foreground, jni_int, 1);

        /* To be fully implemented within the MVC framework since it performs color range checks */
#if 0
        sciInitForeground(pobj,(*foreground));
#endif
    }

    if(background != NULL){
        if(isinterpshaded == TRUE)
        { /* 3 or 4 values to store */

            setGraphicObjectProperty(pobjUID, __GO_INTERP_COLOR_VECTOR__, background, jni_int_vector, n1);

            /* To be deleted */
#if 0
            sciSetInterpVector(pobj,n1,background);
#endif
        }
        else
        {
            setGraphicObjectProperty(pobjUID, __GO_BACKGROUND__, background, jni_int, 1);

            /* To be fully implemented within the MVC framework since it performs color range checks */
#if 0
            sciInitBackground(pobj,(*background));
#endif
        }
    }

    if(mark_style != NULL)
    {
        /* This does use the MVC */
#if 0
        sciInitMarkStyle(pobj,(*mark_style));
#endif
    }

    if(mark_foreground != NULL)
    {
        setGraphicObjectProperty(pobjUID, __GO_MARK_FOREGROUND__, mark_foreground, jni_int, 1);

        /* To be fully implemented within the MVC framework since it performs color range checks */
#if 0
        sciInitMarkForeground(pobj,(*mark_foreground));
#endif
    }

    if(mark_background != NULL)
    {
        setGraphicObjectProperty(pobjUID, __GO_MARK_BACKGROUND__, mark_background, jni_int, 1);

        /* To be fully implemented within the MVC framework since it performs color range checks */
#if 0
        sciInitMarkBackground(pobj,(*mark_background));
#endif
    }

    /* no sons for now */
    /* To be deleted */
#if 0
    sciInitSelectedSons( pobj ) ;

    sciGetRelationship(pobj)->psons        = NULL ;
    sciGetRelationship(pobj)->plastsons    = NULL ;
    sciGetRelationship(pobj)->pSelectedSon = NULL ;
#endif

    /* Parent reset to the null object */
    setGraphicObjectProperty(pobjUID, __GO_PARENT__, "", jni_string, 1);

    visible = 1;
    setGraphicObjectProperty(pobjUID, __GO_VISIBLE__, &visible, jni_bool, 1);

    /*
     * Deactivated for now since not fully implemented yet by the MVC
     * To be implemented
     */
#if 0
    initUserData(pobj);
#endif

    return pobjUID;
}
/*---------------------------------------------------------------------------------*/
/**ConstructPolyline
 * This function creates  Polyline 2d structure
 */
char *ConstructPolyline (char *pparentsubwinUID, double *pvecx, double *pvecy, double *pvecz,
		   int closed, int n1,int plot, int *foreground, int *background,
		   int *mark_style, int *mark_foreground, int *mark_background,
                         BOOL isline, BOOL isfilled, BOOL ismark, BOOL isinterpshaded)
{
    char* pobjUID = allocatePolyline(pparentsubwinUID, pvecx, pvecy, pvecz, closed, n1, plot,
                                     foreground, background, mark_style, mark_foreground, mark_background,
                                     isline, isfilled, ismark, isinterpshaded);

    if (pobjUID == NULL)
    {
        return NULL;
    }

    /* allocatePolyline created a "fake" relationship, destroy it */
    /*
     * Deactivated since the sciPolyline struct is not used anymore
     * and sciStandardBuildOperations uses the obsolete C hierarchical
     * relationships.
     * The operations still relevant are performed below
     * (sciStandardBuildOperations should be updated as to include them).
     */
#if 0
    FREE(pobj->relationShip);

    if (sciStandardBuildOperations(pobj, pparentsubwin) == NULL)
    {
        FREE(pobj->pfeatures);
        FREE(pobj);
        return NULL;
    }
#endif


//  if (sciAddNewHandle(pobj) == -1)
//  {
//    deleteGraphicObject(pobj->UID);
//    deleteDataObject(pobj->UID);
//    FREE(pobj);
//    return NULL;
//  }

    /*
     * Sets the Axes as the polyline's parent and adds the polyline to
     * its parent's list of children.
     */
    setGraphicObjectRelationship(pparentsubwinUID, pobjUID);

    return pobjUID;
}



/**ConstructArc
 * This function creates an Arc structure
 */
char *
ConstructArc (char * pparentsubwinUID, double x, double y,
	      double height, double width, double alphabegin, double alphaend,
	      int *foreground, int *background, BOOL isfilled, BOOL isline)
{
    char* pobjUID = NULL;
    char* type;
    double upperLeftPoint[3];
    double* clipRegion;
    int* tmp;
    int visible = 0;
    int *piVisible = &visible;
    int arcDrawingMethod = 0;
    int *piArcDrawingMethod = &arcDrawingMethod;
    int clipRegionSet = 0;
    int *piClipRegionSet = &clipRegionSet;
    int clipState = 0;
    int *piClipState = &clipState;

    getGraphicObjectProperty(pparentsubwinUID, __GO_TYPE__, jni_string, &type);

    if (strcmp(type, __GO_AXES__) != 0)
    {
        Scierror(999, _("The parent has to be a SUBWIN\n"));
        return (char *) NULL;
    }

    pobjUID = (char*) createGraphicObject(__GO_ARC__);

    /*
     * Sets the arc's parent in order to initialize the former's Contoured properties
     * with the latter's values (cloneGraphicContext call below)
     */
    setGraphicObjectProperty(pobjUID, __GO_PARENT__, pparentsubwinUID, jni_string, 1);

    /* To be implemented */
#if 0
    ppArc->callback = (char *)NULL;
    ppArc->callbacklen = 0;
    ppArc->callbackevent = 100;
#endif

    upperLeftPoint[0] = x;
    upperLeftPoint[1] = y;
    upperLeftPoint[2] = 0.0;

    setGraphicObjectProperty(pobjUID, __GO_UPPER_LEFT_POINT__, upperLeftPoint, jni_double_vector, 3);

    setGraphicObjectProperty(pobjUID, __GO_HEIGHT__, &height, jni_double, 1);
    setGraphicObjectProperty(pobjUID, __GO_WIDTH__, &width, jni_double, 1);

    setGraphicObjectProperty(pobjUID, __GO_START_ANGLE__, &alphabegin, jni_double, 1);
    setGraphicObjectProperty(pobjUID, __GO_END_ANGLE__, &alphaend, jni_double, 1);

    /* To be implemented */
#if 0
    ppArc->isselected = TRUE;
#endif

    getGraphicObjectProperty(pparentsubwinUID, __GO_VISIBLE__, jni_bool, &piVisible);

    setGraphicObjectProperty(pobjUID, __GO_VISIBLE__, &visible, jni_bool, 1);

    getGraphicObjectProperty(pparentsubwinUID, __GO_ARC_DRAWING_METHOD__, jni_int, &piArcDrawingMethod);

    setGraphicObjectProperty(pobjUID, __GO_ARC_DRAWING_METHOD__, &arcDrawingMethod, jni_int, 1);

    /* To be checked for consistency */
#if 0
      ppArc->clip_region_set = 0;
      /*ppArc->isclip = sciGetIsClipping((sciPointObj *) sciGetParentSubwin(pobj)); */
      sciInitIsClipping( pobj, sciGetIsClipping(pparentsubwin) ) ;
      sciSetClipping(pobj,sciGetClipping(pparentsubwin));
      /*      pARC_FEATURE (pobj)->clip_region = (double *) NULL; */
#endif

    /*
     * Clip state and region
     * To be checked for consistency
     */
    getGraphicObjectProperty(pparentsubwinUID, __GO_CLIP_BOX__, jni_double_vector, &clipRegion);
    setGraphicObjectProperty(pobjUID, __GO_CLIP_BOX__, clipRegion, jni_double_vector, 4);

    getGraphicObjectProperty(pparentsubwinUID, __GO_CLIP_BOX_SET__, jni_bool, &piClipRegionSet);
    setGraphicObjectProperty(pobjUID, __GO_CLIP_BOX_SET__, &clipRegionSet, jni_bool, 1);

    getGraphicObjectProperty(pparentsubwinUID, __GO_CLIP_STATE__, jni_int, &piClipState);
    setGraphicObjectProperty(pobjUID, __GO_CLIP_STATE__, &clipState, jni_int, 1);

    /*
     * Initializes the contour properties (background, foreground, etc)
     * to the parent Axes' values.
     */
    cloneGraphicContext(pparentsubwinUID, pobjUID);

    /* Contour settings */
    setGraphicObjectProperty(pobjUID, __GO_LINE_MODE__, &isline, jni_bool, 1);
    setGraphicObjectProperty(pobjUID, __GO_FILL_MODE__, &isfilled, jni_bool, 1);

    if(foreground != NULL)
    {
        setGraphicObjectProperty(pobjUID, __GO_LINE_COLOR__, foreground, jni_int, 1);
    }

    if(background != NULL)
    {
        setGraphicObjectProperty(pobjUID, __GO_BACKGROUND__, background, jni_int, 1);
    }

    /* Parent reset to the null object */
    setGraphicObjectProperty(pobjUID, __GO_PARENT__, "", jni_string, 1);

//    if (sciAddNewHandle(pobj) == -1)
//    {
//        deleteGraphicObject(pobj->UID);
//        FREE(pobj);
//        return NULL;
//    }

    /*
     * Sets the Axes as the arc's parent and adds the arc to
     * its parent's list of children.
     */
    setGraphicObjectRelationship(pparentsubwinUID, pobjUID);

    return pobjUID;
}



/**ConstructRectangle
 * This function creates Rectangle structure and only this to destroy all sons use DelGraphicsSon
 */
char *
ConstructRectangle (char * pparentsubwinUID, double x, double y,
		    double height, double width,  int *foreground, int *background,
		    int isfilled, int isline)
{
    char* pobjUID = NULL;
    char* type;
    double upperLeftPoint[3];
    double* clipRegion;
    int visible = 0;
    int *piVisible = &visible;
    int* tmp;
    int clipRegionSet = 0;
    int *piClipRegionSet = &clipRegionSet;
    int clipState = 0;
    int *piClipState = &clipState;

    if ( height < 0.0 || width < 0.0 )
    {
        Scierror(999,_("Width and height must be positive.\n"));
        return NULL;
    }

    getGraphicObjectProperty(pparentsubwinUID, __GO_TYPE__, jni_string, &type);

    if (strcmp(type, __GO_AXES__) != 0)
    {
        Scierror(999, _("The parent has to be a SUBWIN\n"));
        return (char *) NULL;
    }

    pobjUID = (char*) createGraphicObject(__GO_RECTANGLE__);

    /*
     * Sets the rectangle's parent in order to initialize the former's Contoured properties
     * with the latter's values (cloneGraphicContext call below)
     */
    setGraphicObjectProperty(pobjUID, __GO_PARENT__, pparentsubwinUID, jni_string, 1);

      /* To be implemented */
#if 0
      pRECTANGLE_FEATURE (pobj)->callback = (char *)NULL;
      pRECTANGLE_FEATURE (pobj)->callbacklen = 0;
      pRECTANGLE_FEATURE (pobj)->callbackevent = 100;
#endif

    upperLeftPoint[0] = x;
    upperLeftPoint[1] = y;
    upperLeftPoint[2] = 0.0;

    setGraphicObjectProperty(pobjUID, __GO_UPPER_LEFT_POINT__, upperLeftPoint, jni_double_vector, 3);

    setGraphicObjectProperty(pobjUID, __GO_HEIGHT__, &height, jni_double, 1);
    setGraphicObjectProperty(pobjUID, __GO_WIDTH__, &width, jni_double, 1);

      /* To be implemented */
#if 0
    pRECTANGLE_FEATURE (pobj)->isselected = TRUE;
#endif

    getGraphicObjectProperty(pparentsubwinUID, __GO_VISIBLE__, jni_bool, &piVisible);
    setGraphicObjectProperty(pobjUID, __GO_VISIBLE__, &visible, jni_bool, 1);

    /* Clipping: to be checked */
#if 0
    pRECTANGLE_FEATURE (pobj)->clip_region_set = 0;
    sciInitIsClipping( pobj, sciGetIsClipping((sciPointObj *) sciGetParentSubwin(pobj)) ) ;
    sciSetClipping(pobj,sciGetClipping(sciGetParentSubwin(pobj)));
#endif

    /* Clip state and region */
    /* To be checked for consistency */

    getGraphicObjectProperty(pparentsubwinUID, __GO_CLIP_BOX__, jni_double_vector, &clipRegion);
    setGraphicObjectProperty(pobjUID, __GO_CLIP_BOX__, clipRegion, jni_double_vector, 4);

    getGraphicObjectProperty(pparentsubwinUID, __GO_CLIP_BOX_SET__, jni_bool, &piClipRegionSet);
    setGraphicObjectProperty(pobjUID, __GO_CLIP_BOX_SET__, &clipRegionSet, jni_bool, 1);

    getGraphicObjectProperty(pparentsubwinUID, __GO_CLIP_STATE__, jni_int, &piClipState);
    setGraphicObjectProperty(pobjUID, __GO_CLIP_STATE__, &clipState, jni_int, 1);

    /*
     * Initializes the contour properties (background, foreground, etc)
     * to the default values (those of the parent Axes).
     */
    cloneGraphicContext(pparentsubwinUID, pobjUID);

    /* To be deleted */
#if 0
    if (sciInitGraphicContext (pobj) == -1)
    {
        deleteGraphicObject(pobjUID);
        FREE(pobj);
        return (sciPointObj *) NULL;
    }
#endif

    /* Contour settings */
    setGraphicObjectProperty(pobjUID, __GO_LINE_MODE__, &isline, jni_bool, 1);
    setGraphicObjectProperty(pobjUID, __GO_FILL_MODE__, &isfilled, jni_bool, 1);

    if(foreground != NULL)
    {
        setGraphicObjectProperty(pobjUID, __GO_LINE_COLOR__, foreground, jni_int, 1);
    }

    if(background != NULL)
    {
        setGraphicObjectProperty(pobjUID, __GO_BACKGROUND__, background, jni_int, 1);
    }

    /* Parent reset to the null object */
    setGraphicObjectProperty(pobjUID, __GO_PARENT__, "", jni_string, 1);

//    if (sciAddNewHandle(pobj) == -1)
//    {
//        deleteGraphicObject(pobj->UID);
//        FREE(pobj);
//        return NULL;
//    }

    /*
     * Sets the Axes as the rectangle's parent and adds the rectangle to
     * its parent's list of children.
     */
    setGraphicObjectRelationship(pparentsubwinUID, pobjUID);

    return pobjUID;
}



/**ConstructSurface
 * This function creates Surface Structure
 */
char *
ConstructSurface (char * pparentsubwinUID, sciTypeOf3D typeof3d,
		  double * pvecx, double * pvecy, double * pvecz,double *zcol,
		  int izcol, int dimzx, int dimzy,
		  int *flag, double *ebox,int flagcolor,
		  int *isfac, int *m1, int *n1, int *m2,
		  int *n2, int *m3, int *n3, int *m3n, int *n3n)
{
    char *pobjUID = NULL;
    /*debug F.Leray*/
    /* To be deleted */
#if 0
    sciSurface *psurf;
#endif

    char* parentType;
    char* surfaceID;
    char* surfaceTypes[2] = {__GO_PLOT3D__, __GO_FAC3D__};

    double* clipRegion;

    int i=0, j=0;
    int nx,ny,nz,nc,izc=izcol;
    int result;
    int clipRegionSet = 0;
    int *piClipRegionSet = &clipRegionSet;
    int clipState = 0;
    int *piClipState = &clipState;
    int visible = 0;
    int *piVisible = &visible;
    int cdataMapping;
    int hiddenColor = 0;
    int *piHiddenColor = &hiddenColor;
    int surfaceMode;
    int* tmp;

    /* To be modified: the MVC does not allow Plot3d objects with color data yet */
    if (typeof3d == SCI_PLOT3D)
    {
        nx=dimzx;
        ny=dimzy;
        nz=dimzx*dimzy;
        if (flagcolor == 2)
        {
            /* one color per facet: nc = dimzx * dimzy */
            nc=nz;
        }
        else if (flagcolor == 3)
        {
            /*
             * one color per edge: nc = 4* dimzx * dimzy ??????
             * 3 or 4 vertices are needed: I think we take 4 to have enough allocated memory
             */
            nc=nz*4;
         }
        /* made by Djalel : comes from the genfac3d case */
        else
        {
            nc=0;
        }
    }
    /* DJ.A 2003 */
    else
    { /* case SCI_FAC3D */
        nx=dimzx*dimzy;
        ny=dimzx*dimzy;
        nz=dimzx*dimzy;
        if (flagcolor == 2)
        {
            /* one color per facet: nc = dimzy */
            nc=dimzy;
        }
        else if (flagcolor == 3)
        {
            /* one color per edge: nc = dimzx * dimzy */
            nc=nz;
        }
        else
        {
            nc=0;
        }
    }

    getGraphicObjectProperty(pparentsubwinUID, __GO_TYPE__, jni_string, &parentType);

    /* test using sciGetEntityType replaced by a test on the type string */
    if (strcmp(parentType, __GO_AXES__) != 0)
    {
        Scierror(999, _("The parent has to be a SUBWIN\n"));
        return NULL;
    }

    /* To be implemented */
#if 0
    psurf->callback = (char *)NULL;
    psurf->callbacklen = 0;
    psurf->callbackevent = 100;
#endif

    pobjUID = (char*) createGraphicObject(surfaceTypes[*isfac]);
    surfaceID = (char*) createDataObject(pobjUID, surfaceTypes[*isfac]);

    /*Adding F.Leray 19.03.04*/
    /* Dimension of the color matrix, to be implemented (vector case) */
#if 0
    psurf->m3n= *m3n;
    psurf->n3n= *n3n;
#endif

    /* Clip state and region */
    /* To be checked for consistency */

    getGraphicObjectProperty(pparentsubwinUID, __GO_CLIP_BOX__, jni_double_vector, &clipRegion);
    setGraphicObjectProperty(pobjUID, __GO_CLIP_BOX__, clipRegion, jni_double_vector, 4);

    getGraphicObjectProperty(pparentsubwinUID, __GO_CLIP_BOX_SET__, jni_bool, &piClipRegionSet);
    setGraphicObjectProperty(pobjUID, __GO_CLIP_BOX_SET__, &clipRegionSet, jni_bool, 1);

    getGraphicObjectProperty(pparentsubwinUID, __GO_CLIP_STATE__, jni_int, &piClipState);
    setGraphicObjectProperty(pobjUID, __GO_CLIP_STATE__, &clipState, jni_int, 1);

    /* Visibility */
    getGraphicObjectProperty(pparentsubwinUID, __GO_VISIBLE__, jni_bool, &piVisible);

    setGraphicObjectProperty(pobjUID, __GO_VISIBLE__, &visible, jni_bool, 1);

    setGraphicObjectProperty(pobjUID, __GO_COLOR_FLAG__, &flagcolor, jni_int, 1);

    /* Direct mode enabled as default */
    cdataMapping = 1;

    /* Only for Fac3D */
    setGraphicObjectProperty(pobjUID, __GO_DATA_MAPPING__, &cdataMapping, jni_int, 1);

    setGraphicObjectProperty(pobjUID, __GO_COLOR_MODE__, &flag[0], jni_int, 1);

    /* Plot3d case */
    if (!*isfac)
    {
        int gridSize[4];

        gridSize[0] = *m1;
        gridSize[1] = *n1;
        gridSize[2] = *m2;
        gridSize[3] = *n2;

        /* Allocates the coordinates arrays */
        result = setGraphicObjectProperty(pobjUID, __GO_DATA_MODEL_GRID_SIZE__, gridSize, jni_int_vector, 4);
    }
    /* Fac3d case */
    else
    {
        int numElementsArray[3];

        /*
         * First element: number of n-gons
         * Second element: number of vertices per n-gon
         * Third element: number of input colors
         */
        numElementsArray[0] = dimzy;
        numElementsArray[1] = dimzx;
        numElementsArray[2] = nc;

        /* Allocates the coordinates and color arrays */
        result = setGraphicObjectProperty(pobjUID, __GO_DATA_MODEL_NUM_ELEMENTS_ARRAY__, numElementsArray, jni_int_vector, 3);
    }

    if (result == 0)
    {
        deleteGraphicObject(pobjUID);
        deleteDataObject(pobjUID);
        return NULL;
    }

    setGraphicObjectProperty(pobjUID, __GO_DATA_MODEL_X__, pvecx, jni_double_vector, nx);
    setGraphicObjectProperty(pobjUID, __GO_DATA_MODEL_Y__, pvecy, jni_double_vector, ny);
    setGraphicObjectProperty(pobjUID, __GO_DATA_MODEL_Z__, pvecz, jni_double_vector, nz);

    /* Add the color matrix dimensions as a property ? */
    if (nc > 0)
    {
        setGraphicObjectProperty(pobjUID, __GO_DATA_MODEL_COLORS__, zcol, jni_double_vector, nc);
    }

    /*-------END Replaced by: --------*/

    /* To be implemented */
#if 0
    psurf->isselected = TRUE;
#endif

    /* The flag array is apparently now useless (flag[0] == COLOR_MODE, the two remaining ones are unused) */
#if 0
    psurf->flag[0] = flag[0]; /* F.Leray 16.04.04 HERE We store the flag=[mode (hidden part ), type (scaling), box (frame around the plot)] */
    psurf->flag[1] = flag[1];
    psurf->flag[2] = flag[2];
#endif

    /* DJ.A 2003 */
    /* Apparently useless */
#if 0
    psurf->ebox[0] = ebox[0];
    psurf->ebox[1] = ebox[1];
    psurf->ebox[2] = ebox[2];
    psurf->ebox[3] = ebox[3];
    psurf->ebox[4] = ebox[4];
    psurf->ebox[5] = ebox[5];
#endif

    getGraphicObjectProperty(pparentsubwinUID, __GO_HIDDEN_COLOR__, jni_int, &piHiddenColor);
    setGraphicObjectProperty(pobjUID, __GO_HIDDEN_COLOR__, &hiddenColor, jni_int, 1);

    /*
     * surfaceMode set to "on", was previously done by InitGraphicContext, by setting
     * the graphic context's line_mode to on, which previously stood for the surface_mode.
     */
    surfaceMode = 1;

    setGraphicObjectProperty(pobjUID, __GO_SURFACE_MODE__, &surfaceMode, jni_bool, 1);


    /*
     * Adding a new handle and setting the parent-child relationship is now
     * done after data initialization in order to avoid additional
     * clean-up.
     */
//    if (sciAddNewHandle(pobj) == -1)
//    {
//        deleteGraphicObject(pobj->UID);
//        deleteDataObject(pobj->UID);
//        FREE(pobj);
//        return (sciPointObj*) NULL;
//    }

    // Here we init old 'graphicContext' by cloning it from parent.
    cloneGraphicContext(pparentsubwinUID, pobjUID);
    setGraphicObjectRelationship(pparentsubwinUID, pobjUID);

//    if (sciInitGraphicContext (pobj) == -1)
//    {
//        setGraphicObjectRelationship("", pobjUID);
//        deleteGraphicObject(pobjUID);
//        deleteDataObject(pobjUID);
//        // sciDelHandle(pobj);
//
//        return NULL;
//    }

    return pobjUID;
}

/********************** 14/05/2002 *****
 **ConstructGrayplot
 * This function is used to build Grayplot and Matplot objects.
 * It would probably be better to put the code related to Matplot objects
 * in a separate build function, as it would avoid having to perform several tests
 * on the type parameter. This is done so because Matplot objects were previously
 * internally represented by sciGrayplot structures.
 */
sciPointObj *
ConstructGrayplot (sciPointObj * pparentsubwin, double *pvecx, double *pvecy,
		   double *pvecz, int n1, int n2, int type)
{
    sciPointObj *pobj = (sciPointObj *) NULL;

    char* objectTypes[3] = {__GO_GRAYPLOT__, __GO_MATPLOT__, __GO_MATPLOT__};

    char* typeParent;
    char* grayplotID;
    int result;
    int dataMapping;
    int gridSize[4];

    int parentVisible = 0;
    int *piParentVisible = &parentVisible;
    double* clipRegion;
    int clipRegionSet = 0;
    int *piClipRegionSet = &clipRegionSet;
    int clipState = 0;
    int *piClipState = &clipState;
    int* tmp;
    int numElements;

    getGraphicObjectProperty(pparentsubwin->UID, __GO_TYPE__, jni_string, &typeParent);

    if (strcmp(typeParent, __GO_AXES__) != 0)
    {
        Scierror(999, _("The parent has to be a SUBWIN\n"));
        return (sciPointObj *) NULL;
    }

    if ((pobj = MALLOC ((sizeof (sciPointObj)))) == NULL)
    {
        return (sciPointObj *) NULL;
    }

    pobj->UID = (char*) createGraphicObject(objectTypes[type]);
    grayplotID = (char*) createDataObject(pobj->UID, objectTypes[type]);

    if (grayplotID == NULL)
    {
        deleteGraphicObject(pobj->UID);
        FREE(pobj);
        return (sciPointObj*) NULL;
    }

    /* To be implemented */
#if 0
    pGRAYPLOT_FEATURE (pobj)->callback = (char *)NULL;
    pGRAYPLOT_FEATURE (pobj)->callbacklen = 0;
    pGRAYPLOT_FEATURE (pobj)->callbackevent = 100;

    pGRAYPLOT_FEATURE (pobj)->isselected = TRUE;
#endif

    /* 0: scaled; only used for Grayplot */
    if (type == 0)
    {
        dataMapping = 0;
        setGraphicObjectProperty(pobj->UID, __GO_DATA_MAPPING__, &dataMapping, jni_int, 1);
    }

    /* The x and y vectors are column ones */

    /*
     * For the Grayplot object, the number of rows and columns respectively
     * correspond to the grid's x and y dimensions whereas for Matplot objects,
     * they respectively correspond to the grid's y and x dimensions.
     */
    if (type == 0)
    {
        gridSize[0] = n1;
        gridSize[1] = 1;
        gridSize[2] = n2;
        gridSize[3] = 1;
    }
    else
    {
        gridSize[0] = n2;
        gridSize[1] = 1;
        gridSize[2] = n1;
        gridSize[3] = 1;
    }

    /* Only for Matplot objects */
    if (type != 0)
    {
        if (type == 1)
        {
            setGraphicObjectProperty(pobj->UID, __GO_DATA_MODEL_MATPLOT_BOUNDS__, NULL, jni_double_vector, 4);
        }
        else if (type == 2)
        {
            setGraphicObjectProperty(pobj->UID, __GO_DATA_MODEL_MATPLOT_BOUNDS__, pvecx, jni_double_vector, 4);
        }
    }

    /* Allocates the coordinates arrays */
    result = setGraphicObjectProperty(pobj->UID, __GO_DATA_MODEL_GRID_SIZE__, gridSize, jni_int_vector, 4);

    if (result == 0)
    {
        deleteGraphicObject(pobj->UID);
        deleteDataObject(pobj->UID);
        FREE(pobj);
        return (sciPointObj*) NULL;
    }

    /* Only for Grayplot objects, for Matplot objects, x and y coordinates are automatically computed */
    if (type == 0)
    {
        setGraphicObjectProperty(pobj->UID, __GO_DATA_MODEL_X__, pvecx, jni_double_vector, n1);
        setGraphicObjectProperty(pobj->UID, __GO_DATA_MODEL_Y__, pvecy, jni_double_vector, n2);
    }

    if (type == 0)
    {
        numElements = n1*n2;
    }
    else
    {
        numElements = (n1-1)*(n2-1);
    }

    setGraphicObjectProperty(pobj->UID, __GO_DATA_MODEL_Z__, pvecz, jni_double_vector, numElements);

    /*
     * Adding a new handle and setting the parent-child relationship is now
     * done after data initialization in order to avoid additional
     * clean-up.
     */
//    if (sciAddNewHandle(pobj) == -1)
//    {
//        deleteGraphicObject(pobj->UID);
//        FREE(pobj);
//        return (sciPointObj*) NULL;
//    }

    setGraphicObjectRelationship(pparentsubwin->UID, pobj->UID);


    getGraphicObjectProperty(pparentsubwin->UID, __GO_VISIBLE__, jni_bool, &piParentVisible);
    setGraphicObjectProperty(pobj->UID, __GO_VISIBLE__, &parentVisible, jni_bool, 1);

   /*
    * Clip state and region
    * To be checked for consistency
    */
    getGraphicObjectProperty(pparentsubwin->UID, __GO_CLIP_BOX__, jni_double_vector, &clipRegion);
    setGraphicObjectProperty(pobj->UID, __GO_CLIP_BOX__, clipRegion, jni_double_vector, 4);

    getGraphicObjectProperty(pparentsubwin->UID, __GO_CLIP_BOX_SET__, jni_bool, &piClipRegionSet);
    setGraphicObjectProperty(pobj->UID, __GO_CLIP_BOX_SET__, &clipRegionSet, jni_bool, 1);

    getGraphicObjectProperty(pparentsubwin->UID, __GO_CLIP_STATE__, jni_int, &piClipState);
    setGraphicObjectProperty(pobj->UID, __GO_CLIP_STATE__, &clipState, jni_int, 1);

    if (sciInitGraphicContext (pobj) == -1)
    {
        setGraphicObjectRelationship("", pobj->UID);
        deleteGraphicObject(pobj->UID);
        deleteDataObject(pobj->UID);
        // sciDelHandle(pobj);

        FREE(pobj);
        return (sciPointObj *) NULL;
    }

    return pobj;
}


/**ConstructAxes
 * This function creates an Axis object
 * To do: rename to ConstructAxis
 * @author Djalel ABDEMOUCHE
 * @see sciSetCurrentObj
 *
 */
sciPointObj *
ConstructAxes (sciPointObj * pparentsubwin, char dir, char tics, double *vx,
	       int nx, double *vy, int ny,char **str, int subint, char *format,
	       int fontsize, int textcolor, int ticscolor, char logscale, int seg, int nb_tics_labels)
{
    char* parentType;
    sciPointObj *pobj = (sciPointObj *) NULL;
    int i;
    int clipRegionSet;
    int clipState;
    int ticksDirection;
    int ticksStyle;
    double* clipRegion;
    double doubleFontSize;

    getGraphicObjectProperty(pparentsubwin->UID, __GO_TYPE__, jni_string, &parentType);

    if (strcmp(parentType, __GO_AXES__) != 0)
    {
        Scierror(999, _("The parent has to be a SUBWIN\n"));
        return (sciPointObj*) NULL;
    }

    if ((pobj = MALLOC ((sizeof (sciPointObj)))) == NULL)
    {
        return (sciPointObj *) NULL;
    }

    pobj->UID = (char*) createGraphicObject(__GO_AXIS__);

    /* Required to initialize the default contour properties */
    setGraphicObjectProperty(pobj->UID, __GO_PARENT__, pparentsubwin->UID, jni_string, 1);

    /* To be implemented */
#if 0
    pAXES_FEATURE (pobj)->callback = (char *)NULL;
    pAXES_FEATURE (pobj)->callbacklen = 0;
    pAXES_FEATURE (pobj)->callbackevent = 100;
    pAXES_FEATURE (pobj)->visible = sciGetVisibility(sciGetParentSubwin(pobj));
#endif

    /* Clipping: to be checked for consistency */
    clipRegionSet = 0;
    setGraphicObjectProperty(pobj->UID, __GO_CLIP_BOX_SET__, &clipRegionSet, jni_bool, 1);

    getGraphicObjectProperty(pparentsubwin->UID, __GO_CLIP_BOX__, jni_double_vector, &clipRegion);
    setGraphicObjectProperty(pobj->UID, __GO_CLIP_BOX__, clipRegion, jni_double_vector, 4);

    /* 0: OFF */
    clipState = 0;
    setGraphicObjectProperty(pobj->UID, __GO_CLIP_STATE__, &clipState, jni_int, 1);

    /* The ticks style and direction MVC properties are Integers */
    if (dir == 'u')
    {
        ticksDirection = 0;
    }
    else if (dir == 'd')
    {
        ticksDirection = 1;
    }
    else if (dir == 'l')
    {
        ticksDirection = 2;
    }
    else if (dir == 'r')
    {
        ticksDirection = 3;
    }
    else
    {
        ticksDirection = 0;
    }

    if (tics == 'v')
    {
        ticksStyle = 0;
    }
    else if (tics == 'r')
    {
        ticksStyle = 1;
    }
    else if (tics == 'i')
    {
        ticksStyle = 2;
    }
    else
    {
        ticksStyle = 0;
    }

    setGraphicObjectProperty(pobj->UID, __GO_TICKS_DIRECTION__, &ticksDirection, jni_int, 1);
    setGraphicObjectProperty(pobj->UID, __GO_TICKS_STYLE__, &ticksStyle, jni_int, 1);

    setGraphicObjectProperty(pobj->UID, __GO_X_TICKS_COORDS__, vx, jni_double_vector, nx);
    setGraphicObjectProperty(pobj->UID, __GO_Y_TICKS_COORDS__, vy, jni_double_vector, ny);

    /*
     * Labels are computed automatically depending on the ticks coordinates.
     * The computation is performed by a C function which has been adapted
     * to the MVC (property get calls) and was previously done in the
     * tics labels property get C function.
     * It should be done (or called) directly in the Java part of the model
     * for the sake of efficiency.
     * To be modified
     */
    if (str == NULL)
    {
        char** matData;
        StringMatrix* tics_labels;

        tics_labels = computeDefaultTicsLabels(pobj);

        if (tics_labels == NULL)
        {
            deleteGraphicObject(pobj->UID);
            FREE(pobj);
            return (sciPointObj*) NULL;
        }

        matData = getStrMatData(tics_labels);

        /*
         * The labels vector size must be computed using the matrix's dimensions.
         * To be modified when the labels computation is moved to the Model.
         */
        setGraphicObjectProperty(pobj->UID, __GO_TICKS_LABELS__, matData, jni_string_vector, tics_labels->nbCol*tics_labels->nbRow);

        deleteMatrix(tics_labels);
    }
    else
    {
        /*
         * Labels are set using the str argument; the previous code tested whether each element of the
         * str array was null and set the corresponding Axis' element to NULL, though there was no
         * apparent reason to do so. This is still checked, but now aborts building the Axis.
         */

        if(nb_tics_labels == -1)
        {
            Scierror(999, _("Impossible case when building axis\n"));
            deleteGraphicObject(pobj->UID);
            FREE(pobj);
            return (sciPointObj*) NULL;
        }

        for (i = 0; i < nb_tics_labels; i++)
        {
            if (str[i] == NULL)
            {
                deleteGraphicObject(pobj->UID);
                FREE(pobj);
                return (sciPointObj*) NULL;
            }
        }

        setGraphicObjectProperty(pobj->UID, __GO_TICKS_LABELS__, str, jni_string_vector, nb_tics_labels);
    }

    setGraphicObjectProperty(pobj->UID, __GO_SUBTICKS__, &subint, jni_int, 1);
    setGraphicObjectProperty(pobj->UID, __GO_TICKS_SEGMENT__, &seg, jni_bool, 1);

    if (format != NULL)
    {
        setGraphicObjectProperty(pobj->UID, __GO_FORMATN__, format, jni_string, 1);
    }

    if (sciInitGraphicContext (pobj) == -1)
    {
        deleteGraphicObject(pobj->UID);
        FREE(pobj);
        return (sciPointObj*) NULL;
    }

    if ( sciInitFontContext( pobj ) == -1 )
    {
        deleteGraphicObject(pobj->UID);
        FREE(pobj);
        return (sciPointObj*) NULL;
    }

    /* Parent reset to the null object */
    setGraphicObjectProperty(pobj->UID, __GO_PARENT__, "", jni_string, 1);

//    if (sciAddNewHandle(pobj) == -1)
//    {
//        deleteGraphicObject(pobj->UID);
//        deleteDataObject(pobj->UID);
//        FREE(pobj);
//        return (sciPointObj*) NULL;
//    }

    setGraphicObjectRelationship(pparentsubwin->UID, pobj->UID);

    doubleFontSize = (double) fontsize;

    setGraphicObjectProperty(pobj->UID, __GO_FONT_SIZE__, &doubleFontSize, jni_double, 1);
    setGraphicObjectProperty(pobj->UID, __GO_FONT_COLOR__, &textcolor, jni_int, 1);
    setGraphicObjectProperty(pobj->UID, __GO_TICKS_COLOR__, &ticscolor, jni_int, 1);

    return pobj;
}


/********************** 21/05/2002 *****
 **ConstructFec
 * This function creates Fec
 * @author Djalel.ABDEMOUCHE
 * @see sciSetCurrentObj
 */
sciPointObj *
ConstructFec (sciPointObj * pparentsubwin, double *pvecx, double *pvecy, double *pnoeud,
	      double *pfun, int Nnode, int Ntr, double *zminmax, int *colminmax,
	      int *colout, BOOL with_mesh)
{
    sciPointObj *pobj = (sciPointObj *) NULL;
    char* fecId = NULL;
    int result;

    char* parentType;
    int parentVisible = 0;
    int *piParentVisible = &parentVisible;

    int lineMode;

    double* clipRegion;
    int clipRegionSet = 0;
    int *piClipRegionSet = &clipRegionSet;
    int clipState = 0;
    int *piClipState = &piClipState;
    int* tmp;

    getGraphicObjectProperty(pparentsubwin->UID, __GO_TYPE__, jni_string, &parentType);

    /* test using sciGetEntityType replaced by a test on the type string */
    if (strcmp(parentType, __GO_AXES__) != 0)
    {
        Scierror(999, _("The parent has to be a SUBWIN\n"));
        return (sciPointObj *) NULL;
    }

    if ((pobj = MALLOC ((sizeof (sciPointObj)))) == NULL)
    {
        return (sciPointObj *) NULL;
    }

    pobj->UID = createGraphicObject(__GO_FEC__);
    fecId = (char*) createDataObject(pobj->UID, __GO_FEC__);

    if (fecId == NULL)
    {
        deleteGraphicObject(pobj->UID);
        FREE(pobj);
        return (sciPointObj*) NULL;
    }

    /* To be implemented */
#if 0
    pFEC_FEATURE (pobj)->callback = (char *)NULL;
    pFEC_FEATURE (pobj)->callbacklen = 0;
    pFEC_FEATURE (pobj)->callbackevent = 100;

    pFEC_FEATURE (pobj)->isselected = TRUE;
#endif

    /* Allocates the coordinates array */
    result = setGraphicObjectProperty(pobj->UID, __GO_DATA_MODEL_NUM_VERTICES__, &Nnode, jni_int, 1);

    if (result == 0)
    {
        deleteGraphicObject(pobj->UID);
        deleteDataObject(pobj->UID);
        FREE(pobj);
        return (sciPointObj*) NULL;
    }

    /* Allocates the triangle indices and values array */
    result = setGraphicObjectProperty(pobj->UID, __GO_DATA_MODEL_NUM_INDICES__, &Ntr, jni_int, 1);

    if (result == 0)
    {
        deleteGraphicObject(pobj->UID);
        deleteDataObject(pobj->UID);
        FREE(pobj);
        return (sciPointObj*) NULL;
    }

    setGraphicObjectProperty(pobj->UID, __GO_DATA_MODEL_X__, pvecx, jni_double_vector, Nnode);
    setGraphicObjectProperty(pobj->UID, __GO_DATA_MODEL_Y__, pvecy, jni_double_vector, Nnode);

    /* Fec-specific property: triangle indices plus special values (triangle number and flag) */
    setGraphicObjectProperty(pobj->UID, __GO_DATA_MODEL_FEC_TRIANGLES__, pnoeud, jni_double_vector, Ntr);

    /* Function values */
    setGraphicObjectProperty(pobj->UID, __GO_DATA_MODEL_VALUES__, pfun, jni_double_vector, Nnode);

    setGraphicObjectProperty(pobj->UID, __GO_Z_BOUNDS__, zminmax, jni_double_vector, 2);
    setGraphicObjectProperty(pobj->UID, __GO_COLOR_RANGE__, colminmax, jni_int_vector, 2);
    setGraphicObjectProperty(pobj->UID, __GO_OUTSIDE_COLOR__, colout, jni_int_vector, 2);

    /*
     * Adding a new handle and setting the parent-child relationship is now
     * done after data initialization in order to avoid additional
     * clean-up.
     */
//    if (sciAddNewHandle(pobj) == -1)
//    {
//        deleteGraphicObject(pobj->UID);
//        deleteDataObject(pobj->UID);
//        FREE(pobj);
//        return (sciPointObj*) NULL;
//    }

    setGraphicObjectRelationship(pparentsubwin->UID, pobj->UID);

#if 0
    pFEC_FEATURE (pobj)->visible = sciGetVisibility(sciGetParentSubwin(pobj));
#endif

    getGraphicObjectProperty(pparentsubwin->UID, __GO_VISIBLE__, jni_bool, &piParentVisible);
    setGraphicObjectProperty(pobj->UID, __GO_VISIBLE__, &parentVisible, jni_bool, 1);

    /* Clipping: to be checked */
#if 0
    sciInitIsClipping( pobj, sciGetIsClipping(pparentsubwin) ) ;
    sciSetClipping(pobj, sciGetClipping(pparentsubwin)) ;
#endif

   /*
    * Clip state and region
    * To be checked for consistency
    */
    getGraphicObjectProperty(pparentsubwin->UID, __GO_CLIP_BOX__, jni_double_vector, &clipRegion);
    setGraphicObjectProperty(pobj->UID, __GO_CLIP_BOX__, clipRegion, jni_double_vector, 4);

    getGraphicObjectProperty(pparentsubwin->UID, __GO_CLIP_BOX_SET__, jni_bool, &piClipRegionSet);
    setGraphicObjectProperty(pobj->UID, __GO_CLIP_BOX_SET__, &clipRegionSet, jni_bool, 1);

    getGraphicObjectProperty(pparentsubwin->UID, __GO_CLIP_STATE__, jni_int, &piClipState);
    setGraphicObjectProperty(pobj->UID, __GO_CLIP_STATE__, &clipState, jni_int, 1);

    if (sciInitGraphicContext (pobj) == -1)
    {
        setGraphicObjectRelationship("", pobj->UID);
        deleteGraphicObject(pobj->UID);
        deleteDataObject(pobj->UID);
        // sciDelHandle(pobj);

        FREE(pobj);
        return (sciPointObj *) NULL;
    }

    /* line mode is set using with_mesh */
    setGraphicObjectProperty(pobj->UID, __GO_LINE_MODE__, &with_mesh, jni_bool, 1);

    return pobj;
}



/**ConstructSegs
 * This function creates Segments
 * It is used to create and initialize the data of both the Champ and Segs MVC objects.
 * @author Djalel.ABDEMOUCHE
 * @version 0.1
 * @see sciSetCurrentObj
 */
sciPointObj *
ConstructSegs ( sciPointObj * pparentsubwin, int type,
                double *vx, double *vy, double *vz,
                int Nbr1,int Nbr2, int Nbr3,
                double *vfx, double *vfy,
                int flag, int *style, double arsize,
                int colored, int typeofchamp)
{
    sciPointObj *pobj = (sciPointObj *) NULL;
    /* To be deleted */
#if 0
    sciSegs * ppSegs = (sciSegs *) NULL;
#endif
    int visible = 0;
    int* piVisible = &visible;
    int clipRegionSet = 0;
    int *piClipRegionSet = &clipRegionSet;
    int clipState = 0;
    int* piClipState = &clipState;
    int numberArrows;
    int dimensions[2];
    int i;
    int foreground;
    int* tmp;

    double* clipRegion;
    double* arrowCoords;

    if ((pobj = MALLOC ((sizeof (sciPointObj)))) == NULL)
    {
        return (sciPointObj *) NULL;
    }

    if (type == 0)
    {
        pobj->UID = createGraphicObject(__GO_SEGS__);
    }
    else if (type == 1)
    {
        pobj->UID = createGraphicObject(__GO_CHAMP__);
    }
    else
    {
        return (sciPointObj*) NULL;
    }

    /* To be implemented */
#if 0
    ppSegs->callback = (char *)NULL;
    ppSegs->callbacklen = 0;
    ppSegs->callbackevent = 100;

    ppSegs->isselected = TRUE;
#endif

    getGraphicObjectProperty(pparentsubwin->UID, __GO_VISIBLE__, jni_bool, &piVisible);

    setGraphicObjectProperty(pobj->UID, __GO_VISIBLE__, &visible, jni_bool, 1);

    /* this must be done prior to the call of sciSetClipping to know */
    /* if the clip_state has been set */

   /*
    * Clip state and region
    * To be checked for consistency
    */
    getGraphicObjectProperty(pparentsubwin->UID, __GO_CLIP_BOX__, jni_double_vector, &clipRegion);
    setGraphicObjectProperty(pobj->UID, __GO_CLIP_BOX__, clipRegion, jni_double_vector, 4);

    getGraphicObjectProperty(pparentsubwin->UID, __GO_CLIP_BOX_SET__, jni_bool, &piClipRegionSet);
    setGraphicObjectProperty(pobj->UID, __GO_CLIP_BOX_SET__, &clipRegionSet, jni_bool, 1);

    getGraphicObjectProperty(pparentsubwin->UID, __GO_CLIP_STATE__, jni_int, &piClipState);
    setGraphicObjectProperty(pobj->UID, __GO_CLIP_STATE__, &clipState, jni_int, 1);


    if (type == 1)
    {
        numberArrows = Nbr1*Nbr2;
    }
    else
    {
        /* Segs: Nbr1/2 arrows, Nbr1 is the number of endpoints */
        numberArrows = Nbr1/2;
    }

    /* Triggers the creation of the Arrow objects part of Champ or Segs */
    setGraphicObjectProperty(pobj->UID, __GO_NUMBER_ARROWS__, &numberArrows, jni_int, 1);

    /* Champ property only */
    if (type == 1)
    {
        dimensions[0] = Nbr1;
        dimensions[1] = Nbr2;

        setGraphicObjectProperty(pobj->UID, __GO_CHAMP_DIMENSIONS__, dimensions, jni_int_vector, 2);
    }

    arrowCoords = (double*) MALLOC(3*numberArrows*sizeof(double));

    if (arrowCoords == NULL)
    {
        deleteGraphicObject(pobj->UID);
        FREE(pobj);
        return (sciPointObj*) NULL;
    }

    /* Type 0 corresponds to a SEGS object */
    if (type == 0)
    {
        for (i = 0; i < numberArrows; i++)
        {
            arrowCoords[3*i] = vx[2*i];
            arrowCoords[3*i+1] = vy[2*i];

            if (vz != NULL)
            {
                arrowCoords[3*i+2] = vz[2*i];
            }
            else
            {
                arrowCoords[3*i+2] = 0.0;
            }
        }

        setGraphicObjectProperty(pobj->UID, __GO_BASE__, arrowCoords, jni_double_vector, 3*numberArrows);

        for (i = 0; i < numberArrows; i++)
        {
            arrowCoords[3*i] = vx[2*i+1];
            arrowCoords[3*i+1] = vy[2*i+1];

            if (vz != NULL)
            {
                arrowCoords[3*i+2] = vz[2*i+1];
            }
            else
            {
                arrowCoords[3*i+2] = 0.0;
            }
        }

        setGraphicObjectProperty(pobj->UID, __GO_DIRECTION__, arrowCoords, jni_double_vector, 3*numberArrows);

        if (flag == 1)
        {
            /* Style is an array of numberArrows elements */
            setGraphicObjectProperty(pobj->UID, __GO_SEGS_COLORS__, style, jni_int_vector, numberArrows);
        }
        else
        {
            /* Style is a scalar */
            setGraphicObjectProperty(pobj->UID, __GO_SEGS_COLORS__, style, jni_int_vector, 1);
        }

    }
    else
    {
        /*
         * Type 1 corresponds to a CHAMP object
         * so building comes from champg
         */
        setGraphicObjectProperty(pobj->UID, __GO_BASE_X__, vx, jni_double_vector, Nbr1);
        setGraphicObjectProperty(pobj->UID, __GO_BASE_Y__, vy, jni_double_vector, Nbr2);

        /*
         * Foreground color
         * It has no apparent effect in Scilab 5.3.X and is therefore
         * commented out for now.
         */
#if 0
        tmp = (int*) getGraphicObjectProperty(pparentsubwin->UID, __GO_LINE_COLOR__, jni_int);
        foreground = *tmp;
        setGraphicObjectProperty(pobj->UID, __GO_LINE_COLOR__, &foreground, jni_int, 1);
#endif

        setGraphicObjectProperty(pobj->UID, __GO_ARROW_SIZE__, &arsize, jni_double, 1);

        for (i = 0; i < numberArrows; i++)
        {
            arrowCoords[3*i] = vfx[i];
            arrowCoords[3*i+1] = vfy[i];
            arrowCoords[3*i+2] = 0.0;
        }

        setGraphicObjectProperty(pobj->UID, __GO_DIRECTION__, arrowCoords, jni_double_vector, 3*numberArrows);

        /* typeofchamp corresponds to COLORED (0: false, 1: true) */
        setGraphicObjectProperty(pobj->UID, __GO_COLORED__, &typeofchamp, jni_bool, 1);
    }

    /* Required to initialize the default contour properties */
    setGraphicObjectProperty(pobj->UID, __GO_PARENT__, pparentsubwin->UID, jni_string, 1);

    if (sciInitGraphicContext (pobj) == -1)
    {
        deleteGraphicObject(pobj->UID);
        FREE(arrowCoords);
        FREE(pobj);
        return (sciPointObj *) NULL;
    }

//    if ( sciAddNewHandle(pobj) == -1 )
//    {
//        deleteGraphicObject(pobj->UID);
//        FREE(arrowCoords);
//        FREE(pobj);
//        return (sciPointObj *) NULL;
//    }

    setGraphicObjectProperty(pobj->UID, __GO_PARENT__, "", jni_string, 1);

    setGraphicObjectRelationship(pparentsubwin->UID, pobj->UID);

    FREE(arrowCoords);

    return pobj;
}


/**sciConstructCompound
 * constructs a Compound of entities
 * do only a association with a parent and a handle reservation !
 * check for valid handle can be done using CheckForCompound
 */
char *
ConstructCompound (long *handelsvalue, int number) /* Conflicting types with definition */
{
  char* compoundUID;
  char* parentAxesUID;
  char* firstMovedObjectUID;

  int i;
  int parentVisible = 0;
  int *piParentVisible = &parentVisible;

  compoundUID = createGraphicObject(__GO_COMPOUND__);

  /* Adding the Compound's handle was previously done by sciStandardBuildOperations */
//  if (sciAddNewHandle(compound) == -1)
//  {
//    deleteGraphicObject(compound->UID);
//    FREE(compound);
//    return NULL;
//  }

  /* The Compound's parent Axes is considered to be the Compound's first child's own parent */
  firstMovedObjectUID = getObjectFromHandle( (long) handelsvalue[0]);
  getGraphicObjectProperty(firstMovedObjectUID, __GO_PARENT__, jni_string, &parentAxesUID);

  /* Set the parent-child relationship between the Compound and each aggregated object */
  for ( i = 0 ; i < number ; i++ )
  {
    char* movedObjectUID = getObjectFromHandle( (long) handelsvalue[i]);

    setGraphicObjectRelationship(compoundUID, movedObjectUID);
  }

  /* Sets the parent-child relationship for the Compound */
  setGraphicObjectRelationship(parentAxesUID, compoundUID);

  /* Deactivated */
#if 0
  ppCompound->callback = (char *)NULL;
  ppCompound->callbacklen = 0;
#endif

  /*
   * To be checked: difference between sciGetParentSubwin and directly getting
   * the Compound's parent's visible property
   */
#if 0
  ppCompound->visible = sciGetVisibility(sciGetParentSubwin(compound));
#endif

  getGraphicObjectProperty(parentAxesUID, __GO_VISIBLE__, jni_bool, &piParentVisible);
  setGraphicObjectProperty(compoundUID, __GO_VISIBLE__, &parentVisible, jni_bool, 1);

 /*
  * Not implemented within the MVC yet
  * To be implemented
  */
#if 0
  ppCompound->isselected = TRUE;
#endif

  return (char *) compoundUID;
}

/**sciConstructCompoundSeq
 * constructs a Compound of with the last n entities created in the current subwindow
 on entry the subwin children list is
 s1->s2->...->sn->sn+1->...->sN
 with sn the least recent of the last n entities created and s1 the most recent one
 (that is, the last entity created), and N the subwin's initial number of children
 on exit it is
 A->sn+1->sn+2->...->sN
 with A a Compound object whose children list is:
 s1->s2->...->sn-1->sn
*/
sciPointObj *
ConstructCompoundSeq (int number)
{
    char** children;
    char* parentFigure;
    int numberChildren = 0;
    int piNumberChildren = &numberChildren;
    int i;
    int visible = 0;
    int piVisible = &visible;

    sciPointObj *pobj;
    char* *psubwinUID;

    psubwinUID = getCurrentSubWin();

    /* Creates the Compound object A */
    if ((pobj = MALLOC ((sizeof (sciPointObj)))) == NULL)
    {
        return NULL;
    }

    pobj->UID = createGraphicObject(__GO_COMPOUND__);

    /* Adding the Compound's handle was previously done by sciStandardBuildOperations */
//    if (sciAddNewHandle(pobj) == -1)
//    {
//        deleteGraphicObject(pobj->UID);
//        FREE(pobj);
//        return NULL;
//    }

    getGraphicObjectProperty(psubwinUID, __GO_CHILDREN_COUNT__, jni_int, &piNumberChildren);

    getGraphicObjectProperty(psubwinUID, __GO_CHILDREN__, jni_string_vector, &children);

    /*
     * Remove the last "number" created objects (located at the children list's head)
     * and add them to the compound in the same order
     */
    for ( i = 0 ; i < number ; i++ )
    {
        /*
         * Set the parent-child relationship between the Compound and each aggregated object.
         * Children are added to the Compound from the least recent to the most recent, to
         * preserve their former ordering.
         */
        setGraphicObjectRelationship(pobj->UID, children[number-i-1]);
    }

    /* Sets the parent-child relationship for the Compound */
    setGraphicObjectRelationship(psubwinUID, pobj->UID);

    /* set Compound properties*/
    /* To be implemented */
#if 0
    initUserData(pobj);
    ppagr->callback = (char *)NULL;
    ppagr->callbacklen = 0;
#endif

    /*
     * visibility is obtained from the parent Figure, whereas it is retrieved from the
     * parent Axes in ConstructCompound.
     * To be made consistent.
     */
    getGraphicObjectProperty(pobj->UID, __GO_PARENT_FIGURE__, jni_string, &parentFigure);
    getGraphicObjectProperty(parentFigure, __GO_VISIBLE__, jni_bool, &piVisible);

    setGraphicObjectProperty(pobj->UID, __GO_VISIBLE__, &visible, jni_bool, 1);

    /* To be implemented */
#if 0
    ppagr->isselected = TRUE;
#endif

    return (sciPointObj *)pobj;
}



/**ConstructLabel
 * This function creates Label structure used for x,y,z labels and for the Title.
 * @param  sciPointObj *pparentsubwin
 * @param  char text[] : intial text string.
 * @param  int type to get info. on the type of label
 * @return  : pointer sciPointObj if ok , NULL if not
 */
sciPointObj *
ConstructLabel (sciPointObj * pparentsubwin, char *text, int type)
{
  sciPointObj * pobj = NULL;
  /* get a pointer on the feature */
  sciLabel    * ppLabel ;
  char * emptyString = "" ;
  int defaultColor = 0 ;

  if (sciGetEntityType (pparentsubwin) == SCI_SUBWIN)
  {
    if ((pobj = MALLOC (sizeof (sciPointObj))) == NULL)
    {
      return (sciPointObj *) NULL;
    }

    sciSetEntityType (pobj, SCI_LABEL);
    if ((pobj->pfeatures = MALLOC ((sizeof (sciLabel)))) == NULL)
    {
      FREE(pobj);
      return (sciPointObj *) NULL;
    }

    ppLabel = pLABEL_FEATURE( pobj ) ;

    ppLabel->text = allocateText( pparentsubwin, &emptyString, 1, 1,
                                  0.0, 0.0, TRUE, NULL, FALSE, &defaultColor, &defaultColor,
                                  FALSE, FALSE, FALSE, ALIGN_LEFT ) ;

		/* RelationShip is actually stored in the text object */
		pobj->relationShip = ppLabel->text->relationShip;

    sciStandardBuildOperations(pobj, pparentsubwin);

    /* labels are not clipped */
    sciSetIsClipping(ppLabel->text, -1) ;

    /* Use centered mode */
    sciInitCenterPos(ppLabel->text, FALSE);
    sciInitAutoSize(ppLabel->text, TRUE);

    /* 1.0 for logarithmic mode */
    sciInitTextPos(pobj, 1.0, 1.0, 1.0);

    sciInitIsFilled(pobj,FALSE); /* by default a simple text is display (if existing) */

    sciInitIs3d( pobj, FALSE ) ; /* the text of labels is displayed using 2d scale */

    ppLabel->ptype = type;
    ppLabel->auto_position = TRUE;
    ppLabel->auto_rotation = TRUE;

    ppLabel->isselected = TRUE;

    return pobj;
  }
  else
  {
    Scierror(999, _("The parent has to be a SUBWIN\n"));
    return (sciPointObj *) NULL;
  }
}
/*----------------------------------------------------------------------------*/
/**
 * contains the functions always called when creating an object
 * pObj should have just been allocated.
 * @return the modified object. Should be the same as pObj, unless an error occured.
 *         then it is NULL.
 */
sciPointObj * sciStandardBuildOperations( sciPointObj * pObj, sciPointObj * parent )
{

	/* Allocate relationShip */
	createDefaultRelationShip(pObj);

  /* add the handle in the handle list */
//  if ( sciAddNewHandle(pObj) == -1 )
//  {
//    return NULL ;
//  }


  /* connect the object under its parent in the hierarchy */
  //if ( !sciAddThisToItsParent( pObj, parent) )
  //{
  //  sciDelHandle(pObj) ;
  //  return NULL ;
  //}

  sciInitVisibility( pObj, TRUE ) ;

  initUserData(pObj);

  pObj->pObservers = DoublyLinkedList_new() ;
  createDrawingObserver( pObj ) ;


  pObj->pDrawer = NULL ;

  return pObj ;

}
/*----------------------------------------------------------------------------*/
/**
 * Create a figure if none exists.
 */
void SciWin(void)
{
// ???
#if 0
  if (!sciHasFigures())
  {
    sciGetCurrentFigure();
  }
#endif
}
/*----------------------------------------------------------------------------*/
/**
 * Get the first subwin object within a figure.
 * If not exists, create one withj default value.
 * @return the first subwin or NULL if an error occured during subwin creation
 */
sciPointObj * createFirstSubwin(sciPointObj * pFigure)
{
	if (sciGetNbTypedObjects(pFigure, SCI_SUBWIN) > 0)
	{
		/* return the first object */
		return sciGetFirstTypedSelectedSon(pFigure, SCI_SUBWIN);
	}
	else
	{
		/* No subwins, create the default one */
		sciPointObj * newSubwin = ConstructSubWin(pFigure);
		if (newSubwin != NULL)
		{
			sciSetCurrentObj(newSubwin);
			sciSetOriginalSubWin(pFigure, newSubwin);
			return newSubwin;
		}
		else
		{
			return NULL;
		}
	}
}
/*----------------------------------------------------------------------------*/
void createDefaultRelationShip(sciPointObj * pObj)
{
	/* Create a new relationship structure */
	sciRelationShip * relationShip = NULL;
	if(sciGetEntityType(pObj) == SCI_LABEL)
	{
		/* labels have their relationShip stored in their text objects */
		return;
	}

	relationShip = MALLOC(sizeof(sciRelationShip));
	if (relationShip == NULL)
	{
		return;
	}

	/* Set default values */
	relationShip->handleIndex = 0;
	relationShip->plastsons = NULL;
	relationShip->psons = NULL;
	relationShip->pparent = NULL;
	relationShip->pSelectedSon = DoublyLinkedList_new();

	/* Add it to the object */
	pObj->relationShip = relationShip;

}
/*----------------------------------------------------------------------------*/
/**
 * Initialize the user data of a graphic obj
 */
void initUserData(sciPointObj * pObj)
{
	int ** userData = NULL ;
  int *  udSize   = NULL ;

	sciGetPointerToUserData( pObj, &userData, &udSize ) ;
  *userData = NULL ;
  *udSize   = 0    ;
}
/*----------------------------------------------------------------------------*/
