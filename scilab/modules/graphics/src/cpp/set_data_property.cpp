/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2002-2004 - INRIA - Djalel Abdemouche
 * Copyright (C) 2004-2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Allan Cornet
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2010 - DIGITEO - Bruno JOFRET
 * Copyright (C) 2010-2011 - DIGITEO - Manuel Juliachs
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
/* file: set_data_property.c                                              */
/* desc : function to modify in Scilab the data field of                  */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "types.hxx"
#include "double.hxx"
#include "int.hxx"

extern "C"
{
#include "set_data_property.h"
#include "setGraphicObjectProperty.h"
#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"
#include "SetPropertyStatus.h"
#include "SetProperty.h"
#include "Scierror.h"
#include "localization.h"
#include "Matplot.h"
}

int set_data_property(void* _pvCtx, int iObjUID, void* _pvData, int valueType, int nbRow, int nbCol)
{
    int type = -1;
    int *piType = &type;

    getGraphicObjectProperty(iObjUID, __GO_TYPE__, jni_int, (void **)&piType);

    if (type == __GO_CHAMP__)
    {
        AssignedList* tlist = NULL;
        int status = -1;

        if (!(valueType  == sci_tlist))
        {
            Scierror(999, _("Wrong type for input argument: A tlist expected.\n"));
            return SET_PROPERTY_ERROR;
        }

        /* we should have 4 properties in the tlist */
        tlist = createAssignedList(_pvCtx, 3, 4);
        if (tlist == NULL)
        {
            return SET_PROPERTY_ERROR;
        }

        status = setchampdata(_pvCtx, iObjUID, tlist);
        destroyAssignedList(tlist);
        return status;
    }
    else if (type == __GO_GRAYPLOT__)
    {
        AssignedList * tlist = NULL;
        int status = -1;

        if (!(valueType  == sci_tlist))
        {
            Scierror(999, _("Wrong type for input argument: A tlist expected.\n"));
            return SET_PROPERTY_ERROR;
        }

        /* we should have 3 properties in the tlist */
        tlist = createAssignedList(_pvCtx, 3, 3);
        if (tlist == NULL)
        {
            return SET_PROPERTY_ERROR;
        }

        status = setgrayplotdata(_pvCtx, iObjUID, tlist);
        destroyAssignedList(tlist);
        return status;
    }
    else if (type == __GO_FAC3D__ || type == __GO_PLOT3D__)
    {
        AssignedList * tlist = NULL;
        int status = -1;
        int listSize = 0;

        if (!(valueType  == sci_tlist))
        {
            Scierror(999, _("Wrong type for input argument: A tlist expected.\n"));
            return SET_PROPERTY_ERROR;
        }

        listSize = getStackListNbElement(_pvCtx, 3);

        if (listSize == 3)
        {
            tlist = createAssignedList(_pvCtx, 3, 3);
        }
        else if (listSize == 4)
        {
            tlist = createAssignedList(_pvCtx, 3, 4);
        }
        else
        {
            Scierror(999, _("Wrong size for input argument: %d or %d expected.\n"), 4, 5);
            return SET_PROPERTY_ERROR;
        }

        if (tlist == NULL)
        {
            return SET_PROPERTY_ERROR;
        }

        status = set3ddata(_pvCtx, iObjUID, tlist);
        destroyAssignedList(tlist);
        return status;

    }
    else if (type == __GO_MATPLOT__)
    {
        int datatype = 0;
        int * piDataType = &datatype;
        int imagetype = 0;
        int * piImageType = &imagetype;
        int dataorder = 0;
        int * piDataOrder = &dataorder;
        int plottype = -1;
        int gridSize[4];
        int numX = 0;
        int *piNumX = &numX;
        int numY = 0;
        int *piNumY = &numY;

        bool bRGB = false;

        getGraphicObjectProperty(iObjUID, __GO_DATA_MODEL_NUM_X__, jni_int, (void **)&piNumX);
        getGraphicObjectProperty(iObjUID, __GO_DATA_MODEL_NUM_Y__, jni_int, (void **)&piNumY);
        getGraphicObjectProperty(iObjUID, __GO_DATA_MODEL_MATPLOT_DATA_TYPE__, jni_int, (void **)&piDataType);
        getGraphicObjectProperty(iObjUID, __GO_DATA_MODEL_MATPLOT_IMAGE_TYPE__, jni_int, (void **)&piImageType);
        getGraphicObjectProperty(iObjUID, __GO_DATA_MODEL_MATPLOT_DATA_ORDER__, jni_int, (void **)&piDataOrder);

        gridSize[1] = 1;
        gridSize[3] = 1;

        types::InternalType* pIT = (types::InternalType*)_pvData;
        if (pIT->isGenericType() == false)
        {
            Scierror(202, _("%s: Wrong type for input argument #%d: A real or integer expected.\n"), "data", 1);
            return 1;
        }

        types::GenericType* pGT = pIT->getAs<types::GenericType>();
        int iDims = pGT->getDims();
        int* piDimsArray = pGT->getDimsArray();

        int iRows = piDimsArray[0];
        int iCols = piDimsArray[1];

        void* pvData = NULL;
        if (iDims != 2 && (iDims != 3 || (piDimsArray[2] != 1 && piDimsArray[2] != 3 && piDimsArray[2] != 4)))
        {
            Scierror(202, _("%s: Wrong type for input argument #%d: A real or integer expected.\n"), "data", 1);
            return 1;
        }

        switch (pGT->getType())
        {
            case types::InternalType::ScilabInt8:
            {
                if (iDims == 2 && (DataType)datatype != MATPLOT_Char)
                {
                    plottype = buildMatplotType(MATPLOT_Char, (DataOrder)dataorder, (ImageType)imagetype);
                    bRGB = true;
                }
                else if (piDimsArray[2] != 1 && (DataType)datatype != MATPLOT_HM1_Char)
                {
                    plottype = buildMatplotType(MATPLOT_HM1_Char, (DataOrder)dataorder, (ImageType)imagetype);
                }
                else if (piDimsArray[2] != 3 && (DataType)datatype != MATPLOT_HM3_Char)
                {
                    plottype = buildMatplotType(MATPLOT_HM3_Char, (DataOrder)dataorder, (ImageType)imagetype);
                }
                else if (piDimsArray[2] != 4 && (DataType)datatype != MATPLOT_HM4_Char)
                {
                    plottype = buildMatplotType(MATPLOT_HM4_Char, (DataOrder)dataorder, (ImageType)imagetype);
                }

                pvData = (void*)pGT->getAs<types::Int8>()->get();
                break;
            }
            case types::InternalType::ScilabUInt8:
            {
                if (iDims == 2 && (DataType)datatype != MATPLOT_UChar)
                {
                    plottype = buildMatplotType(MATPLOT_UChar, (DataOrder)dataorder, (ImageType)imagetype);
                    bRGB = true;
                }
                else if ((DataType)datatype != MATPLOT_HM1_Char)
                {
                    plottype = buildMatplotType(MATPLOT_HM1_UChar, (DataOrder)dataorder, (ImageType)imagetype);
                }
                else if (piDimsArray[2] != 3 && (DataType)datatype != MATPLOT_HM3_UChar)
                {
                    plottype = buildMatplotType(MATPLOT_HM3_UChar, (DataOrder)dataorder, (ImageType)imagetype);
                }
                else if (piDimsArray[2] != 4 && (DataType)datatype != MATPLOT_HM4_UChar)
                {
                    plottype = buildMatplotType(MATPLOT_HM4_UChar, (DataOrder)dataorder, (ImageType)imagetype);
                }

                pvData = (void*)pGT->getAs<types::UInt8>()->get();
                break;
            }
            case types::InternalType::ScilabInt16:
            {
                if (iDims == 2 && (DataType)datatype != MATPLOT_Short)
                {
                    plottype = buildMatplotType(MATPLOT_Short, (DataOrder)dataorder, (ImageType)imagetype);
                }
                else
                {
                    Scierror(202, _("%s: Wrong type for input argument #%d: A real or integer expected.\n"), "data", 1);
                    return 1;
                }
                break;
            }
            case types::InternalType::ScilabUInt16:
            {
                if (iDims == 2 && (DataType)datatype != MATPLOT_UShort)
                {
                    plottype = buildMatplotType(MATPLOT_UShort, (DataOrder)dataorder, (ImageType)imagetype);
                }
                else
                {
                    Scierror(202, _("%s: Wrong type for input argument #%d: A real or integer expected.\n"), "data", 1);
                    return 1;
                }
                break;
            }
            case types::InternalType::ScilabInt32:
            {
                if (iDims == 2 && (DataType)datatype != MATPLOT_Int)
                {
                    plottype = buildMatplotType(MATPLOT_Int, (DataOrder)dataorder, (ImageType)imagetype);
                }
                else
                {
                    Scierror(202, _("%s: Wrong type for input argument #%d: A real or integer expected.\n"), "data", 1);
                    return 1;
                }
                break;
            }
            case types::InternalType::ScilabUInt32:
            {
                if (iDims == 2 && (DataType)datatype != MATPLOT_UInt)
                {
                    plottype = buildMatplotType(MATPLOT_UInt, (DataOrder)dataorder, (ImageType)imagetype);
                }
                else
                {
                    Scierror(202, _("%s: Wrong type for input argument #%d: A real or integer expected.\n"), "data", 1);
                    return 1;
                }
                break;
            }
            case types::InternalType::ScilabDouble:
            {
                if (iDims == 2 && (DataType)datatype != MATPLOT_Double)
                {
                    plottype = buildMatplotType(MATPLOT_Double, (DataOrder)dataorder, (ImageType)imagetype);
                    iRows = nbRow;
                    iCols = nbCol;
                    pvData = _pvData;
                    break;
                }
                else if ((DataType)datatype != MATPLOT_HM1_Double)
                {
                    plottype = buildMatplotType(MATPLOT_HM1_Double, (DataOrder)dataorder, (ImageType)imagetype);
                }
                else if (piDimsArray[2] != 3 && (DataType)datatype != MATPLOT_HM3_Double)
                {
                    plottype = buildMatplotType(MATPLOT_HM3_Double, (DataOrder)dataorder, (ImageType)imagetype);
                }
                else if (piDimsArray[2] != 4 && (DataType)datatype != MATPLOT_HM4_Double)
                {
                    plottype = buildMatplotType(MATPLOT_HM4_Double, (DataOrder)dataorder, (ImageType)imagetype);
                }

                pvData = (void*)pGT->getAs<types::Double>()->get();
                break;
            }
            default:
            {
                Scierror(202, _("%s: Wrong type for input argument #%d: A real or integer expected.\n"), "data", 1);
                return 1;
            }
        }

        if (bRGB)
        {
            if ((ImageType)imagetype == MATPLOT_RGB)
            {
                const int mb3 = iRows / 3;
                if (mb3 * 3 == iRows)
                {
                    iRows = mb3;
                }
                else
                {
                    Scierror(202, _("%s: Wrong size for input argument #%d: The number of rows must be a multiple of 3.\n"), "data", 1);
                    return 1;
                }
            }
            else if ((ImageType)imagetype == MATPLOT_RGBA)
            {
                const int mb4 = iRows / 4;
                if (mb4 * 4 == iRows)
                {
                    iRows = mb4;
                }
                else
                {
                    Scierror(202, _("%s: Wrong size for input argument #%d: The number of rows must be a multiple of 4.\n"), "data", 1);
                    return 1;
                }
            }
        }

        if (numX != iCols + 1 || numY != iRows + 1)
        {
            gridSize[0] = iCols + 1;
            gridSize[2] = iRows + 1;
            setGraphicObjectPropertyAndNoWarn(iObjUID, __GO_DATA_MODEL_GRID_SIZE__, gridSize, jni_int_vector, 4);
        }

        if (plottype != -1)
        {
            setGraphicObjectPropertyAndNoWarn(iObjUID, __GO_DATA_MODEL_MATPLOT_DATA_INFOS__, &plottype, jni_int, 1);
        }

        setGraphicObjectProperty(iObjUID, __GO_DATA_MODEL_MATPLOT_IMAGE_DATA__, pvData, jni_double_vector, iRows * iCols);
    }
    else  /* F.Leray 02.05.05 : "data" case for others (using sciGetPoint routine inside GetProperty.c) */
    {
        if (valueType != sci_matrix)
        {
            Scierror(999, _("Incompatible type for property ''%s''.\n"), "data");
            return SET_PROPERTY_ERROR;
        }

        return sciSetPoint(iObjUID, (double*)_pvData, &nbRow, &nbCol);
    }
    return SET_PROPERTY_ERROR;
}
/*------------------------------------------------------------------------*/
