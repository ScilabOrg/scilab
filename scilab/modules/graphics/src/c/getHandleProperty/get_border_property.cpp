/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2014 - Scilab Enterprises - Antoine ELIAS
* Copyright (C) 2014 - Scilab Enterprises - Cedric Delamarre
 *
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

/*------------------------------------------------------------------------*/
/* file: get_figure_name_property.c                                       */
/* desc : function to retrieve in Scilab the figure_name field of a       */
/*        handle                                                          */
/*------------------------------------------------------------------------*/

#include "internal.hxx"
#include "list.hxx"
#include "tlist.hxx"
#include "double.hxx"
#include "string.hxx"
#include "bool.hxx"

extern "C"
{
#include <string.h>
//#include "api_scilab.h"
#include "Scierror.h"
#include "localization.h"
#include "SetPropertyStatus.h"
//#include "stricmp.h"

#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"
#include "FrameBorderType.h"
//#include "returnPropertyList.h"
}

/*------------------------------------------------------------------------*/
void* getBorder             (types::InternalType* _pITParent, int _iPos, int _iObjUID);
void* getNoBorder           (types::InternalType* _pITParent, int _iPos, int _iObjUID);
void* getLineBorder         (types::InternalType* _pITParent, int _iPos, int _iObjUID);
void* getBevelBorder        (types::InternalType* _pITParent, int _iPos, int _iObjUID);
void* getSoftBevelBorder    (types::InternalType* _pITParent, int _iPos, int _iObjUID);
void* getCommonBevelBorder  (types::InternalType* _pITParent, int _iPos, int _iObjUID, int _iBevel);
void* getEtchedBorder       (types::InternalType* _pITParent, int _iPos, int _iObjUID);
void* getTitledBorder       (types::InternalType* _pITParent, int _iPos, int _iObjUID);
void* getEmptyBorder        (types::InternalType* _pITParent, int _iPos, int _iObjUID);
void* getCompoundBorder     (types::InternalType* _pITParent, int _iPos, int _iObjUID);
void* getMatteBorder        (types::InternalType* _pITParent, int _iPos, int _iObjUID);
/*------------------------------------------------------------------------*/
extern "C"
{
void* get_border_property(void* _pvCtx, int iObjUID)
{
    int iBorder   = 0;
    int* piBorder = &iBorder;

    getGraphicObjectProperty(iObjUID, __GO_UI_FRAME_BORDER__, jni_int, (void **)&piBorder);
    if (piBorder == NULL)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "border");
        return NULL;
    }

    return getBorder(NULL, 0, iBorder);
}
}
/*------------------------------------------------------------------------*/
void* getBorder(types::InternalType* _pITParent, int _iPos, int _iObjUID)
{
    int iStyle   = 0;
    int* piStyle = &iStyle;

    getGraphicObjectProperty(_iObjUID, __GO_UI_FRAME_BORDER_STYLE__, jni_int, (void **)&piStyle);
    if (piStyle == NULL)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "border");
        return NULL;
    }

    switch (iStyle)
    {
        default:
        {
            return getNoBorder(_pITParent, _iPos, _iObjUID);
        }
        break;
        case LINE :
        {
            return getLineBorder(_pITParent, _iPos, _iObjUID);
        }
        break;
        case BEVEL :
        {
            return getBevelBorder(_pITParent, _iPos, _iObjUID);
        }
        break;
        case SOFTBEVEL :
        {
            return getSoftBevelBorder(_pITParent, _iPos, _iObjUID);
        }
        break;
        case ETCHED :
        {
            return getEtchedBorder(_pITParent, _iPos, _iObjUID);
        }
        break;
        case TITLED :
        {
            return getTitledBorder(_pITParent, _iPos, _iObjUID);
        }
        break;
        case EMPTY :
        {
            return getEmptyBorder(_pITParent, _iPos, _iObjUID);
        }
        break;
        case COMPOUND :
        {
            return getCompoundBorder(_pITParent, _iPos, _iObjUID);
        }
        break;
        case MATTE :
        {
            return getMatteBorder(_pITParent, _iPos, _iObjUID);
        }
        break;
    }

    return 0;
}
/*------------------------------------------------------------------------*/
void* getNoBorder(types::InternalType* _pITParent, int _iPos, int _iObjUID)
{
    types::TList* pTL = new types::TList();
    pTL->append(new types::String("NoBorder"));

    if (_pITParent)
    {
        types::List* pL = _pITParent->getAs<types::List>();
        pL->set(_iPos - 1, pTL);
    }

    return pTL;
}
/*------------------------------------------------------------------------*/
void* getLineBorder(types::InternalType* _pITParent, int _iPos, int _iObjUID)
{
    // properties
    char* pstColor      = NULL;
    int iThickness      = 0;
    int* piThickness    = &iThickness;
    int iRounded        = 0;
    int* piRounded      = &iRounded;
    int iListSize       = 4;

    char* pstFieldList[] = {"LineBorder", "color", "thickness", "rounded"};

    getGraphicObjectProperty(_iObjUID, __GO_UI_FRAME_BORDER_COLOR__, jni_string, (void **)&pstColor);
    if (pstColor == NULL)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "border");
        return NULL;
    }

    getGraphicObjectProperty(_iObjUID, __GO_LINE_THICKNESS__, jni_int, (void **)&piThickness);
    getGraphicObjectProperty(_iObjUID, __GO_UI_FRAME_BORDER_ROUNDED__, jni_bool, (void **)&piRounded);

    if (piRounded == NULL)
    {
        iListSize = 3;
    }

    if (piThickness == NULL)
    {
        iListSize = 2;
    }

    types::TList* pTL   = new types::TList();
    types::String* pStr = new types::String(1, iListSize);

    for (int i = 0; i < iListSize; i++)
    {
        pStr->set(i, pstFieldList[i]);
    }

    pTL->append(pStr);
    pTL->append(new types::String(pstColor));

    if (iListSize > 2)
    {
        pTL->append(new types::Double((double)iThickness));
    }

    if (iListSize > 3)
    {
        pTL->append(new types::Bool(iRounded));
    }

    if (_pITParent)
    {
        types::List* pL = _pITParent->getAs<types::List>();
        pL->set(_iPos - 1, pTL);
    }

    return pTL;
}
/*------------------------------------------------------------------------*/
void* getBevelBorder(types::InternalType* _pITParent, int _iPos, int _iObjUID)
{
    return getCommonBevelBorder(_pITParent, _iPos, _iObjUID, 1);
}
/*------------------------------------------------------------------------*/
void* getSoftBevelBorder(types::InternalType* _pITParent, int _iPos, int _iObjUID)
{
    return getCommonBevelBorder(_pITParent, _iPos, _iObjUID, 2);
}
/*------------------------------------------------------------------------*/
void* getCommonBevelBorder(types::InternalType* _pITParent, int _iPos, int _iObjUID, int _iBevel)
{
    char* pstFieldList1[]       = {"BevelBorder", "type", "hlouter", "hlinner", "shadowouter", "shadowinner"};
    char* pstFieldList2[]       = {"BevelBorder", "type", "hlouter", "shadowouter"};
    char** pstFieldList         = pstFieldList1;

    char* pstSoftFieldList1[]   = {"SoftBevelBorder", "type", "hlouter", "hlinner", "shadowouter", "shadowinner"};
    char* pstSoftFieldList2[]   = {"SoftBevelBorder", "type", "hlouter", "shadowouter"};
    char** pstSoftFieldList     = pstSoftFieldList1;

    int iListSize = 6;

    // properties
    int iType               = 0;
    int* piType             = &iType;
    char* pstType[]         = {"raised", "lowered"};
    char* pstHlOutColor     = NULL;
    char* pstHlInColor      = NULL;
    char* pstShadowOutColor = NULL;
    char* pstShadowInColor  = NULL;

    getGraphicObjectProperty(_iObjUID, __GO_UI_FRAME_BORDER_TYPE__, jni_int, (void **)&piType);
    if (piType == NULL)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "border");
        return NULL;
    }

    getGraphicObjectProperty(_iObjUID, __GO_UI_FRAME_BORDER_HIGHLIGHT_OUT__, jni_string, (void **)&pstHlOutColor);
    getGraphicObjectProperty(_iObjUID, __GO_UI_FRAME_BORDER_HIGHLIGHT_IN__, jni_string, (void **)&pstHlInColor);
    getGraphicObjectProperty(_iObjUID, __GO_UI_FRAME_BORDER_SHADOW_OUT__, jni_string, (void **)&pstShadowOutColor);
    getGraphicObjectProperty(_iObjUID, __GO_UI_FRAME_BORDER_SHADOW_IN__, jni_string, (void **)&pstShadowInColor);

    if (pstHlInColor == NULL || pstShadowInColor == NULL)
    {
        iListSize = 4;
        pstFieldList = pstFieldList2;
        pstSoftFieldList = pstSoftFieldList2;
    }

    if (pstHlOutColor == NULL || pstShadowOutColor == NULL)
    {
        iListSize = 2;
        pstFieldList = pstFieldList2;
        pstSoftFieldList = pstSoftFieldList2;
    }

    types::TList* pTL   = new types::TList();
    types::String* pStr = new types::String(1, iListSize);

    if (_iBevel == 1)
    {
        for (int i = 0; i < iListSize; i++)
        {
            pStr->set(i, pstFieldList[i]);
        }
    }
    else
    {
        for (int i = 0; i < iListSize; i++)
        {
            pStr->set(i, pstSoftFieldList[i]);
        }
    }

    pTL->append(pStr);
    pTL->append(new types::String(pstType[iType]));

    if (iListSize == 4)
    {
        pTL->append(new types::String(pstHlOutColor));
        pTL->append(new types::String(pstShadowOutColor));
    }
    else if (iListSize == 6)
    {
        pTL->append(new types::String(pstHlOutColor));
        pTL->append(new types::String(pstHlInColor));
        pTL->append(new types::String(pstShadowOutColor));
        pTL->append(new types::String(pstShadowInColor));
    }

    if (_pITParent)
    {
        types::List* pL = _pITParent->getAs<types::List>();
        pL->set(_iPos - 1, pTL);
    }

    return pTL;
}
/*------------------------------------------------------------------------*/
void* getEtchedBorder(types::InternalType* _pITParent, int _iPos, int _iObjUID)
{
    char* pstFieldList1[]   = {"EtchedBorder", "type", "hl", "shadow"};
    char* pstFieldList2[]   = {"EtchedBorder", "hl", "shadow"};
    char** pstFieldList     = pstFieldList1;
    int iListSize           = 4;

    // properties
    int iType               = 0;
    int* piType             = &iType;
    char* pstType[]         = {"raised", "lowered"};
    char* pstHlOutColor     = NULL;
    char* pstShadowOutColor = NULL;

    getGraphicObjectProperty(_iObjUID, __GO_UI_FRAME_BORDER_TYPE__, jni_int, (void **)&piType);
    getGraphicObjectProperty(_iObjUID, __GO_UI_FRAME_BORDER_HIGHLIGHT_OUT__, jni_string, (void **)&pstHlOutColor);
    getGraphicObjectProperty(_iObjUID, __GO_UI_FRAME_BORDER_SHADOW_OUT__, jni_string, (void **)&pstShadowOutColor);

    if (piType == NULL)
    {
        if (pstHlOutColor == NULL || pstShadowOutColor == NULL)
        {
            iListSize = 1;
        }
        else
        {
            iListSize = 3;
            pstFieldList = pstFieldList2;
        }
    }
    else
    {
        if (pstHlOutColor == NULL || pstShadowOutColor == NULL)
        {
            iListSize = 2;
        }
        else
        {
            iListSize = 4;
        }
    }

    types::TList* pTL   = new types::TList();
    types::String* pStr = new types::String(1, iListSize);

    for (int i = 0; i < iListSize; i++)
    {
        pStr->set(i, pstFieldList[i]);
    }

    pTL->append(pStr);

    if (iListSize == 2 || iListSize == 4)
    {
        pTL->append(new types::String(pstType[iType]));
    }

    if (iListSize == 3 || iListSize == 4)
    {
        pTL->append(new types::String(pstHlOutColor));
        pTL->append(new types::String(pstShadowOutColor));
    }

    if (_pITParent)
    {
        types::List* pL = _pITParent->getAs<types::List>();
        pL->set(_iPos - 1, pTL);
    }

    return pTL;
}
/*------------------------------------------------------------------------*/
void* getTitledBorder(types::InternalType* _pITParent, int _iPos, int _iObjUID)
{
    char* pstFieldList1[]       = {"TitledBorder", "border", "title", "justification", "position", "font", "color"};
    char* pstFieldList2[]       = {"TitledBorder", "title"};
    char** pstFieldList         = pstFieldList1;
    char* pstJustification[]    = {"leading" , "left" , "center" , "right" , "trailing"};
    char* pstPosition[]         = {"top" , "above_top" , "below_top" , "bottom" , "above_bottom", "below_bottom"};

    int iListSize = 7;

    // properties
    int iChildBorder        = 0;
    int* piChildBorder      = &iChildBorder;
    char* pstTitle          = NULL;
    int iJustification      = 0;
    int* piJustification    = &iJustification;
    char* pstFontName       = NULL;
    char* pstFontAngle      = NULL;
    int iFontSize           = 0;
    int* piFontSize         = &iFontSize;
    char* pstFontWeight     = NULL;
    int iPosition           = 0;
    int* piPosition         = &iPosition;
    char* pstColor          = NULL;

    getGraphicObjectProperty(_iObjUID, __GO_UI_FRAME_BORDER_TITLE__, jni_int, (void **)&piChildBorder);
    getGraphicObjectProperty(_iObjUID, __GO_TITLE__, jni_string, (void **)&pstTitle);
    getGraphicObjectProperty(_iObjUID, __GO_UI_FRAME_BORDER_JUSTIFICATION__, jni_int, (void **)&piJustification);
    getGraphicObjectProperty(_iObjUID, __GO_UI_FONTNAME__, jni_string, (void **)&pstFontName);
    getGraphicObjectProperty(_iObjUID, __GO_UI_FONTANGLE__, jni_string, (void **)&pstFontAngle);
    getGraphicObjectProperty(_iObjUID, __GO_UI_FONTSIZE__, jni_int, (void **)&piFontSize);
    getGraphicObjectProperty(_iObjUID, __GO_UI_FONTWEIGHT__, jni_string, (void **)&pstFontWeight);
    getGraphicObjectProperty(_iObjUID, __GO_UI_FRAME_BORDER_POSITION__, jni_int, (void **)&piPosition);
    getGraphicObjectProperty(_iObjUID, __GO_UI_FRAME_BORDER_COLOR__, jni_string, (void **)&pstColor);

    //2 3 5 6 7
    if (pstColor == NULL)
    {
        iListSize = 6;
    }

    if (pstFontName == NULL || pstFontAngle == NULL || piFontSize == NULL || pstFontWeight == NULL)
    {
        iListSize = 5;
    }

    if (piJustification == NULL)
    {
        iListSize = 3;
    }

    if (pstTitle == NULL)
    {
        iListSize = 2;
    }

    if (piChildBorder == NULL)
    {
        iListSize = 2;
        pstFieldList = pstFieldList2;
    }

    types::TList* pTL   = new types::TList();
    types::String* pStr = new types::String(1, iListSize);

    for (int i = 0; i < iListSize; i++)
    {
        pStr->set(i, pstFieldList[i]);
    }

    if (piChildBorder)
    {
        //get child information and fill current list
        if (getBorder(pTL, 2, iChildBorder) == NULL)
        {
            return NULL;
        }
    }

    if (pstTitle)
    {
        //pos 2 or 3
        pTL->append(new types::String(pstTitle));
    }

    if (iListSize > 3)
    {
        pTL->set(3, new types::String(pstJustification[iJustification]));
        pTL->set(4, new types::String(pstPosition[iPosition]));
    }

    if (iListSize > 5)
    {
        //create a Font Border
        types::TList* pTLBorder = new types::TList();

        types::String* pStrBorder = new types::String(1, 5);
        pStrBorder->set(0, "BorderFont");
        pStrBorder->set(1, "name");
        pStrBorder->set(2, "size");
        pStrBorder->set(3, "angle");
        pStrBorder->set(4, "weight");

        pTLBorder->append(pStrBorder);
        pTLBorder->append(new types::String(pstFontName));
        pTLBorder->append(new types::Double((double)iFontSize));
        pTLBorder->append(new types::String(pstFontAngle));
        pTLBorder->append(new types::String(pstFontWeight));

        pTL->set(5, pTLBorder);
    }

    if (iListSize > 6)
    {
        pTL->set(6, new types::String(pstColor));
    }

    if (_pITParent)
    {
        types::List* pL = _pITParent->getAs<types::List>();
        pL->set(_iPos - 1, pTL);
    }

    return pTL;
}
/*------------------------------------------------------------------------*/
void* getEmptyBorder(types::InternalType* _pITParent, int _iPos, int _iObjUID)
{
    char* pstFieldList[] = {"EmptyBorder", "top", "left", "bottom", "right"};
    int iListSize        = 5;
    double* pdblPosition = NULL;

    getGraphicObjectProperty(_iObjUID, __GO_POSITION__, jni_double_vector, (void **)&pdblPosition);

    if (pdblPosition == NULL)
    {
        iListSize = 1;
    }

    types::TList* pTL   = new types::TList();
    types::String* pStr = new types::String(1, iListSize);

    for (int i = 0; i < iListSize; i++)
    {
        pStr->set(i, pstFieldList[i]);
    }

    pTL->append(pStr);

    if (iListSize > 1)
    {
        pTL->append(new types::Double(pdblPosition[0]));
        pTL->append(new types::Double(pdblPosition[1]));
        pTL->append(new types::Double(pdblPosition[2]));
        pTL->append(new types::Double(pdblPosition[3]));
    }

    if (_pITParent)
    {
        types::List* pL = _pITParent->getAs<types::List>();
        pL->set(_iPos - 1, pTL);
    }

    return pTL;
}
/*------------------------------------------------------------------------*/
void* getCompoundBorder(types::InternalType* _pITParent, int _iPos, int _iObjUID)
{
    char* pstFieldList[] = {"CompoundBorder", "outer", "inner"};
    int iListSize        = 3;

    // properties
    int iChildBorderOut     = 0;
    int* piChildBorderOut   = &iChildBorderOut;
    int iChildBorderIn      = 0;
    int* piChildBorderIn    = &iChildBorderIn;

    getGraphicObjectProperty(_iObjUID, __GO_UI_FRAME_BORDER_OUT_BORDER__, jni_int, (void **)&piChildBorderOut);
    getGraphicObjectProperty(_iObjUID, __GO_UI_FRAME_BORDER_IN_BORDER__, jni_int, (void **)&piChildBorderIn);

    if (piChildBorderOut == NULL || piChildBorderIn == NULL)
    {
        iListSize = 1;
    }

    types::TList* pTL   = new types::TList();
    types::String* pStr = new types::String(1, iListSize);

    for (int i = 0; i < iListSize; i++)
    {
        pStr->set(i, pstFieldList[i]);
    }

    pTL->append(pStr);

    if (iListSize > 1)
    {
        //get child information and fill current list
        if (getBorder(pTL, 2, iChildBorderOut) == NULL)
        {
            return NULL;
        }

        if (getBorder(pTL, 3, iChildBorderIn) == NULL)
        {
            return NULL;
        }
    }

    if (_pITParent)
    {
        types::List* pL = _pITParent->getAs<types::List>();
        pL->set(_iPos - 1, pTL);
    }

    return pTL;
}
/*------------------------------------------------------------------------*/
void* getMatteBorder(types::InternalType* _pITParent, int _iPos, int _iObjUID)
{
    // properties
    double* pdblPosition = NULL;
    char* pstColor       = NULL;

    getGraphicObjectProperty(_iObjUID, __GO_POSITION__, jni_double_vector, (void **)&pdblPosition);
    getGraphicObjectProperty(_iObjUID, __GO_UI_FRAME_BORDER_COLOR__, jni_string, (void **)&pstColor);

    types::TList* pTL   = new types::TList();
    types::String* pStr = new types::String(1, 6);
    pStr->set(0, "MatteBorder");
    pStr->set(1, "top");
    pStr->set(2, "left");
    pStr->set(3, "bottom");
    pStr->set(4, "right");
    pStr->set(5, "color");

    pTL->append(pStr);
    pTL->append(new types::Double(pdblPosition[0]));
    pTL->append(new types::Double(pdblPosition[1]));
    pTL->append(new types::Double(pdblPosition[2]));
    pTL->append(new types::Double(pdblPosition[3]));
    pTL->append(new types::String(pstColor));

    if (_pITParent)
    {
        types::List* pL = _pITParent->getAs<types::List>();
        pL->set(_iPos - 1, pTL);
    }

    return pTL;
}
/*------------------------------------------------------------------------*/
