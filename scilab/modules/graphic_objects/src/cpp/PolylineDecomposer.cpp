/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2010 - DIGITEO - Pierre Lando
 *  Copyright (C) 2011 - DIGITEO - Manuel Juliachs
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include <iostream>

#include "DecompositionUtils.hxx"
#include "PolylineDecomposer.hxx"

extern "C"
{
#include <math.h>

#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"
}

int PolylineDecomposer::getDataSize(char* id)
{
    int nPoints = 0;
    int *piNPoints = &nPoints;
    int polylineStyle = 0;
    int* piPolylineStyle = &polylineStyle;
    int closed = 0;
    int* piClosed = &closed;

    getGraphicObjectProperty(id, __GO_POLYLINE_STYLE__, jni_int, (void**) &piPolylineStyle);
    getGraphicObjectProperty(id, __GO_DATA_MODEL_NUM_ELEMENTS__, jni_int, (void**) &piNPoints);
    getGraphicObjectProperty(id, __GO_CLOSED__, jni_bool, (void**) &piClosed);

    /* If 0 points, 0 elements */
    if (nPoints == 0)
    {
        return 0;
    }

    /* Segments */
    if (polylineStyle == 1)
    {
        return nPoints;
    }
    /* Staircase */
    else if (polylineStyle == 2)
    {
        if (closed)
        {
            return 2*nPoints;
        }
        else
        {
            return (2*nPoints)-1;
        }
    }
    /* Vertical segments plus segments */
    else if (polylineStyle == 3)
    {
        return 2*nPoints;
    }
    /* Arrowed segments */
    else if (polylineStyle == 4)
    {
        return nPoints;
    }
    /* Vertical bars plus segments */
    else if (polylineStyle == 6)
    {
        return 5*nPoints;
    }
    /* To be done: remaining styles */
    else
    {
        return 0;
    }

}

void PolylineDecomposer::fillVertices(char* id, float* buffer, int bufferLength, int elementsSize, int coordinateMask, double* scale, double* translation, int logMask)
{
    double* t;
    double* xshift;
    double* yshift;
    double* zshift;

    int polylineStyle = 0;
    int* piPolylineStyle = &polylineStyle;

    int nPoints = 0;
    int *piNPoints = &nPoints;

    getGraphicObjectProperty(id, __GO_DATA_MODEL_COORDINATES__, jni_double_vector, (void**) &t);
    getGraphicObjectProperty(id, __GO_DATA_MODEL_NUM_ELEMENTS__, jni_int, (void**) &piNPoints);

    getGraphicObjectProperty(id, __GO_DATA_MODEL_X_COORDINATES_SHIFT__, jni_double_vector, (void**) &xshift);
    getGraphicObjectProperty(id, __GO_DATA_MODEL_Y_COORDINATES_SHIFT__, jni_double_vector, (void**) &yshift);
    getGraphicObjectProperty(id, __GO_DATA_MODEL_Z_COORDINATES_SHIFT__, jni_double_vector, (void**) &zshift);

    getGraphicObjectProperty(id, __GO_POLYLINE_STYLE__, jni_int, (void**) &piPolylineStyle);

    if (polylineStyle == 1)
    {
        fillSegmentsDecompositionVertices(id, buffer, bufferLength, elementsSize, coordinateMask, scale, translation, logMask, t, nPoints, xshift, yshift, zshift);
    }
    else if (polylineStyle == 2)
    {
        fillStairDecompositionVertices(id, buffer, bufferLength, elementsSize, coordinateMask, scale, translation, logMask, t, nPoints, xshift, yshift, zshift);
    }
    else if (polylineStyle == 3)
    {
        fillVerticalLinesDecompositionVertices(id, buffer, bufferLength, elementsSize, coordinateMask, scale, translation, logMask, t, nPoints, xshift, yshift, zshift);
    }
    else if (polylineStyle == 4)
    {
        fillSegmentsDecompositionVertices(id, buffer, bufferLength, elementsSize, coordinateMask, scale, translation, logMask, t, nPoints, xshift, yshift, zshift);
    }
    else if (polylineStyle == 6)
    {
        fillVerticalBarsDecompositionVertices(id, buffer, bufferLength, elementsSize, coordinateMask, scale, translation, logMask, t, nPoints, xshift, yshift, zshift);
    }

}

void PolylineDecomposer::fillSegmentsDecompositionVertices(char* id, float* buffer, int bufferLength, int elementsSize, int coordinateMask, double* scale, double* translation,
    int logMask, double* coordinates, int nPoints, double* xshift, double* yshift, double* zshift)
{
    double coordinate;

    int componentIndices[3];
    int i;

    // TODO Optimize ? (test if s = 1 and t = 0, coordinateMask = 0 ...)
    for (i = 0; i < nPoints; i++)
    {
        /* Offset of a polyline vertex */
        int v0;

        v0 = elementsSize*i;

        componentIndices[0] = i;
        componentIndices[1] = i;
        componentIndices[2] = i;

        getAndWriteVertexToBuffer(buffer, v0, coordinates, componentIndices, nPoints, elementsSize,
            xshift, yshift, zshift, coordinateMask, scale, translation, logMask);
    }

}

void PolylineDecomposer::getAndWriteVertexToBuffer(float* buffer, int offset, double* coordinates, int* vertexIndices, int nPoints, int elementsSize,
    double* xshift, double* yshift, double* zshift, int coordinateMask, double* scale, double* translation, int logMask)
{
    double coordinate;

    if (coordinateMask & 0x01)
    {
        coordinate = coordinates[vertexIndices[0]];

        if (xshift != NULL)
        {
            coordinate += xshift[vertexIndices[0]];
        }

        if (logMask & 0x01)
        {
            coordinate = DecompositionUtils::getLog10Value(coordinate);
        }

        buffer[offset +0] = coordinate * scale[0] + translation[0];
    }

    if (coordinateMask & 0x02)
    {
        coordinate = coordinates[vertexIndices[1] + nPoints];

        if (yshift != NULL)
        {
            coordinate += yshift[vertexIndices[1]];
        }

        if (logMask & 0x02)
        {
            coordinate = DecompositionUtils::getLog10Value(coordinate);
        }

        buffer[offset +1] = coordinate * scale[1] + translation[1];
    }

    if (coordinateMask & 0x04)
    {
        coordinate = coordinates[vertexIndices[2] + 2*nPoints];

        if (zshift != NULL)
        {
            coordinate += zshift[vertexIndices[2]];
        }

        if (logMask & 0x04)
        {
            coordinate = DecompositionUtils::getLog10Value(coordinate);
        }

        buffer[offset +2] = coordinate * scale[2] + translation[2];
    }

    if((elementsSize == 4) && (coordinateMask & 0x08))
    {
        buffer[offset +3] = 1.0;
    }

}

void PolylineDecomposer::fillStairDecompositionVertices(char* id, float* buffer, int bufferLength, int elementsSize, int coordinateMask, double* scale, double* translation,
    int logMask, double* coordinates, int nPoints, double* xshift, double* yshift, double* zshift)
{
    int i;

    int closed = 0;
    int* piClosed = &closed;

    /* Offsets of the left and right vertices (respectively) */
    int v0;
    int v1;

    int componentIndices[3];

    if (nPoints == 0)
    {
        return;
    }

    getGraphicObjectProperty(id, __GO_CLOSED__, jni_bool, (void**) &piClosed);

    for (i = 0; i < nPoints-1; i++)
    {
        v0 = elementsSize*2*i;
        v1 = elementsSize*(2*i+1);

        componentIndices[0] = i;
        componentIndices[1] = i;
        componentIndices[2] = i;

        getAndWriteVertexToBuffer(buffer, v0, coordinates, componentIndices, nPoints, elementsSize, xshift, yshift, zshift, coordinateMask, scale, translation, logMask);

        componentIndices[0] = i+1;
        componentIndices[1] = i;
        componentIndices[2] = i;

        /* To be optimized: the y and z components are fetched and transformed twice */
        getAndWriteVertexToBuffer(buffer, v1, coordinates, componentIndices, nPoints, elementsSize, xshift, yshift, zshift, coordinateMask, scale, translation, logMask);
    }

    /* Last point */
    v0 = elementsSize*2*(nPoints-1);

    componentIndices[0] = nPoints-1;
    componentIndices[1] = nPoints-1;
    componentIndices[2] = nPoints-1;

    getAndWriteVertexToBuffer(buffer, v0, coordinates, componentIndices, nPoints, elementsSize, xshift, yshift, zshift, coordinateMask, scale, translation, logMask);

    /*
     * One additional vertex if closed
     * Its x-coordinate is equal to the one of the polyline's first point
     * whereas its y and z coordinates are equal to those of the last point.
     */
    if (closed)
    {
        v0 = elementsSize*(2*nPoints-1);

        componentIndices[0] = 0;
        componentIndices[1] = nPoints-1;
        componentIndices[2] = nPoints-1;

        getAndWriteVertexToBuffer(buffer, v0, coordinates, componentIndices, nPoints, elementsSize, xshift, yshift, zshift, coordinateMask, scale, translation, logMask);
    }

}

void PolylineDecomposer::fillVerticalLinesDecompositionVertices(char* id, float* buffer, int bufferLength, int elementsSize, int coordinateMask, double* scale, double* translation,
    int logMask, double* coordinates, int nPoints, double* xshift, double* yshift, double* zshift)
{
    int i;

    for (i = 0; i < nPoints; i++)
    {
        /* Offsets of the lower and upper vertices (respectively) */
        int v0;
        int v1;

        /* Coordinates of the lower and upper vertices (respectively) */
        double coord0;
        double coord1;

        v0 = elementsSize*2*i;
        v1 = elementsSize*(2*i+1);

        /* Lower and upper endpoints x coordinates */
        if (coordinateMask & 0x01)
        {
            coord0 = coordinates[i];
            coord1 = coordinates[i];

            if (xshift != NULL)
            {
                coord0 += xshift[i];
                coord1 += xshift[i];
            }

            if (logMask & 0x01)
            {
                coord0 = DecompositionUtils::getLog10Value(coord0);
                coord1 = DecompositionUtils::getLog10Value(coord1);
            }

            buffer[v0 +0] = coord0 * scale[0] + translation[0];
            buffer[v1 +0] = coord1 * scale[0] + translation[0];
        }

        /* Lower and upper endpoints y coordinates */
        if (coordinateMask & 0x02)
        {
            coord0 = 0.0;
            coord1 = coordinates[i + nPoints];

            if (yshift != NULL)
            {
                /*
                 * Only the upper vertex's y coordinate is shifted,
                 * the lower vertex's one remains unchanged.
                 */
                coord1 += yshift[i];
            }

            if (logMask & 0x02)
            {
                /* The lower endpoint's y coordinate is unchanged (it amounts to log10(1), which is 0) */
                coord1 = DecompositionUtils::getLog10Value(coord1);
            }

            buffer[v0 +1] = coord0 * scale[1] + translation[1];
            buffer[v1 +1] = coord1 * scale[1] + translation[1];
        }

        /* Lower and upper endpoints z coordinates */
        if (coordinateMask & 0x04)
        {
            coord0 = coordinates[2*nPoints+i];
            coord1 = coordinates[2*nPoints+i];

            if (zshift != NULL)
            {
                coord0 += zshift[i];
                coord1 += zshift[i];
            }

            if (logMask & 0x04)
            {
                coord0 = DecompositionUtils::getLog10Value(coord0);
                coord1 = DecompositionUtils::getLog10Value(coord1);
            }

            buffer[v0 +2] = coord0 * scale[2] + translation[2];
            buffer[v1 +2] = coord1 * scale[2] + translation[2];
        }

        if((elementsSize == 4) && (coordinateMask & 0x08))
        {
            buffer[v0 +3] = 1.0;
            buffer[v1 +3] = 1.0;
        }

    }

}

void PolylineDecomposer::writeBarVerticesToBuffer(float* buffer, int* offsets, int componentOffset, double* coordinates, double shift, int shiftUsed,
    double scale, double translation, int logUsed)
{
    if (shiftUsed)
    {
        coordinates[0] += shift;
        coordinates[1] += shift;
        coordinates[2] += shift;
        coordinates[3] += shift;

        coordinates[4] += shift;
    }

    if (logUsed)
    {
        coordinates[0] = DecompositionUtils::getLog10Value(coordinates[0]);
        coordinates[1] = DecompositionUtils::getLog10Value(coordinates[1]);
        coordinates[2] = DecompositionUtils::getLog10Value(coordinates[2]);
        coordinates[3] = DecompositionUtils::getLog10Value(coordinates[3]);

        coordinates[4] = DecompositionUtils::getLog10Value(coordinates[4]);
    }

    buffer[offsets[0] +componentOffset] = coordinates[0] * scale + translation;
    buffer[offsets[1] +componentOffset] = coordinates[1] * scale + translation;
    buffer[offsets[2] +componentOffset] = coordinates[2] * scale + translation;
    buffer[offsets[3] +componentOffset] = coordinates[3] * scale + translation;

    buffer[offsets[4] +componentOffset] = coordinates[4] * scale + translation;
}

void PolylineDecomposer::fillVerticalBarsDecompositionVertices(char* id, float* buffer, int bufferLength, int elementsSize, int coordinateMask, double* scale, double* translation,
    int logMask, double* coordinates, int nPoints, double* xshift, double* yshift, double* zshift)
{
    double barWidth = 0.0;
    double* pdBarWidth = &barWidth;

    int i;

    int shiftUsed[3];
    int *piShiftUsed;

    /*
     * Offsets of the lower-left, lower-right, upper-right and upper-left bar vertices (respectively)
     * and of the polyline vertex proper
     */
    int offsets[5];

    /*
     * {x,y or z}-component values of a bar's 4 vertices (same ordering as the offsets)
     * and of the polyline vertex proper.
     */
    double coords[5];

    double shift;


    getGraphicObjectProperty(id, __GO_BAR_WIDTH__, jni_double, (void**) &pdBarWidth);

    piShiftUsed = &shiftUsed[0];
    getGraphicObjectProperty(id, __GO_DATA_MODEL_X_COORDINATES_SHIFT_SET__, jni_double_vector, (void**) &piShiftUsed);
    piShiftUsed = &shiftUsed[1];
    getGraphicObjectProperty(id, __GO_DATA_MODEL_Y_COORDINATES_SHIFT_SET__, jni_double_vector, (void**) &piShiftUsed);
    piShiftUsed = &shiftUsed[2];
    getGraphicObjectProperty(id, __GO_DATA_MODEL_Z_COORDINATES_SHIFT_SET__, jni_double_vector, (void**) &piShiftUsed);


    for (i = 0; i < nPoints; i++)
    {
        offsets[0] = elementsSize*4*i;
        offsets[1] = elementsSize*(4*i+1);
        offsets[2] = elementsSize*(4*i+2);
        offsets[3] = elementsSize*(4*i+3);

        offsets[4] = elementsSize*(4*nPoints+i);

        if (coordinateMask & 0x01)
        {
            coords[0] = coordinates[i] - 0.5*barWidth;
            coords[1] = coordinates[i] + 0.5*barWidth;
            coords[2] = coordinates[i] + 0.5*barWidth;
            coords[3] = coordinates[i] - 0.5*barWidth;

            coords[4] = coordinates[i];

            if (shiftUsed[0])
            {
                shift = xshift[i];
            }

            writeBarVerticesToBuffer(buffer, offsets, 0, coords, shift, shiftUsed[0], scale[0], translation[0], logMask & 0x01);
        }

        if (coordinateMask & 0x02)
        {
            coords[0] = 0.0;
            coords[1] = 0.0;
            coords[2] = coordinates[i + nPoints];
            coords[3] = coordinates[i + nPoints];

            coords[4] = coordinates[i + nPoints];

            if (shiftUsed[1])
            {
                shift = yshift[i];
            }

            if (logMask & 0x02)
            {
                /*
                 * The two lower endpoints' y coordinates must be set to 1
                 * since writeBarVerticesToBuffer applies the logarithmic transformation.
                 */
                coords[0] = 1.0;
                coords[1] = 1.0;
            }

            writeBarVerticesToBuffer(buffer, offsets, 1, coords, shift, shiftUsed[1], scale[1], translation[1], logMask & 0x02);
        }

        if (coordinateMask & 0x04)
        {
            coords[0] = coordinates[i + 2 * nPoints];
            coords[1] = coordinates[i + 2 * nPoints];
            coords[2] = coordinates[i + 2 * nPoints];
            coords[3] = coordinates[i + 2 * nPoints];

            coords[4] = coordinates[i + 2* nPoints];

            if (shiftUsed[2])
            {
                shift = zshift[i];
            }

            writeBarVerticesToBuffer(buffer, offsets, 2, coords, shift, shiftUsed[2], scale[2], translation[2], logMask & 0x04);
        }

        if((elementsSize == 4) && (coordinateMask & 0x08))
        {
            buffer[offsets[0] +3] = 1.0;
            buffer[offsets[1] +3] = 1.0;
            buffer[offsets[2] +3] = 1.0;
            buffer[offsets[3] +3] = 1.0;

            buffer[offsets[4] +3] = 1.0;
        }

    }

}

int PolylineDecomposer::getIndicesSize(char* id)
{
    int nPoints = 0;
    int *piNPoints = &nPoints;
    int polylineStyle = 0;
    int* piPolylineStyle = &polylineStyle;

    int lineMode = 0;
    int* piLineMode = &lineMode;

    int closed = 0;
    int* piClosed = &closed;

    getGraphicObjectProperty(id, __GO_POLYLINE_STYLE__, jni_int, (void**) &piPolylineStyle);
    getGraphicObjectProperty(id, __GO_DATA_MODEL_NUM_ELEMENTS__, jni_int, (void**) &piNPoints);
    getGraphicObjectProperty(id, __GO_LINE_MODE__, jni_bool, (void**) &piLineMode);
    getGraphicObjectProperty(id, __GO_CLOSED__, jni_bool, (void**) &piClosed);

    /* No segments if 0 points */
    if (nPoints == 0)
    {
        return 0;
    }

    /* Segments */
    if (polylineStyle == 1)
    {
        return getSegmentsDecompositionIndicesSize(nPoints, lineMode, closed);
    }
    /* Staircase */
    else if (polylineStyle == 2)
    {
        return getStairDecompositionIndicesSize(nPoints, lineMode, closed);
    }
    /* Vertical segments plus segments */
    else if (polylineStyle == 3)
    {
        return getVerticalLinesDecompositionIndicesSize(nPoints, lineMode);
    }
    /* Arrowed segments */
    else if (polylineStyle == 4)
    {
        return getSegmentsDecompositionIndicesSize(nPoints, lineMode, closed);
    }
    /* Vertical bars plus segments */
    else if (polylineStyle == 6)
    {
        return getVerticalBarsDecompositionIndicesSize(nPoints, lineMode);
    }
    else
    {
        return 0;
    }

}

int PolylineDecomposer::getSegmentsDecompositionIndicesSize(int nPoints, int lineMode, int closed)
{
    if (nPoints < 2)
    {
        return 0;
    }

    if (lineMode)
    {
        if (closed)
        {
            return 2*(nPoints);
        }
        else
        {
            return 2*(nPoints-1);
        }
    }
    else
    {
        return 0;
    }
}

int PolylineDecomposer::getStairDecompositionIndicesSize(int nPoints, int lineMode, int closed)
{
    if (nPoints < 2)
    {
        return 0;
    }

    if (lineMode)
    {
        if (closed)
        {
            return 4*(nPoints);
        }
        else
        {
            return 4*(nPoints-1);
        }
    }
    else
    {
        return 0;
    }
}

int PolylineDecomposer::getVerticalLinesDecompositionIndicesSize(int nPoints, int lineMode)
{
    if (nPoints == 0)
    {
        return 0;
    }

    if (lineMode)
    {
        return 2*(nPoints) + 2*(nPoints-1);
    }
    else
    {
        return 2*(nPoints);
    }
}

int PolylineDecomposer::getVerticalBarsDecompositionIndicesSize(int nPoints, int lineMode)
{
    if (nPoints == 0)
    {
        return 0;
    }

    if (lineMode)
    {
        return 2*4*(nPoints) + 2*(nPoints-1);
    }
    else
    {
        return 2*4*(nPoints);
    }
}

int PolylineDecomposer::fillIndices(char* id, int* buffer, int bufferLength, int logMask)
{
    double* coordinates;
    double* xshift;
    double* yshift;
    double* zshift;

    int polylineStyle = 0;
    int* piPolylineStyle = &polylineStyle;
    int nPoints = 0;
    int* piNPoints = &nPoints;
    int closed = 0;
    int* piClosed = &closed;
    int lineMode = 0;
    int* piLineMode = &lineMode;

    getGraphicObjectProperty(id, __GO_POLYLINE_STYLE__, jni_int, (void**) &piPolylineStyle);

    getGraphicObjectProperty(id, __GO_DATA_MODEL_COORDINATES__, jni_double_vector, (void**) &coordinates);
    getGraphicObjectProperty(id, __GO_DATA_MODEL_NUM_ELEMENTS__, jni_int, (void**) &piNPoints);
    getGraphicObjectProperty(id, __GO_DATA_MODEL_X_COORDINATES_SHIFT__, jni_double_vector, (void**) &xshift);
    getGraphicObjectProperty(id, __GO_DATA_MODEL_Y_COORDINATES_SHIFT__, jni_double_vector, (void**) &yshift);
    getGraphicObjectProperty(id, __GO_DATA_MODEL_Z_COORDINATES_SHIFT__, jni_double_vector, (void**) &zshift);

    getGraphicObjectProperty(id, __GO_LINE_MODE__, jni_bool, (void**) &piLineMode);
    getGraphicObjectProperty(id, __GO_CLOSED__, jni_bool, (void**) &piClosed);

    if (polylineStyle == 1)
    {
        return fillSegmentsDecompositionIndices(id, buffer, bufferLength, logMask, coordinates, nPoints, xshift, yshift, zshift, lineMode, closed);
    }
    else if (polylineStyle == 2)
    {
        return fillStairDecompositionIndices(id, buffer, bufferLength, logMask, coordinates, nPoints, xshift, yshift, zshift, lineMode, closed);
    }
    else if (polylineStyle == 3)
    {
        return fillVerticalLinesDecompositionIndices(id, buffer, bufferLength, logMask, coordinates, nPoints, xshift, yshift, zshift, lineMode);
    }
    else if (polylineStyle == 4)
    {
        return fillSegmentsDecompositionIndices(id, buffer, bufferLength, logMask, coordinates, nPoints, xshift, yshift, zshift, lineMode, closed);
    }
    else if (polylineStyle == 6)
    {
        return fillVerticalBarsDecompositionIndices(id, buffer, bufferLength, logMask, coordinates, nPoints, xshift, yshift, zshift, lineMode);
    }

    return 0;
}

int PolylineDecomposer::fillSegmentsDecompositionIndices(char* id, int* buffer, int bufferLength,
    int logMask, double* coordinates, int nPoints, double* xshift, double* yshift, double* zshift, int lineMode, int closed)
{
    double coordsi[3];

    int i;

    int currentValid;
    int nextValid;

    int offset = 0;
    int numberValidIndices = 0;

    /* If less than 2 points, no segments */
    if (nPoints < 2)
    {
        return 0;
    }

    if (lineMode == 0)
    {
        return 0;
    }

    getShiftedPolylinePoint(coordinates, xshift, yshift, zshift, nPoints, 0, &coordsi[0], &coordsi[1], &coordsi[2]);

    currentValid = DecompositionUtils::isValid(coordsi[0], coordsi[1], coordsi[2]);

    if (logMask)
    {
        currentValid &= DecompositionUtils::isLogValid(coordsi[0], coordsi[1], coordsi[2], logMask);
    }

    for (i = 0; i < nPoints-1; i++)
    {
        getShiftedPolylinePoint(coordinates, xshift, yshift, zshift, nPoints, i+1, &coordsi[0], &coordsi[1], &coordsi[2]);

        nextValid = DecompositionUtils::isValid(coordsi[0], coordsi[1], coordsi[2]);

        if (logMask)
        {
            nextValid &= DecompositionUtils::isLogValid(coordsi[0], coordsi[1], coordsi[2], logMask);
        }

        if (currentValid && nextValid)
        {
            buffer[2*offset] = i;
            buffer[2*offset+1] = i+1;

            numberValidIndices += 2;
            offset++;
        }

        currentValid = nextValid;
    }

    if (closed)
    {
        getShiftedPolylinePoint(coordinates, xshift, yshift, zshift, nPoints, 0, &coordsi[0], &coordsi[1], &coordsi[2]);

        nextValid = DecompositionUtils::isValid(coordsi[0], coordsi[1], coordsi[2]);

        if (logMask)
        {
            nextValid &= DecompositionUtils::isLogValid(coordsi[0], coordsi[1], coordsi[2], logMask);
        }

        if (currentValid && nextValid)
        {
            buffer[2*offset] = nPoints-1;
            buffer[2*offset+1] = 0;

            numberValidIndices += 2;
        }
    }

    return numberValidIndices;
}

int PolylineDecomposer::fillStairDecompositionIndices(char* id, int* buffer, int bufferLength,
    int logMask, double* coordinates, int nPoints, double* xshift, double* yshift, double* zshift, int lineMode, int closed)
{
    double coordsi[3];
    double coordsip1[3];

    int i;

    int currentValid;
    int middleVertexValid;
    int nextValid;

    int offset = 0;
    int numberValidIndices = 0;

    /* If less than 2 points, no segments */
    if (nPoints < 2)
    {
        return 0;
    }

    if (lineMode == 0)
    {
        return 0;
    }

    getShiftedPolylinePoint(coordinates, xshift, yshift, zshift, nPoints, 0, &coordsi[0], &coordsi[1], &coordsi[2]);

    currentValid = DecompositionUtils::isValid(coordsi[0], coordsi[1], coordsi[2]);

    if (logMask)
    {
        currentValid &= DecompositionUtils::isLogValid(coordsi[0], coordsi[1], coordsi[2], logMask);
    }

    for (i = 0; i < nPoints-1; i++)
    {
        getShiftedPolylinePoint(coordinates, xshift, yshift, zshift, nPoints, i+1, &coordsip1[0], &coordsip1[1], &coordsip1[2]);

        nextValid = DecompositionUtils::isValid(coordsip1[0], coordsip1[1], coordsip1[2]);
        middleVertexValid = DecompositionUtils::isValid(coordsip1[0], coordsi[1], coordsi[2]);

        if (logMask)
        {
            nextValid &= DecompositionUtils::isLogValid(coordsip1[0], coordsip1[1], coordsip1[2], logMask);
            middleVertexValid &= DecompositionUtils::isLogValid(coordsip1[0], coordsi[1], coordsi[2], logMask);
        }

        if (currentValid && middleVertexValid)
        {
            buffer[2*offset] = 2*i;
            buffer[2*offset+1] = 2*i+1;

            numberValidIndices += 2;
            offset++;
        }

        if ( middleVertexValid && nextValid)
        {
            buffer[2*offset] = 2*i+1;
            buffer[2*offset+1] = 2*i+2;

            numberValidIndices += 2;
            offset++;
        }

        coordsi[1] = coordsip1[1];
        coordsi[2] = coordsip1[2];

        currentValid = nextValid;
    }

    if (closed)
    {
        getShiftedPolylinePoint(coordinates, xshift, yshift, zshift, nPoints, 0, &coordsip1[0], &coordsip1[1], &coordsip1[2]);

        nextValid = DecompositionUtils::isValid(coordsip1[0], coordsip1[1], coordsip1[2]);
        middleVertexValid = DecompositionUtils::isValid(coordsip1[0], coordsi[1], coordsi[2]);

        if (logMask)
        {
            nextValid &= DecompositionUtils::isLogValid(coordsip1[0], coordsip1[1], coordsip1[2], logMask);
            middleVertexValid &= DecompositionUtils::isLogValid(coordsip1[0], coordsi[1], coordsi[2], logMask);
        }

        if (currentValid && middleVertexValid)
        {
            buffer[2*offset] = 2*nPoints-2;
            buffer[2*offset+1] = 2*nPoints-1;

            numberValidIndices += 2;
            offset++;
        }

        if (middleVertexValid && nextValid)
        {
            buffer[2*offset] = 2*nPoints-1;
            buffer[2*offset+1] = 0;

            numberValidIndices += 2;
        }

    }

    return numberValidIndices;
}

int PolylineDecomposer::fillVerticalLinesDecompositionIndices(char* id, int* buffer, int bufferLength,
    int logMask, double* coordinates, int nPoints, double* xshift, double* yshift, double* zshift, int lineMode)
{
    double coordsi[3];

    int i;
    int offset = 0;
    int numberValidIndices = 0;

    if (nPoints == 0)
    {
        return 0;
    }

    /* Vertical lines */
    for (i = 0; i < nPoints; i++)
    {
        getShiftedPolylinePoint(coordinates, xshift, yshift, zshift, nPoints, i, &coordsi[0], &coordsi[1], &coordsi[2]);

        if (DecompositionUtils::isValid(coordsi[0], coordsi[1], coordsi[2]))
        {
            if (logMask && !DecompositionUtils::isLogValid(coordsi[0], coordsi[1], coordsi[2], logMask))
            {
                continue;
            }

            buffer[2*offset] = 2*i;
            buffer[2*offset+1] = 2*i+1;

            numberValidIndices += 2;
            offset++;
        }

    }

    if (lineMode)
    {
        int currentValid;
        int nextValid;

        getShiftedPolylinePoint(coordinates, xshift, yshift, zshift, nPoints, 0, &coordsi[0], &coordsi[1], &coordsi[2]);

        currentValid = DecompositionUtils::isValid(coordsi[0], coordsi[1], coordsi[2]);

        if (logMask)
        {
            currentValid &= DecompositionUtils::isLogValid(coordsi[0], coordsi[1], coordsi[2], logMask);
        }

        for (i = 0; i < nPoints-1; i++)
        {
            getShiftedPolylinePoint(coordinates, xshift, yshift, zshift, nPoints, i+1, &coordsi[0], &coordsi[1], &coordsi[2]);

            nextValid = DecompositionUtils::isValid(coordsi[0], coordsi[1], coordsi[2]);

            if (logMask)
            {
                nextValid &= DecompositionUtils::isLogValid(coordsi[0], coordsi[1], coordsi[2], logMask);
            }

            if (currentValid && nextValid)
            {
                buffer[2*offset] = 2*i+1;
                buffer[2*offset+1] = 2*(i+1)+1;

                numberValidIndices += 2;
                offset++;
            }

            currentValid = nextValid;
        }
    }

    return numberValidIndices;
}


int PolylineDecomposer::fillVerticalBarsDecompositionIndices(char* id, int* buffer, int bufferLength,
    int logMask, double* coordinates, int nPoints, double* xshift, double* yshift, double* zshift, int lineMode)
{
    double coordsi[3];

    int i;
    int offset = 0;
    int numberValidIndices = 0;

    if (nPoints == 0)
    {
        return 0;
    }

    /* Bars */
    for (i = 0; i < nPoints; i++)
    {
        getShiftedPolylinePoint(coordinates, xshift, yshift, zshift, nPoints, i, &coordsi[0], &coordsi[1], &coordsi[2]);

        if (DecompositionUtils::isValid(coordsi[0], coordsi[1], coordsi[2]))
        {
            if (logMask && !DecompositionUtils::isLogValid(coordsi[0], coordsi[1], coordsi[2], logMask))
            {
                continue;
            }

            buffer[8*offset] = 4*i;
            buffer[8*offset+1] = 4*i+1;
            buffer[8*offset+2] = 4*i+1;
            buffer[8*offset+3] = 4*i+2;
            buffer[8*offset+4] = 4*i+2;
            buffer[8*offset+5] = 4*i+3;
            buffer[8*offset+6] = 4*i+3;
            buffer[8*offset+7] = 4*i;

            numberValidIndices += 8;
            offset++;
        }

    }

    /* Lines */
    if (lineMode)
    {
        int loffset = 0;

        int currentValid;
        int nextValid;

        getShiftedPolylinePoint(coordinates, xshift, yshift, zshift, nPoints, 0, &coordsi[0], &coordsi[1], &coordsi[2]);

        currentValid = DecompositionUtils::isValid(coordsi[0], coordsi[1], coordsi[2]);

        if (logMask)
        {
            currentValid &= DecompositionUtils::isLogValid(coordsi[0], coordsi[1], coordsi[2], logMask);
        }

        for (i = 0; i < nPoints-1; i++)
        {
            getShiftedPolylinePoint(coordinates, xshift, yshift, zshift, nPoints, i+1, &coordsi[0], &coordsi[1], &coordsi[2]);

            nextValid = DecompositionUtils::isValid(coordsi[0], coordsi[1], coordsi[2]);

            if (logMask)
            {
                nextValid &= DecompositionUtils::isLogValid(coordsi[0], coordsi[1], coordsi[2], logMask);
            }

            if (currentValid && nextValid)
            {
                buffer[8*offset+2*loffset] = 4*nPoints +i;
                buffer[8*offset+2*loffset+1] = 4*nPoints +i+1;

                numberValidIndices += 2;
                loffset++;
            }

            currentValid = nextValid;
        }
    }

    return numberValidIndices;
}

void PolylineDecomposer::getShiftedPolylinePoint(double* coordinates, double* xshift, double* yshift, double* zshift, int nPoints, int index,
    double* x, double* y, double* z)
{
    *x = coordinates[index];

    if (xshift != NULL)
    {
        *x += xshift[index];
    }

    *y = coordinates[index + nPoints];

    if (yshift != NULL)
    {
        *y += yshift[index];
    }

    *z = coordinates[index + 2*nPoints];

    if (zshift != NULL)
    {
        *z += zshift[index];
    }

}

