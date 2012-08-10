/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2012 - Scilab Enterprises - Antoine ELIAS
*
*  This file must be used under the terms of the CeCILL.
*  This source file is licensed as described in the file COPYING, which
*  you should have received as part of this distribution.  The terms
*  are also available at
*  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

#define H5_USE_16_API

#ifndef _MSC_VER
#include <sys/time.h>
#else
#include <windows.h>
//#include <winbase.h>
#endif

#include <string.h>
#include <hdf5.h>
#include <stdlib.h>
#include "MALLOC.h"
#include "sci_types.h"
#include "stack3.h"
#include "h5_attributeConstants.h"
#include "h5_readDataFromFile.h"
#include "h5_readDataFromFile_v1.h"

int readDouble_v1(int _iDatasetId, int _iRows, int _iCols, double *_pdblData)
{
    herr_t status;

    /*
     * Read the data.
     */
    status = H5Dread(_iDatasetId, H5T_NATIVE_DOUBLE, H5S_ALL, H5S_ALL, H5P_DEFAULT, _pdblData);
    if (status < 0)
    {
        return -1;
    }

    status = H5Dclose(_iDatasetId);
    if (status < 0)
    {
        return -1;
    }

    return 0;
}


int readDoubleMatrix_v1(int _iDatasetId, int _iRows, int _iCols, double *_pdblData)
{
    herr_t status;

    if (_iRows != 0 && _iCols != 0)
    {
        hid_t obj;
        hobj_ref_t Ref;

        //Read the data.
        status = H5Dread(_iDatasetId, H5T_STD_REF_OBJ, H5S_ALL, H5S_ALL, H5P_DEFAULT, &Ref);
        if (status < 0)
        {
            return -1;
        }

        //Open the referenced object, get its name and type.
        obj = H5Rdereference(_iDatasetId, H5R_OBJECT, &Ref);
        readDouble_v1(obj, _iRows, _iCols, _pdblData);
    }

    status = H5Dclose(_iDatasetId);
    if (status < 0)
    {
        return -1;
    }

    return 0;
}

int readDoubleComplexMatrix_v1(int _iDatasetId, int _iRows, int _iCols, double *_pdblReal, double *_pdblImg)
{
    hid_t obj;
    herr_t status;
    hobj_ref_t pRef[2] = {0};

    //Read the data.
    status = H5Dread(_iDatasetId, H5T_STD_REF_OBJ, H5S_ALL, H5S_ALL, H5P_DEFAULT, pRef);
    if (status < 0)
    {
        return -1;
    }

    //Open the referenced object, get its name and type.
    obj = H5Rdereference(_iDatasetId, H5R_OBJECT, &pRef[0]);
    status = readDouble_v1(obj, _iRows, _iCols, _pdblReal);
    if (status < 0)
    {
        return -1;
    }

    obj = H5Rdereference(_iDatasetId, H5R_OBJECT, &pRef[1]);
    status = readDouble_v1(obj, _iRows, _iCols, _pdblImg);
    if (status < 0)
    {
        return -1;
    }

    status = H5Dclose(_iDatasetId);
    if (status < 0)
    {
        return -1;
    }

    return 0;
}
