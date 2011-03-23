/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2011 - DIGITEO - Manuel Juliachs
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "ColorComputer.hxx"
#include "DecompositionUtils.hxx"
#include "Plot3DDecomposer.hxx"

extern "C"
{
#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"
}

Plot3DDecomposer* Plot3DDecomposer::decomposer = NULL;

void Plot3DDecomposer::fillVertices(char* id, float* buffer, int bufferLength, int elementsSize, int coordinateMask, double* scale, double* translation, int logMask)
{
    double* x;
    double* y;
    double* z;

    int numX = 0;
    int* piNumX = &numX;
    int numY = 0;
    int* piNumY = &numY;

    Plot3DDecomposer* decomposer = get();

    getGraphicObjectProperty(id, __GO_DATA_MODEL_X__, jni_double_vector, (void**) &x);
    getGraphicObjectProperty(id, __GO_DATA_MODEL_Y__, jni_double_vector, (void**) &y);
    getGraphicObjectProperty(id, __GO_DATA_MODEL_Z__, jni_double_vector, (void**) &z);

    getGraphicObjectProperty(id, __GO_DATA_MODEL_NUM_X__, jni_int, (void**) &piNumX);
    getGraphicObjectProperty(id, __GO_DATA_MODEL_NUM_Y__, jni_int, (void**) &piNumY);

    decomposer->fillGridVertices(buffer, bufferLength, elementsSize, coordinateMask, scale, translation, logMask, x, y, z, numX, numY);
}

double Plot3DDecomposer::getZCoordinate(double* z, int numX, int numY, int i, int j)
{
    double zij;

    zij = z[numX*j+i];

    return zij;
}

double Plot3DDecomposer::getZCoordinate(double* z, int numX, int numY, int i, int j, int logUsed)
{
    double zij;

    zij = z[numX*j+i];

    if (logUsed)
    {
        zij = DecompositionUtils::getLog10Value(zij);
    }

    return zij;
}

/*
 * To do: compute and return color indices instead of directly looking
 * up colors from the colormap.
 */
void Plot3DDecomposer::fillColors(char* id, float* buffer, int bufferLength, int elementsSize)
{
    char* parentFigure;
    char* parent;

    double* z;
    double* colormap;

    double zMin;
    double zMax;
    double zRange;
    double minDoubleValue;

    int numX = 0;
    int* piNumX = &numX;
    int numY = 0;
    int* piNumY = &numY;
    int colormapSize = 0;
    int* piColormapSize = &colormapSize;

    int i;
    int j;

    int bufferOffset = 0;

    getGraphicObjectProperty(id, __GO_DATA_MODEL_NUM_X__, jni_int, (void**) &piNumX);
    getGraphicObjectProperty(id, __GO_DATA_MODEL_NUM_Y__, jni_int, (void**) &piNumY);

    getGraphicObjectProperty(id, __GO_DATA_MODEL_Z__, jni_double_vector, (void**) &z);

    getGraphicObjectProperty(id, __GO_TYPE__, jni_string, (void**) &parent);
    getGraphicObjectProperty(id, __GO_PARENT__, jni_string, (void**) &parent);

    /* Temporary: to avoid getting a null parent_figure property when the object is built */
    if (strcmp(parent, "") == 0)
    {
        return;
    }

    getGraphicObjectProperty(id, __GO_PARENT_FIGURE__, jni_string, (void**) &parentFigure);

    if (parentFigure == NULL)
    {
        return;
    }

    getGraphicObjectProperty(parentFigure, __GO_COLORMAP__, jni_double_vector, (void**) &colormap);
    getGraphicObjectProperty(parentFigure, __GO_COLORMAP_SIZE__, jni_int, (void**) &piColormapSize);

    computeMinMaxZValues(z, numX, numY, &zMin, &zMax);

    minDoubleValue = DecompositionUtils::getMinDoubleValue();

    /* To be verified */
    if ((zMax - zMin) < minDoubleValue)
    {
        zRange = 1.0;
    }
    else
    {
        zRange = zMax - zMin;
    }

    for (j = 0; j < numY; j++)
    {
        for (i = 0; i < numX; i++)
        {
            int currentPointIndex = getPointIndex(numX, numY, i, j);

            ColorComputer::getColor(z[currentPointIndex], zMin, zRange, colormap, colormapSize, &buffer[bufferOffset]);

            if (elementsSize == 4)
            {
                buffer[bufferOffset +3] = 1.0;
            }

            bufferOffset += elementsSize;
        }
    }

}

int Plot3DDecomposer::fillIndices(char* id, int* buffer, int bufferLength, int logMask)
{
    double* x;
    double* y;
    double* z;

    int numX = 0;
    int* piNumX = &numX;
    int numY = 0;
    int* piNumY = &numY;

    int numberIndices;

    Plot3DDecomposer* decomposer = get();

    getGraphicObjectProperty(id, __GO_DATA_MODEL_NUM_X__, jni_int, (void**) &piNumX);
    getGraphicObjectProperty(id, __GO_DATA_MODEL_NUM_Y__, jni_int, (void**) &piNumY);

    /* 0 indices if 0 points */
    if (numX == 0 || numY == 0)
    {
        return 0;
    }

    getGraphicObjectProperty(id, __GO_DATA_MODEL_X__, jni_double_vector, (void**) &x);
    getGraphicObjectProperty(id, __GO_DATA_MODEL_Y__, jni_double_vector, (void**) &y);
    getGraphicObjectProperty(id, __GO_DATA_MODEL_Z__, jni_double_vector, (void**) &z);

    numberIndices = decomposer->fillTriangleIndices(buffer, bufferLength, logMask, x, y, z, numX, numY);

    return numberIndices;
}

int Plot3DDecomposer::getWireIndicesSize(char* id)
{
    int numX = 0;
    int* piNumX = &numX;
    int numY = 0;
    int* piNumY = &numY;

    getGraphicObjectProperty(id, __GO_DATA_MODEL_NUM_X__, jni_int, (void**) &piNumX);
    getGraphicObjectProperty(id, __GO_DATA_MODEL_NUM_Y__, jni_int, (void**) &piNumY);

    if (numX == 0 || numY == 0)
    {
        return 0;
    }

    return 2*2*(numX-1)*(numY-1) + 2*(numX-1) + 2*(numY-1);
}

/*
 * To be optimized:
 * -a lot of work performed redundantly with NgonGridDataDecomposer::fillIndices, ought to be merged
 *  with it.
 * -edges shared by two adjacent faces whose opposite edges have invalid x or y coordinate values
 *  are drawn, though they shouldn't be.
 */
int Plot3DDecomposer::fillWireIndices(char* id, int* buffer, int bufferLength, int logMask)
{
    double* x;
    double* y;
    double* z;

    int numX = 0;
    int* piNumX = &numX;
    int numY = 0;
    int* piNumY = &numY;
    int colorMode = 0;
    int* piColorMode = &colorMode;

    int i;
    int j;

    int currentRowValid;
    int nextRowValid;

    int currentLineValid;
    int nextLineValid;

    int lowerLeftZValid;
    int lowerRightZValid;
    int upperLeftZValid;

    int ij;
    int ip1j;
    int ijp1;

    int bufferOffset = 0;

    getGraphicObjectProperty(id, __GO_DATA_MODEL_NUM_X__, jni_int, (void**) &piNumX);
    getGraphicObjectProperty(id, __GO_DATA_MODEL_NUM_Y__, jni_int, (void**) &piNumY);

    /* 0 indices if 0 points */
    if (numX == 0 || numY == 0)
    {
        return 0;
    }

    getGraphicObjectProperty(id, __GO_DATA_MODEL_X__, jni_double_vector, (void**) &x);
    getGraphicObjectProperty(id, __GO_DATA_MODEL_Y__, jni_double_vector, (void**) &y);
    getGraphicObjectProperty(id, __GO_DATA_MODEL_Z__, jni_double_vector, (void**) &z);

    /* First row */
    currentRowValid = DecompositionUtils::isValid(y[0]);

    if (logMask & 0x2)
    {
        currentRowValid &= DecompositionUtils::isLogValid(y[0]);
    }

    for (j = 0; j < numY-1; j++)
    {
        nextRowValid = DecompositionUtils::isValid(y[j+1]);

        if (logMask & 0x2)
        {
            nextRowValid &= DecompositionUtils::isLogValid(y[j+1]);
        }

        currentLineValid = DecompositionUtils::isValid(x[0]);

        if (logMask & 0x1)
        {
            currentLineValid &= DecompositionUtils::isLogValid(x[0]);
        }

        lowerLeftZValid = DecompositionUtils::isValid(z[j*numX]);

        if (logMask & 0x4)
        {
            lowerLeftZValid &= DecompositionUtils::isLogValid(z[j*numX]);
        }

        ij = getPointIndex(numX, numY, 0, j);

        for (i = 0; i < numX-1; i++)
        {
            ip1j = getPointIndex(numX, numY, i+1, j);
            ijp1 = getPointIndex(numX, numY, i, j+1);

            nextLineValid = DecompositionUtils::isValid(x[i+1]);

            if (logMask & 0x1)
            {
                nextLineValid &= DecompositionUtils::isLogValid(x[i+1]);
            }

            lowerRightZValid = DecompositionUtils::isValid(z[ip1j]);
            upperLeftZValid = DecompositionUtils::isValid(z[ijp1]);

            if (logMask & 0x4)
            {
                lowerRightZValid &= DecompositionUtils::isLogValid(z[ip1j]);
                upperLeftZValid &= DecompositionUtils::isLogValid(z[ijp1]);
            }

            /*
             * Two segments: from the lower-left to the lower-right corner
             * and from the lower-left to the upper-left corner.
             */
            if (currentRowValid && (currentLineValid && nextLineValid) && (lowerLeftZValid && lowerRightZValid))
            {
                buffer[bufferOffset] = ij;
                buffer[bufferOffset+1] = ip1j;

                bufferOffset += 2;
            }

            if (currentLineValid && (currentRowValid && nextRowValid) && (lowerLeftZValid && upperLeftZValid))
            {
                buffer[bufferOffset] = ij;
                buffer[bufferOffset+1] = ijp1;

                bufferOffset += 2;
            }

            currentLineValid = nextLineValid;
            lowerLeftZValid = lowerRightZValid;

            ij = ip1j;
        }

        /* Rightmost vertical line */
        if (currentLineValid && (currentRowValid && nextRowValid))
        {
            ijp1 = getPointIndex(numX, numY, numX-1, j+1);

            upperLeftZValid = DecompositionUtils::isValid(z[ijp1]);

            if (logMask & 0x4)
            {
                upperLeftZValid &= DecompositionUtils::isLogValid(z[ijp1]);
            }

            if (lowerLeftZValid && upperLeftZValid)
            {
                buffer[bufferOffset] = ij;
                buffer[bufferOffset+1] = ijp1;

                bufferOffset += 2;
            }
        }

        currentRowValid = nextRowValid;
    }

    /* Topmost horizontal lines */
    currentRowValid = DecompositionUtils::isValid(y[numY-1]);

    if (logMask & 0x2)
    {
        currentRowValid &= DecompositionUtils::isLogValid(y[numY-1]);
    }

    currentLineValid = DecompositionUtils::isValid(x[0]);

    if (logMask & 0x1)
    {
        currentLineValid = DecompositionUtils::isValid(x[0]);
    }

    ij = getPointIndex(numX, numY, 0, numY-1);

    lowerLeftZValid = DecompositionUtils::isValid(z[ij]);

    if (logMask & 0x4)
    {
        lowerLeftZValid &= DecompositionUtils::isValid(z[ij]);
    }

    ij = getPointIndex(numX, numY, 0, numY-1);

    for (i = 0; i < numX-1; i++)
    {
        nextLineValid = DecompositionUtils::isValid(x[i+1]);

        if (logMask & 0x1)
        {
            nextLineValid &= DecompositionUtils::isLogValid(x[i+1]);
        }

        ip1j = getPointIndex(numX, numY, i+1, numY-1);

        lowerRightZValid = DecompositionUtils::isValid(z[ip1j]);

        if (logMask & 0x4)
        {
            lowerRightZValid &= DecompositionUtils::isLogValid(z[ip1j]);
        }

        if (currentRowValid && (currentLineValid && nextLineValid) && (lowerLeftZValid && lowerRightZValid))
        {
            buffer[bufferOffset] = ij;
            buffer[bufferOffset+1] = ip1j;

            bufferOffset += 2;
        }

        currentLineValid = nextLineValid;
        lowerLeftZValid = lowerRightZValid;

        ij = ip1j;
    }

    return bufferOffset;
}

