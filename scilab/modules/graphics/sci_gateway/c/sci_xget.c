/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2011 - Scilab Enterprises - Bruno JOFRET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*------------------------------------------------------------------------*/
/* file: sci_xset.c                                                       */
/* desc : interface for xset routine                                      */
/*------------------------------------------------------------------------*/

#include <stdio.h>

#include "api_common.h"
#include "api_double.h"

#include "gw_graphics.h"
#include "stack-c.h"
#include "XsetXgetParameters.h"
#include "GetProperty.h"
#include "Format.h"
#include "Scierror.h"
#include "ObjectSelection.h"
#include "localization.h"
#include "returnProperty.h"
#include "HandleManagement.h"
#include "CurrentSubwin.h"
#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"
#include "BuildObjects.h"

#include "getHandleProperty.h"
#include "CurrentFigure.h"
/*--------------------------------------------------------------------------*/
int xgetg( char * str, char * str1, int * len,int  lx0,int lx1);
/*--------------------------------------------------------------------------*/
int sci_xget(char *fname,unsigned long fname_len)
{
    int flagx=0,x2=0, m1,n1,l1,m2,n2,l2,l3,i ;
    int one = 1;
    int two = 2;
    BOOL keyFound = FALSE ;
    void *psubwin = NULL;

    if ( Rhs <= 0 )
    {
        sci_demo(fname, fname_len);
        return 0;
    }

    CheckRhs(1,2);
    CheckLhs(0,1);

    GetRhsVar(1,STRING_DATATYPE,&m1,&n1,&l1);

    /* check if key is valid */
    for (i=0; i < NUMSETFONC ; i++)
    {
        if ( strcmp(cstk(l1),KeyTab_[i]) == 0 )
        {
            keyFound = TRUE ;
            break ;
        }
    }

    if ( !keyFound )
    {
        Scierror(999, _("%s: Unrecognized input argument: '%s'.\n"), fname, cstk(l1));
        return -1;
    }

    if (Rhs == 2) {
        GetRhsVar(2,MATRIX_OF_DOUBLE_DATATYPE,&m2,&n2,&l2);
        CheckScalar(2,m2,n2);
        flagx = (int)  *stk(l2); }
    else
        flagx = 0;

    if ( strcmp(cstk(l1),"fpf") == 0 || strcmp(cstk(l1),"auto clear") == 0)
    {
        int bufl;
        /*     special case for global variables set */
        xgetg( cstk(l1),C2F(cha1).buf,&bufl,m1,bsiz);
        CreateVar(Rhs+1,STRING_DATATYPE,&bufl,&one,&l3);
        strncpy(cstk(l3),C2F(cha1).buf,bufl);
        LhsVar(1)=Rhs+1;
        PutLhsVar();
        return 0;
    }
    else if ( strcmp(cstk(l1),"colormap") == 0)
    {
        char *pobjUID = NULL;
        // Force figure creation if none exists.
        getOrCreateDefaultSubwin();
        pobjUID = getCurrentFigure();

        get_color_map_property(pobjUID);

        LhsVar(1) = Rhs + 1;
        PutLhsVar();

        return 0;
    }
    else if ( strcmp(cstk(l1),"mark") == 0)
    {
        char *pobjUID = getOrCreateDefaultSubwin();
        int iMarkStyle = 0;
        int* piMarkStyle = &iMarkStyle;
        int iMarkSize = 0;
        int* piMarkSize = &iMarkSize;
        double pdblResult[2];

        getGraphicObjectProperty(pobjUID, __GO_MARK_STYLE__, jni_int, &piMarkStyle);
        getGraphicObjectProperty(pobjUID, __GO_MARK_SIZE__, jni_int, &piMarkSize);
        pdblResult[0] = iMarkStyle;
        pdblResult[1] = iMarkSize;

        createMatrixOfDouble(pvApiCtx, Rhs + 1, 1, 2, pdblResult);
        LhsVar(1) = Rhs + 1;
        PutLhsVar();

        return 0;
    }
    else if ( strcmp(cstk(l1),"mark size") == 0)
    {
        char *pobjUID = getOrCreateDefaultSubwin();
        get_mark_size_property(pobjUID);

        LhsVar(1) = Rhs + 1;
        PutLhsVar();

        return 0;
    }
    else if ( strcmp(cstk(l1),"line style") == 0)
    {
        get_line_style_property(getOrCreateDefaultSubwin());

        LhsVar(1) = Rhs + 1;
        PutLhsVar();
        return 0;
    }
    else if(strcmp(cstk(l1),"clipping") == 0)
    {
        double *clipBox;
        char* pobjUID = getOrCreateDefaultSubwin();

        getGraphicObjectProperty(pobjUID, __GO_CLIP_BOX__, jni_double_vector, &clipBox);

        createMatrixOfDouble(pvApiCtx, Rhs + 1, 1, 4, clipBox);
        LhsVar(1) = Rhs + 1;
        PutLhsVar();
        return 0;
    }
    else if(strcmp(cstk(l1),"font")==0)
    {
        char *pobjUID = getOrCreateDefaultSubwin();
        double dblFontSize = 0;
        double* pdblFontSize = &dblFontSize;
        int iFontStyle = 0;
        int* piFontStyle = &iFontStyle;
        double pdblResult[2];

        getGraphicObjectProperty(pobjUID, __GO_FONT_SIZE__, jni_double, &pdblFontSize);
        getGraphicObjectProperty(pobjUID, __GO_FONT_STYLE__, jni_int, &piFontStyle);

        pdblResult[0] = iFontStyle;
        pdblResult[1] = dblFontSize;


        createMatrixOfDouble(pvApiCtx, Rhs + 1, 1, 2, pdblResult);
        LhsVar(1) = Rhs + 1;
        PutLhsVar();
        return 0;
    }
    else if(strcmp(cstk(l1),"font size")==0)
    {
        double dblFontSize = 0;
        double* pdblFontSize = &dblFontSize;
        getGraphicObjectProperty(getOrCreateDefaultSubwin(), __GO_FONT_SIZE__, jni_double, &pdblFontSize);

        createScalarDouble(pvApiCtx, Rhs + 1, dblFontSize);
        LhsVar(1) = Rhs + 1;
        PutLhsVar();

        return 0;
    }
    else if(strcmp(cstk(l1),"dashes")==0)
    {
        int iLineStyle = 0;
        int* piLineStyle = &iLineStyle;

        getGraphicObjectProperty(getOrCreateDefaultSubwin(), __GO_LINE_STYLE__, jni_int, &piLineStyle);

        createScalarDouble(pvApiCtx, Rhs + 1, iLineStyle);
        LhsVar(1) = Rhs + 1;
        PutLhsVar();

        return 0;
    }
    else if(strcmp(cstk(l1),"hidden3d")==0)
    {
        get_hidden_color_property(getOrCreateDefaultSubwin());

        LhsVar(1) = Rhs + 1;
        PutLhsVar();

        return 0;
    }
    else if(strcmp(cstk(l1),"window")== 0 || strcmp(cstk(l1),"figure") == 0)
    {
        int iFigureId = 0;
        int* piFigureId = &iFigureId;

        getOrCreateDefaultSubwin();
        getGraphicObjectProperty(getCurrentFigure(), __GO_ID__, jni_int, &piFigureId);
        createScalarDouble(pvApiCtx, Rhs + 1, iFigureId);
        LhsVar(1) = Rhs + 1;
        PutLhsVar();
        return 0;
    }
    else if(strcmp(cstk(l1),"thickness") == 0)
    {
        get_thickness_property(getOrCreateDefaultSubwin());

        LhsVar(1) = Rhs + 1;
        PutLhsVar();

        return 0;
    }
    else if(strcmp(cstk(l1),"wdim") == 0 || strcmp(cstk(l1),"wpdim") == 0)
    {
        int *piFigureSize;
        double pdblFigureSize[2];

        getOrCreateDefaultSubwin();
        getGraphicObjectProperty(getCurrentFigure(), __GO_SIZE__, jni_int_vector, (void **) &piFigureSize);
        pdblFigureSize[0] = (double) piFigureSize[0];
        pdblFigureSize[1] = (double) piFigureSize[1];

        createMatrixOfDouble(pvApiCtx, Rhs + 1, 1, 2, pdblFigureSize);
        LhsVar(1) = Rhs + 1;
        PutLhsVar();

        return 0;
    }
    else if(strcmp(cstk(l1),"wpos") == 0)
    {
        int *piFigurePosition;
        double pdblFigurePosition[2];

        getOrCreateDefaultSubwin();
        getGraphicObjectProperty(getCurrentFigure(), __GO_POSITION__, jni_int_vector, (void **) &piFigurePosition);
        pdblFigurePosition[0] = piFigurePosition[0];
        pdblFigurePosition[1] = piFigurePosition[1];

        createMatrixOfDouble(pvApiCtx, Rhs + 1, 1, 2, pdblFigurePosition);
        LhsVar(1) = Rhs + 1;
        PutLhsVar();

        return 0;
    }
    else if(strcmp(cstk(l1),"viewport") == 0)
    {
        int* viewport;
        double pdblViewport[2];

        getOrCreateDefaultSubwin();
        getGraphicObjectProperty(getCurrentFigure(), __GO_VIEWPORT__, jni_int_vector, &viewport);
        pdblViewport[0] = viewport[0];
        pdblViewport[1] = viewport[1];

        createMatrixOfDouble(pvApiCtx, Rhs + 1, 1, 2, pdblViewport);
        LhsVar(1) = Rhs + 1;
        PutLhsVar();

        return 0;
    }
    else if(strcmp(cstk(l1),"background") == 0)
    {
        get_background_property(getOrCreateDefaultSubwin());

        LhsVar(1) = Rhs + 1;
        PutLhsVar();

        return 0;
    }
    else if(   strcmp(cstk(l1),"color") == 0
               || strcmp(cstk(l1),"foreground") == 0
               || strcmp(cstk(l1),"pattern") == 0)
    {
        get_foreground_property(getOrCreateDefaultSubwin());

        LhsVar(1) = Rhs + 1;
        PutLhsVar();

        return 0;
    }
    else if(strcmp(cstk(l1),"lastpattern") == 0)
    {
        int iNumColors = 0;
        int* piNumColors = &iNumColors;

        getOrCreateDefaultSubwin();
        getGraphicObjectProperty(getCurrentFigure(), __GO_COLORMAP_SIZE__, jni_int, &piNumColors);

        createScalarDouble(pvApiCtx, Rhs + 1, iNumColors);

        LhsVar(1) = Rhs + 1;
        PutLhsVar();

        return 0;
    }
    else if(strcmp(cstk(l1),"line mode") == 0)
    {
        int iLineMode = 0;
        int* lineMode = &iLineMode;

        getGraphicObjectProperty(getOrCreateDefaultSubwin(), __GO_LINE_MODE__, jni_bool, &lineMode);

        createScalarDouble(pvApiCtx, Rhs + 1, iLineMode);

        LhsVar(1) = Rhs + 1;
        PutLhsVar();

        return 0;
    }
    else if(strcmp(cstk(l1),"pixmap") == 0)
    {
        int iPixmap = 0;
        int *piPixmap = &iPixmap;

        getOrCreateDefaultSubwin();
        getGraphicObjectProperty(getCurrentFigure(), __GO_PIXMAP__, jni_bool, &piPixmap);

        createScalarDouble(pvApiCtx, Rhs + 1, iPixmap);
        LhsVar(1) = Rhs + 1;
        PutLhsVar();

        return 0;
    }
    else if(strcmp(cstk(l1),"white") == 0)
    {
        int iNumColors = 0;
        int* piNumColors = &iNumColors;

        getOrCreateDefaultSubwin();
        getGraphicObjectProperty(getCurrentFigure(), __GO_COLORMAP_SIZE__, jni_int, &piNumColors);

        /* White is lqst colormap index + 2 */
        createScalarDouble(pvApiCtx, Rhs + 1, iNumColors + 2);

        LhsVar(1) = Rhs + 1;
        PutLhsVar();

        return 0;
    }
    else if(strcmp(cstk(l1),"wresize") == 0)
    {
        // autoresize property
        int iAutoResize = 0;
        int* piAutoResize =  &iAutoResize;

        getOrCreateDefaultSubwin();
        getGraphicObjectProperty(getCurrentFigure(), __GO_AUTORESIZE__, jni_bool, &piAutoResize);

        createScalarDouble(pvApiCtx, Rhs + 1, iAutoResize);

        LhsVar(1) = Rhs + 1;
        PutLhsVar();

        return 0;
    }
    else if( strcmp(cstk(l1),"clipgrf") == 0 )
    {
        /* clip_state : 0 = off, 1 = on */
        int iClipState = 0;
        int* piClipState = &iClipState;

        getGraphicObjectProperty(getOrCreateDefaultSubwin(), __GO_CLIP_STATE__, jni_int, &piClipState);

        createScalarDouble(pvApiCtx, Rhs + 1, iClipState);
        LhsVar(1) = Rhs + 1;
        PutLhsVar();

        return 0;
    }
    else if( strcmp(cstk(l1),"clipoff") == 0 )
    {
        int iClipState = 0;
        int* piClipState = &iClipState;

        getGraphicObjectProperty(getOrCreateDefaultSubwin(), __GO_CLIP_STATE__, jni_int, &piClipState);

        /* clip_state : 0 = off, 1 = on */
        if (iClipState == 0)
        {
            createScalarDouble(pvApiCtx, Rhs + 1, 1);
        }
        else
        {
            createScalarDouble(pvApiCtx, Rhs + 1, 0);
        }
        LhsVar(1) = Rhs + 1;
        PutLhsVar();

        return 0;
    }
    else
    {
        Scierror(999, _("%s: Unrecognized input argument: '%s'.\n"), fname, cstk(l1));
        return -1;
    }
    return 0;
}
/*--------------------------------------------------------------------------*/
int xgetg( char * str, char * str1, int * len,int  lx0,int lx1)
{
    if ( strcmp(str,"fpf") == 0)
    {
        strncpy(str1,getFPF(),32);
        *len= (int) strlen(str1);
    }
    else if ( strcmp(str,"auto clear")==0)
    {
        int iAutoClear = 0;
        int* piAutoClear = &iAutoClear;

        getGraphicObjectProperty(getOrCreateDefaultSubwin(), __GO_AUTO_CLEAR__, jni_bool, &piAutoClear);
        if (iAutoClear == 1)
        {
            strncpy(str1,"on",2);
            *len=2;
        }
        else
        {
            strncpy(str1,"off",3);
            *len=3;
        }
    }
    return 0;
}
/*--------------------------------------------------------------------------*/
