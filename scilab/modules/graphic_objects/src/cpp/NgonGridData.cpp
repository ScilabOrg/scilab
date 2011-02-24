/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2010 - DIGITEO - Manuel Juliachs
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "NgonGridData.hxx"

extern "C" {
    #include <string.h>
    #include <stdlib.h>

    #include "graphicObjectProperties.h"
}

NgonGridData::NgonGridData(void)
{
    numGons = 0;

    /* Grid: must be set to 4 */
    numVerticesPerGon = 4;

    xCoordinates = NULL;
    yCoordinates = NULL;
    zCoordinates = NULL;

    xSize = 0;
    ySize = 0;

    xDimensions[0] = 0;
    xDimensions[1] = 0;

    yDimensions[0] = 0;
    yDimensions[1] = 0;
}

NgonGridData::~NgonGridData(void)
{
    if (xSize > 0)
    {
        delete [] xCoordinates;
    }

    if (ySize > 0)
    {
        delete [] yCoordinates;
    }

    if (xSize > 0 && ySize > 0)
    {
        delete [] zCoordinates;
    }
}

int NgonGridData::getPropertyFromName(char* propertyName)
{
    if (strcmp(propertyName, __GO_DATA_MODEL_NUM_X__) == 0)
    {
        return NUM_X;
    }
    else if (strcmp(propertyName, __GO_DATA_MODEL_NUM_Y__) == 0)
    {
        return NUM_Y;
    }
    else if (strcmp(propertyName, __GO_DATA_MODEL_NUM_Z__) == 0)
    {
        return NUM_Z;
    }
    else if (strcmp(propertyName, __GO_DATA_MODEL_X_DIMENSIONS__) == 0)
    {
        return X_DIMENSIONS;
    }
    else if (strcmp(propertyName, __GO_DATA_MODEL_Y_DIMENSIONS__) == 0)
    {
        return Y_DIMENSIONS;
    }
    else if (strcmp(propertyName, __GO_DATA_MODEL_GRID_SIZE__) == 0)
    {
        return GRID_SIZE;
    }
    else if (strcmp(propertyName, __GO_DATA_MODEL_X__) == 0)
    {
        return X_COORDINATES;
    }
    else if (strcmp(propertyName, __GO_DATA_MODEL_Y__) == 0)
    {
        return Y_COORDINATES;
    }
    else if (strcmp(propertyName, __GO_DATA_MODEL_Z__) == 0)
    {
        return Z_COORDINATES;
    }
    else
    {
        return NgonData::getPropertyFromName(propertyName);
    }

}

int NgonGridData::setDataProperty(int property, void* value, int numElements)
{
    if (property == GRID_SIZE)
    {
        return setGridSize((int*) value);
    }
    else if (property == X_COORDINATES)
    {
        setDataX((double*) value, numElements);
    }
    else if (property == Y_COORDINATES)
    {
        setDataY((double*) value, numElements);
    }
    else if (property == Z_COORDINATES)
    {
        setDataZ((double*) value, numElements);
    }
    else
    {
        return NgonData::setDataProperty(property, value, numElements);
    }

    return 1;
}

void NgonGridData::getDataProperty(int property, void **_pvData)
{
    if (property == NUM_X)
    {
        ((int *) *_pvData)[0] = getNumX();
    }
    else if (property == NUM_Y)
    {
        ((int *) *_pvData)[0] = getNumY();
    }
    else if (property == NUM_Z)
    {
        ((int *) *_pvData)[0] = getNumZ();
    }
    else if (property == X_DIMENSIONS)
    {
        *_pvData = getXDimensions();
    }
    else if (property == Y_DIMENSIONS)
    {
        *_pvData = getYDimensions();
    }
    else if (property == X_COORDINATES)
    {
        *_pvData = getDataX();
    }
    else if (property == Y_COORDINATES)
    {
        *_pvData = getDataY();
    }
    else if (property == Z_COORDINATES)
    {
        *_pvData = getDataZ();
    }
    else
    {
        NgonData::getDataProperty(property, _pvData);
    }

}

int NgonGridData::setGridSize(int* gridSize)
{
    int newXSize;
    int newYSize;
    int xModified;
    int yModified;
    int zModified;
    int result;

    double* newXCoordinates = NULL;
    double* newYCoordinates = NULL;
    double* newZCoordinates = NULL;

    result = 1;

    xModified = 0;
    yModified = 0;
    zModified = 0;

    if ((gridSize[0] != 1) && (gridSize[1] != 1))
    {
        return 0;
    }

    if ((gridSize[2] != 1) && (gridSize[3] != 1))
    {
        return 0;
    }

    newXSize = gridSize[0]*gridSize[1];
    newYSize = gridSize[2]*gridSize[3];


    if (newXSize != xSize)
    {
        xModified = 1;

        try
        {
            newXCoordinates = new double[newXSize];
        }
        catch (const std::exception& e)
        {
            result = 0;
        }
    }

    if (newYSize != ySize)
    {
        yModified = 1;

        try
        {
            newYCoordinates = new double[newYSize];
        }
        catch (const std::exception& e)
        {
            result = 0;
        }
    }

    if (newXSize*newYSize != xSize*ySize)
    {
        zModified = 1;

        try
        {
            newZCoordinates = new double[newXSize*newYSize];
        }
        catch (const std::exception& e)
        {
            result = 0;
        }
    }

    if (result)
    {
        if (xModified)
        {
            if (xSize > 0)
            {
                delete [] xCoordinates;
            }

            xCoordinates = newXCoordinates;
            xSize = newXSize;
        }

        xDimensions[0] = gridSize[0];
        xDimensions[1] = gridSize[1];

        if (yModified)
        {
            if (ySize > 0)
            {
                delete [] yCoordinates;
            }

            yCoordinates = newYCoordinates;
            ySize = newYSize;
        }

        yDimensions[0] = gridSize[2];
        yDimensions[1] = gridSize[3];

        if (zModified)
        {
            if (xSize*ySize > 0)
            {
                delete [] zCoordinates;
            }

            zCoordinates = newZCoordinates;

            numGons = (xSize-1)*(ySize-1);
        }

    }
    else
    {
        /* Failed allocation(s) */

        if (xModified && (newXCoordinates != NULL))
        {
            delete [] newXCoordinates;
        }

        if (yModified && (newYCoordinates != NULL))
        {
            delete [] newYCoordinates;
        }

        if (zModified && (newZCoordinates != NULL))
        {
            delete [] newZCoordinates;
        }

    }

    return result;
}

int NgonGridData::getNumX(void)
{
    return xSize;
}

int NgonGridData::getNumY(void)
{
    return ySize;
}

int NgonGridData::getNumZ(void)
{
    return xSize*ySize;
}

int* NgonGridData::getXDimensions(void)
{
    return xDimensions;
}

int* NgonGridData::getYDimensions(void)
{
    return yDimensions;
}

void NgonGridData::setDataX(double* data, int numElements)
{
    if (numElements > xSize)
    {
        return;
    }

    for (int i = 0; i < numElements; i++)
    {
        xCoordinates[i] = data[i];
    }
}

void NgonGridData::setDataY(double* data, int numElements)
{
    if (numElements > ySize)
    {
        return;
    }

    for (int i = 0; i < numElements; i++)
    {
        yCoordinates[i] = data[i];
    }
}

void NgonGridData::setDataZ(double* data, int numElements)
{
    if (numElements > xSize*ySize)
    {
        return;
    }

    for (int i = 0; i < numElements; i++)
    {
        zCoordinates[i] = data[i];
    }
}

double* NgonGridData::getDataX(void)
{
    return xCoordinates;
}

double* NgonGridData::getDataY(void)
{
    return yCoordinates;
}

double* NgonGridData::getDataZ(void)
{
    return zCoordinates;
}
