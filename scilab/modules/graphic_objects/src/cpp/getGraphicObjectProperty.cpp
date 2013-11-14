/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2010-2010 - DIGITEO - Bruno JOFRET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

extern "C"
{
#include <stdlib.h>
#include <string.h>
#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"
#include "getScilabJavaVM.h"
#include <stdio.h>
}

#include "CallGraphicController.hxx"
#include "DataController.hxx"

using namespace org_scilab_modules_graphic_objects;

void getGraphicObjectProperty(int iUID, int _iName, _ReturnType_ _returnType, void **_pvData)
{
    // do not perform anything if the id is undefined
    if (iUID == 0)
    {
        return;
    }

    /* All the Data model properties have the DATA_MODEL prefix */
    if (_iName == __GO_DATA_MODEL__
            || _iName == __GO_DATA_MODEL_COORDINATES__
            || _iName == __GO_DATA_MODEL_X__
            || _iName == __GO_DATA_MODEL_Y__
            || _iName == __GO_DATA_MODEL_Z__
            || _iName == __GO_DATA_MODEL_X_COORDINATES_SHIFT__
            || _iName == __GO_DATA_MODEL_Y_COORDINATES_SHIFT__
            || _iName == __GO_DATA_MODEL_Z_COORDINATES_SHIFT__
            || _iName == __GO_DATA_MODEL_X_COORDINATES_SHIFT_SET__
            || _iName == __GO_DATA_MODEL_Y_COORDINATES_SHIFT_SET__
            || _iName == __GO_DATA_MODEL_Z_COORDINATES_SHIFT_SET__
            || _iName == __GO_DATA_MODEL_NUM_ELEMENTS__
            || _iName == __GO_DATA_MODEL_NUM_ELEMENTS_ARRAY__
            || _iName == __GO_DATA_MODEL_NUM_VERTICES_PER_GON__
            || _iName == __GO_DATA_MODEL_NUM_GONS__
            || _iName == __GO_DATA_MODEL_Z_COORDINATES_SET__
            || _iName == __GO_DATA_MODEL_COLORS__
            || _iName == __GO_DATA_MODEL_NUM_COLORS__
            || _iName == __GO_DATA_MODEL_NUM_VERTICES__
            || _iName == __GO_DATA_MODEL_NUM_INDICES__
            || _iName == __GO_DATA_MODEL_INDICES__
            || _iName == __GO_DATA_MODEL_VALUES__
            || _iName == __GO_DATA_MODEL_FEC_TRIANGLES__
            || _iName == __GO_DATA_MODEL_NUM_X__
            || _iName == __GO_DATA_MODEL_NUM_Y__
            || _iName == __GO_DATA_MODEL_NUM_Z__
            || _iName == __GO_DATA_MODEL_GRID_SIZE__
            || _iName == __GO_DATA_MODEL_X_DIMENSIONS__
            || _iName == __GO_DATA_MODEL_Y_DIMENSIONS__
            || _iName == __GO_DATA_MODEL_MATPLOT_BOUNDS__
            || _iName == __GO_DATA_MODEL_MATPLOT_TYPE__
            || _iName == __GO_DATA_MODEL_MATPLOT_DATA_INFOS__
            || _iName == __GO_DATA_MODEL_MATPLOT_DATA_TYPE__
            || _iName == __GO_DATA_MODEL_MATPLOT_DATA_ORDER__
            || _iName == __GO_DATA_MODEL_MATPLOT_GL_TYPE__
            || _iName == __GO_DATA_MODEL_MATPLOT_IMAGE_TYPE__
            || _iName == __GO_DATA_MODEL_MATPLOT_IMAGE_DATA__
            || _iName == __GO_DATA_MODEL_MATPLOT_IMAGE_DATASIZE__
       )
    {
        DataController::getGraphicObjectProperty(iUID, _iName, _pvData);
        return;
    }

    try
    {
        switch (_returnType)
        {
            case jni_string:
            {
                *(_pvData) = CallGraphicController::getGraphicObjectPropertyAsString(getScilabJavaVM(), iUID, _iName);
                return;
            }
            case jni_string_vector:
            {
                *_pvData = CallGraphicController::getGraphicObjectPropertyAsStringVector(getScilabJavaVM(), iUID, _iName);
                return;
            }
            case jni_double:
            {
                double * ret = CallGraphicController::getGraphicObjectPropertyAsDoubleVector(getScilabJavaVM(), iUID, _iName);
                if (ret == NULL)
                {
                    *_pvData = NULL;
                }
                else
                {
                    ((double *)*_pvData)[0] = *ret;
                    delete[] ret;
                }

                return;
            }
            case jni_double_vector:
            {
                *_pvData = CallGraphicController::getGraphicObjectPropertyAsDoubleVector(getScilabJavaVM(), iUID, _iName);
                return;
            }
            case jni_bool:
            {
                int * ret = CallGraphicController::getGraphicObjectPropertyAsBooleanVector(getScilabJavaVM(), iUID, _iName);
                if (ret == NULL)
                {
                    *_pvData = NULL;
                }
                else
                {
                    ((int *)*_pvData)[0] = *ret;
                    delete[] ret;
                }

                return;
            }
            case jni_bool_vector:
            {
                *_pvData = CallGraphicController::getGraphicObjectPropertyAsBooleanVector(getScilabJavaVM(), iUID, _iName);
                return;
            }
            case jni_int:
            {
                int * ret = CallGraphicController::getGraphicObjectPropertyAsIntegerVector(getScilabJavaVM(), iUID, _iName);
                if (ret == NULL)
                {
                    *_pvData = NULL;
                }
                else
                {
                    ((int *)*_pvData)[0] = *ret;
                    delete[] ret;
                }

                return;
            }
            case jni_int_vector:
            {
                *_pvData = CallGraphicController::getGraphicObjectPropertyAsIntegerVector(getScilabJavaVM(), iUID, _iName);
                return;
            }
            default:
                *_pvData = NULL;
                return;
        }
    }
    catch (std::exception &e)
    {
        e.what();
        // If we have an exception, return null to scilab to manage error.
        *_pvData = NULL;
        return;
    }

}


void releaseGraphicObjectProperty(int _iName, void * _pvData, enum _ReturnType_ _returnType, int numElements)
{

    /* All the Data model properties have the DATA_MODEL prefix */
    if (_iName == __GO_DATA_MODEL__
            || _iName == __GO_DATA_MODEL_COORDINATES__
            || _iName == __GO_DATA_MODEL_X__
            || _iName == __GO_DATA_MODEL_Y__
            || _iName == __GO_DATA_MODEL_Z__
            || _iName == __GO_DATA_MODEL_X_COORDINATES_SHIFT__
            || _iName == __GO_DATA_MODEL_Y_COORDINATES_SHIFT__
            || _iName == __GO_DATA_MODEL_Z_COORDINATES_SHIFT__
            || _iName == __GO_DATA_MODEL_X_COORDINATES_SHIFT_SET__
            || _iName == __GO_DATA_MODEL_Y_COORDINATES_SHIFT_SET__
            || _iName == __GO_DATA_MODEL_Z_COORDINATES_SHIFT_SET__
            || _iName == __GO_DATA_MODEL_NUM_ELEMENTS__
            || _iName == __GO_DATA_MODEL_NUM_ELEMENTS_ARRAY__
            || _iName == __GO_DATA_MODEL_NUM_VERTICES_PER_GON__
            || _iName == __GO_DATA_MODEL_NUM_GONS__
            || _iName == __GO_DATA_MODEL_Z_COORDINATES_SET__
            || _iName == __GO_DATA_MODEL_COLORS__
            || _iName == __GO_DATA_MODEL_NUM_COLORS__
            || _iName == __GO_DATA_MODEL_NUM_VERTICES__
            || _iName == __GO_DATA_MODEL_NUM_INDICES__
            || _iName == __GO_DATA_MODEL_INDICES__
            || _iName == __GO_DATA_MODEL_VALUES__
            || _iName == __GO_DATA_MODEL_FEC_TRIANGLES__
            || _iName == __GO_DATA_MODEL_NUM_X__
            || _iName == __GO_DATA_MODEL_NUM_Y__
            || _iName == __GO_DATA_MODEL_NUM_Z__
            || _iName == __GO_DATA_MODEL_GRID_SIZE__
            || _iName == __GO_DATA_MODEL_X_DIMENSIONS__
            || _iName == __GO_DATA_MODEL_Y_DIMENSIONS__
            || _iName == __GO_DATA_MODEL_MATPLOT_BOUNDS__
            || _iName == __GO_DATA_MODEL_MATPLOT_TYPE__
            || _iName == __GO_DATA_MODEL_MATPLOT_DATA_INFOS__
            || _iName == __GO_DATA_MODEL_MATPLOT_DATA_TYPE__
            || _iName == __GO_DATA_MODEL_MATPLOT_DATA_ORDER__
            || _iName == __GO_DATA_MODEL_MATPLOT_GL_TYPE__
            || _iName == __GO_DATA_MODEL_MATPLOT_IMAGE_TYPE__
            || _iName == __GO_DATA_MODEL_MATPLOT_IMAGE_DATA__
            || _iName == __GO_DATA_MODEL_MATPLOT_IMAGE_DATASIZE__
       )
    {
        // passed by reference, do not free them
        return;
    }

    switch (_returnType)
    {
        case jni_string:
        {
            delete[] (char*) _pvData;
            return;
        }
        case jni_string_vector:
        {
            char** data = (char**) _pvData;
            for (int i = 0; i < numElements; ++i)
            {
                delete[] data[i];
            }
            delete[] data;
            return;
        }
        case jni_double:
        {
            // passed by value
            return;
        }
        case jni_double_vector:
        {
            delete[] (double*) _pvData;
            return;
        }
        case jni_bool:
        {
            // passed by value
            return;
        }
        case jni_bool_vector:
        {
            delete[] (int*) _pvData;
            return;
        }
        case jni_int:
        {
            // passed by value
            return;
        }
        case jni_int_vector:
        {
            delete[] (int*) _pvData;
            return;
        }
        default:
            return;
    }

}

int getProperty(int _iId, char* _pstPropName, int _iStackPos)
{
    return CallGraphicController::getProperty(getScilabJavaVM(), _iId, _pstPropName, _iStackPos);
}

